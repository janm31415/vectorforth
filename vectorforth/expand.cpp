#include "expand.h"
#include "expand_data.h"
#include "compile_error.h"
#include "primitives.h"
#include "context_defs.h"

#include <cassert>
#include <sstream>

VF_BEGIN

void expand_primitive(std::vector<expanded_token>& expanded, dictionary& d, expand_data& cd, token word)
  {
  static prim_map pm = generate_primitives_map();
  if (word.value == "create")
    {
    if (cd.create_called)
      throw std::runtime_error(compile_error_text(VF_ERROR_CREATE_WAS_ALREADY_CALLED, word.line_nr, word.column_nr).c_str());
    cd.create_called = true;
    expanded.emplace_back(expanded_token::ET_CREATE_VARIABLE);
    expanded.back().binding_space_offset = cd.binding_space_offset;
    }
  else if (word.value == "to")
    {
    if (cd.to_called || cd.create_called)
      throw std::runtime_error(compile_error_text(VF_ERROR_UNCLEAR_TARGET_FOR_TO, word.line_nr, word.column_nr).c_str());
    cd.to_called = true;
    }
  else
    {
    auto it = pm.find(word.value);
    if (it == pm.end())
      {
      if (cd.create_called)
        {
        cd.create_called = false;
        dictionary_entry de;
        de.type = dictionary_entry::T_VARIABLE;
        de.name = word.value;
        de.address = cd.binding_space_offset;
        cd.binding_space_offset += AVX_ALIGNMENT;
        push(d, de);
        return;
        }
      else
        throw std::runtime_error(compile_error_text(VF_ERROR_WORD_UNKNOWN, word.line_nr, word.column_nr, word.value).c_str());
      }
    expanded.emplace_back(expanded_token::ET_PRIMITIVE);
    expanded.back().prim = it->second;
    }
  }

void expand_variable(std::vector<expanded_token>& expanded, expand_data& cd, uint64_t address)
  {
  if (cd.to_called)
    {
    cd.to_called = false;
    expanded.emplace_back(expanded_token::ET_UPDATE_VARIABLE);
    expanded.back().variable_address = address;
    }
  else
    {
    expanded.emplace_back(expanded_token::ET_VARIABLE);
    expanded.back().variable_address = address;
    }
  }

void expand_word(std::vector<expanded_token>& expanded, dictionary& d, expand_data& cd, token word)
  {
  dictionary_entry e;
  if (find(e, d, word.value))
    {
    switch (e.type)
      {
      case dictionary_entry::T_FUNCTION:
      {
      expand_words(expanded, d, cd, e.words);
      break;
      }
      case dictionary_entry::T_VARIABLE:
      {
      if (cd.create_called)  // overwriting existing variable
        {
        expanded.emplace_back(expanded_token::ET_OVERWRITE_VARIABLE);
        expanded.back().binding_space_offset = cd.binding_space_offset;
        expanded.back().variable_address = e.address;
        cd.create_called = false;
        return;
        }
      else
        expand_variable(expanded, cd, e.address);
      break;
      }
      default:
      {
      throw std::runtime_error("compiler error: not implemented");
      break;
      }
      }
    }
  else
    {
    expand_primitive(expanded, d, cd, word);
    }
  }

void expand_float(std::vector<expanded_token>& expanded, token word)
  {
  assert(word.type == token::T_FLOAT);
  expanded.emplace_back(word);
  }

void expand_integer(std::vector<expanded_token>& expanded, token word)
  {
  assert(word.type == token::T_INTEGER);  
  expanded.emplace_back(word);
  }

void expand_vector(std::vector<expanded_token>& expanded, const std::vector<token>& words, token current)
  {  
  expanded_token et(current);

  assert(words.size() == AVX_LENGTH);
  for (int i = 0; i < AVX_LENGTH; ++i)
    {
    assert(words[i].type == token::T_FLOAT || words[i].type == token::T_INTEGER);    

    
    if (words[i].type == token::T_FLOAT)
      {
      float f = to_float(words[i].value.c_str());
      et.f[i] = f;
      }
    else
      {
      std::stringstream ss;
      ss << words[i].value;
      uint32_t v;
      ss >> v;
      et.f[i] = *(reinterpret_cast<float*>(&v));
      }
    }
  expanded.push_back(et);
  }

