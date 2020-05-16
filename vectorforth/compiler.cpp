#include "compiler.h"
#include "compile_data.h"
#include "compile_error.h"
#include "context_defs.h"
#include "primitives.h"
#include "asm_aux.h"
#include "tokenize.h"

#include <cassert>
#include <sstream>
#include <algorithm>

using namespace ASM;

VF_BEGIN

void compile_primitive(asmcode& code, dictionary& d, compile_data& cd, token word)
  {
  static prim_map pm = generate_primitives_map();
  if (word.value == "create")
    {
    if (cd.create_called)
      throw std::runtime_error(compile_error_text(VF_ERROR_CREATE_WAS_ALREADY_CALLED, word.line_nr, word.column_nr).c_str());
    cd.create_called = true;
    code.add(asmcode::MOV, asmcode::RAX, CONSTANT_SPACE_POINTER);
    if (cd.constant_space_offset)
      code.add(asmcode::ADD, asmcode::RAX, asmcode::NUMBER, cd.constant_space_offset);
    code.add(asmcode::VMOVAPS, asmcode::YMM0, MEM_STACK_REGISTER);
    code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, CELLS(4));   
    code.add(asmcode::VMOVAPS, asmcode::MEM_RAX, asmcode::YMM0);
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
        de.address = cd.constant_space_offset;
        cd.constant_space_offset += 32;
        push(d, de);
        return;
        }
      else
        throw std::runtime_error(compile_error_text(VF_ERROR_WORD_UNKNOWN, word.line_nr, word.column_nr, word.value).c_str());
      }
    it->second(code, cd);
    }
  }

void compile_variable(asmcode& code, compile_data& cd, uint64_t address)
  {
  if (cd.to_called)
    {
    cd.to_called = false;
    code.add(asmcode::MOV, asmcode::RAX, CONSTANT_SPACE_POINTER);
    if (address)
      code.add(asmcode::ADD, asmcode::RAX, asmcode::NUMBER, address);
    code.add(asmcode::VMOVAPS, asmcode::YMM0, MEM_STACK_REGISTER);
    code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, CELLS(4));
    code.add(asmcode::VMOVAPS, asmcode::MEM_RAX, asmcode::YMM0);
    }
  else
    {
    code.add(asmcode::MOV, asmcode::RAX, CONSTANT_SPACE_POINTER);
    if (address)
      code.add(asmcode::ADD, asmcode::RAX, asmcode::NUMBER, address);
    code.add(asmcode::VMOVAPS, asmcode::YMM0, asmcode::MEM_RAX);
    code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, CELLS(4));
    code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, asmcode::YMM0);
    }
  }

void compile_word(asmcode& code, dictionary& d, compile_data& cd, token word)
  {
  dictionary_entry e;
  if (find(e, d, word.value))
    {
    switch (e.type)
      {
      case dictionary_entry::T_FUNCTION:
      {
      compile_words(code, d, cd, e.words);
      break;
      }
      case dictionary_entry::T_VARIABLE:
      {
      if (cd.create_called)  // overwriting existing variable
        {
        code.add(asmcode::MOV, asmcode::RAX, CONSTANT_SPACE_POINTER);
        code.add(asmcode::ADD, asmcode::RAX, asmcode::NUMBER, cd.constant_space_offset);
        code.add(asmcode::VMOVAPS, asmcode::YMM0, asmcode::MEM_RAX);
        code.add(asmcode::SUB, asmcode::RAX, asmcode::NUMBER, (cd.constant_space_offset - e.address));
        code.add(asmcode::VMOVAPS, asmcode::MEM_RAX, asmcode::YMM0);
        cd.create_called = false;
        return;
        }
      else
        compile_variable(code, cd, e.address);
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
    compile_primitive(code, d, cd, word);
    }
  }

void compile_float(asmcode& code, token word)
  {
  assert(word.type == token::T_FLOAT);
  float f = to_float(word.value.c_str());
  uint32_t v = *(reinterpret_cast<uint32_t*>(&f));
  uint64_t v64 = (uint64_t)v;
  uint64_t v2 = (v64 << 32) | v64;
  code.add(asmcode::MOV, asmcode::RAX, asmcode::NUMBER, v2);
  code.add(asmcode::MOV, MEM_STACK_REGISTER, -CELLS(1), asmcode::RAX);
  code.add(asmcode::MOV, MEM_STACK_REGISTER, -CELLS(2), asmcode::RAX);
  code.add(asmcode::MOV, MEM_STACK_REGISTER, -CELLS(3), asmcode::RAX);
  code.add(asmcode::MOV, MEM_STACK_REGISTER, -CELLS(4), asmcode::RAX);
  code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, CELLS(4));
  }

void compile_integer(asmcode& code, token word)
  {
  assert(word.type == token::T_INTEGER);
  std::stringstream ss;
  ss << word.value;
  int64_t v;
  ss >> v;
  code.add(asmcode::MOV, asmcode::RAX, asmcode::NUMBER, v);
  code.add(asmcode::MOV, MEM_STACK_REGISTER, -CELLS(4), asmcode::RAX);
  code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, CELLS(4));
  }

void compile_vector8(asmcode& code, const std::vector<token>& words)
  {
  assert(words.size() == 8);
  for (int i = 0; i < 4; ++i)
    {
    assert(words[2 * i].type == token::T_FLOAT || words[2 * i].type == token::T_INTEGER);
    assert(words[2 * i + 1].type == token::T_FLOAT || words[2 * i + 1].type == token::T_INTEGER);
    uint64_t v1_64, v2_64;
    if (words[2 * i].type == token::T_FLOAT)
      {
      float f = to_float(words[i * 2].value.c_str());
      uint32_t v = *(reinterpret_cast<uint32_t*>(&f));
      v1_64 = (uint64_t)v;
      }
    else
      {
      std::stringstream ss;
      ss << words[i * 2].value;
      uint32_t v;
      ss >> v;
      v1_64 = (uint64_t)v;
      }

    if (words[2 * i + 1].type == token::T_FLOAT)
      {
      float f = to_float(words[i * 2 + 1].value.c_str());
      uint32_t v = *(reinterpret_cast<uint32_t*>(&f));
      v2_64 = (uint64_t)v;
      }
    else
      {
      std::stringstream ss;
      ss << words[i * 2 + 1].value;
      uint32_t v;
      ss >> v;
      v2_64 = (uint64_t)v;
      }
    uint64_t v = (v1_64 << 32) | v2_64;
    code.add(asmcode::MOV, asmcode::RAX, asmcode::NUMBER, v);
    code.add(asmcode::MOV, MEM_STACK_REGISTER, -CELLS(i + 1), asmcode::RAX);
    }
  code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, CELLS(4));
  }

void compile_definition(dictionary& d, std::vector<token>& words, int line_nr, int column_nr)
  {
  if (words.empty())
    throw std::runtime_error(compile_error_text(VF_ERROR_EMPTY_DEFINITION, line_nr, column_nr).c_str());
  if (words.front().type != token::T_WORD)
    throw std::runtime_error(compile_error_text(VF_ERROR_INVALID_DEFINITION_NAME, words.front().line_nr, words.front().column_nr).c_str());
  register_definition(d, words);
  }

void compile_words(asmcode& code, dictionary& d, compile_data& cd, std::vector<token>& words)
  {
  while (!words.empty())
    {
    token word = words.back();
    words.pop_back();
    switch (word.type)
      {
      case token::T_WORD:
      {
      compile_word(code, d, cd, word);
      break;
      }
      case token::T_PRIMITIVE:
      {
      compile_primitive(code, d, cd, word);
      break;
      }
      case token::T_FLOAT:
      {
      compile_float(code, word);
      break;
      }
      case token::T_VECTOR8:
      {
      std::vector<token> vector_words;
      if (words.size() < 8)
        throw std::runtime_error(compile_error_text(VF_ERROR_VECTOR8_INVALID_SYNTAX, word.line_nr, word.column_nr).c_str());
      for (int i = 0; i < 8; ++i)
        {
        vector_words.push_back(words.back());
        if (vector_words.back().type != token::T_FLOAT && vector_words.back().type != token::T_INTEGER)
          throw std::runtime_error(compile_error_text(VF_ERROR_VECTOR8_INVALID_SYNTAX, word.line_nr, word.column_nr).c_str());
        words.pop_back();
        }
      compile_vector8(code, vector_words);
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
      compile_definition(d, definition_words, word.line_nr, word.column_nr);
      break;
      }
      case token::T_SEMICOLON:
      {
      throw std::runtime_error(compile_error_text(VF_ERROR_NO_CORRESPONDING_COLON, word.line_nr, word.column_nr).c_str());
      break;
      }
      case token::T_INTEGER:
      {
      compile_integer(code, word);
      break;
      }
      default:
        throw std::runtime_error("not implemented");
      }
    }
  }

void compile(asmcode& code, dictionary& d, compile_data& cd, std::vector<token> words)
  {
  assert(!cd.to_called);
  assert(!cd.create_called);
  std::reverse(words.begin(), words.end());

  code.add(asmcode::GLOBAL, "forth_entry");
#ifdef _WIN32
  /*
  windows parameters calling convention: rcx, rdx, r8, r9
  First parameter (rcx) points to the context.
  We store the pointer to the context in register r10.
  */
  code.add(asmcode::MOV, CONTEXT, asmcode::RCX);
#else
  /*
  Linux parameters calling convention: rdi, rsi, rdx, rcx, r8, r9
  First parameter (rdi) points to the context.
  We store the pointer to the context in register r10.
  */
  code.add(asmcode::MOV, CONTEXT, asmcode::RDI);
#endif

  /*
  Save the current content of the registers in the context
  */
  store_registers(code);

  code.add(asmcode::MOV, STACK_REGISTER, STACK_POINTER);

  code.add(asmcode::MOV, HERE, HERE_POINTER);

  /*
  Align stack with 32 byte boundary
  */
  code.add(asmcode::AND, asmcode::RSP, asmcode::NUMBER, 0xFFFFFFFFFFFFFFE0);

  code.add(asmcode::MOV, RSP_TOP, asmcode::RSP);

  compile_words(code, d, cd, words);

  code.add(asmcode::MOV, STACK_POINTER, STACK_REGISTER);

  /*Restore the registers to their original state*/
  load_registers(code);

  /*Return to the caller*/
  code.add(asmcode::RET);

  if (cd.to_called)
    throw std::runtime_error(compile_error_text(VF_ERROR_UNCLEAR_TARGET_FOR_TO, -1, -1).c_str());
  if (cd.create_called)
    throw std::runtime_error(compile_error_text(VF_ERROR_UNCLEAR_TARGET_FOR_CREATE, -1, -1).c_str());
  }

VF_END