void expand_definition(dictionary& d, std::vector<token>& words, int line_nr, int column_nr)
  {
  if (words.empty())
    throw std::runtime_error(compile_error_text(VF_ERROR_EMPTY_DEFINITION, line_nr, column_nr).c_str());
  if (words.front().type != token::T_WORD)
    throw std::runtime_error(compile_error_text(VF_ERROR_INVALID_DEFINITION_NAME, words.front().line_nr, words.front().column_nr).c_str());
  register_definition(d, words);
  }

void expand_words(std::vector<expanded_token>& expanded, dictionary& d, expand_data& cd, std::vector<token>& words)
  {
  while (!words.empty())
    {
    token word = words.back();
    words.pop_back();
    switch (word.type)
      {
      case token::T_WORD:
      {
      expand_word(expanded, d, cd, word);
      break;
      }
      case token::T_PRIMITIVE:
      {
      expand_primitive(expanded, d, cd, word);
      break;
      }
      case token::T_FLOAT:
      {
      expand_float(expanded, word);
      break;
      }
      case token::T_VECTOR:
      {
      std::vector<token> vector_words;
      if (words.size() < AVX_LENGTH)
#ifdef AVX512
        throw std::runtime_error(compile_error_text(VF_ERROR_VECTOR16_INVALID_SYNTAX, word.line_nr, word.column_nr).c_str());
#else
        throw std::runtime_error(compile_error_text(VF_ERROR_VECTOR8_INVALID_SYNTAX, word.line_nr, word.column_nr).c_str());
#endif
      for (int i = 0; i < AVX_LENGTH; ++i)
        {
        vector_words.push_back(words.back());
        if (vector_words.back().type != token::T_FLOAT && vector_words.back().type != token::T_INTEGER)
#ifdef AVX512
          throw std::runtime_error(compile_error_text(VF_ERROR_VECTOR16_INVALID_SYNTAX, word.line_nr, word.column_nr).c_str());
#else
          throw std::runtime_error(compile_error_text(VF_ERROR_VECTOR8_INVALID_SYNTAX, word.line_nr, word.column_nr).c_str());
#endif
        words.pop_back();
        }
      expand_vector(expanded, vector_words, word);
      break;
      }
      case token::T_COLON:
      {
      std::vector<token> definition_words;
      if (words.empty())
        throw std::runtime_error(compile_error_text(VF_ERROR_NO_CORRESPONDING_SEMICOLON, word.line_nr, word.column_nr).c_str());
      while (!words.empty())
        {
        definition_words.push_back(words.back());
        words.pop_back();
        if (definition_words.back().type == token::T_SEMICOLON)
          break;
        }
      if (definition_words.back().type != token::T_SEMICOLON)
        throw std::runtime_error(compile_error_text(VF_ERROR_NO_CORRESPONDING_SEMICOLON, word.line_nr, word.column_nr).c_str());
      definition_words.pop_back(); // last item is semicolon, we don't need that for the definition
      expand_definition(d, definition_words, word.line_nr, word.column_nr);
      break;
      }
      case token::T_SEMICOLON:
      {
      throw std::runtime_error(compile_error_text(VF_ERROR_NO_CORRESPONDING_COLON, word.line_nr, word.column_nr).c_str());
      break;
      }
      case token::T_INTEGER:
      {
      expand_integer(expanded, word);
      break;
      }
      default:
        throw std::runtime_error("not implemented");
      }
    }
  }

void expand(std::vector<expanded_token>& expanded, dictionary& d, expand_data& cd, std::vector<token> words)
  {
  assert(!cd.to_called);
  assert(!cd.create_called);
  std::reverse(words.begin(), words.end());

  expand_words(expanded, d, cd, words);

  if (cd.to_called)
    throw std::runtime_error(compile_error_text(VF_ERROR_UNCLEAR_TARGET_FOR_TO, -1, -1).c_str());
  if (cd.create_called)
    throw std::runtime_error(compile_error_text(VF_ERROR_UNCLEAR_TARGET_FOR_CREATE, -1, -1).c_str());
  }

VF_END