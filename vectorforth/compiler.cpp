#include "compiler.h"
#include "compile_data.h"
#include "compile_error.h"
#include "constant_folding.h"
#include "context_defs.h"
#include "expand.h"
#include "expand_data.h"
#include "primitives.h"
#include "strength_reduction.h"
#include "superoperators.h"
#include "asm_aux.h"
#include "tokenize.h"

#include <cassert>
#include <sstream>
#include <algorithm>

using namespace ASM;

VF_BEGIN

/////////////////////////////////////////////////////////////
// Singlepass compiler
/////////////////////////////////////////////////////////////

void compile_words_single_pass(asmcode& code, dictionary& d, expand_data& ed, compile_data& cd, std::vector<token>& words);
void compile_word_single_pass(asmcode& code, dictionary& d, expand_data& ed, compile_data& cd, token word);

void compile_primitive_single_pass(asmcode& code, dictionary& d, expand_data& ed, compile_data& cd, token word)
  {
  static prim_map pm = generate_primitives_map();
  if (word.value == "create")
    {
    if (ed.create_called)
      throw std::runtime_error(compile_error_text(VF_ERROR_CREATE_WAS_ALREADY_CALLED, word.line_nr, word.column_nr).c_str());
    ed.create_called = true;
    code.add(asmcode::COMMENT, "BEGIN CREATE VARIABLE");
    code.add(asmcode::MOV, asmcode::RAX, CONSTANT_SPACE_POINTER);
    if (ed.binding_space_offset)
      code.add(asmcode::ADD, asmcode::RAX, asmcode::NUMBER, ed.binding_space_offset);
    code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
    code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
    code.add(asmcode::VMOVAPS, asmcode::MEM_RAX, AVX_REG0);
    code.add(asmcode::COMMENT, "END CREATE VARIABLE");
    }
  else if (word.value == "to")
    {
    if (ed.to_called || ed.create_called)
      throw std::runtime_error(compile_error_text(VF_ERROR_UNCLEAR_TARGET_FOR_TO, word.line_nr, word.column_nr).c_str());
    ed.to_called = true;
    }
  else
    {
    auto it = pm.find(word.value);
    if (it == pm.end())
      {
      if (ed.create_called)
        {
        ed.create_called = false;
        dictionary_entry de;
        de.type = dictionary_entry::T_VARIABLE;
        de.name = word.value;
        de.address = ed.binding_space_offset;
        ed.binding_space_offset += AVX_ALIGNMENT;
        push(d, de);
        return;
        }
      else
        {
        dictionary_entry e;
        if (find(e, d, word.value))
          {
          compile_word_single_pass(code, d, ed, cd, word);
          return;
          }
        else
          throw std::runtime_error(compile_error_text(VF_ERROR_WORD_UNKNOWN, word.line_nr, word.column_nr, word.value).c_str());
        }
      }
    it->second(code, cd);
    }
  }

void compile_variable_single_pass(asmcode& code, expand_data& ed, uint64_t address)
  {
  if (ed.to_called)
    {
    code.add(asmcode::COMMENT, "BEGIN UPDATE VARIABLE VALUE");
    ed.to_called = false;
    code.add(asmcode::MOV, asmcode::RAX, CONSTANT_SPACE_POINTER);
    if (address)
      code.add(asmcode::ADD, asmcode::RAX, asmcode::NUMBER, address);
    code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
    code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
    code.add(asmcode::VMOVAPS, asmcode::MEM_RAX, AVX_REG0);
    code.add(asmcode::COMMENT, "END UPDATE VARIABLE VALUE");
    }
  else
    {
    code.add(asmcode::COMMENT, "BEGIN PUSH VARIABLE ADDRESS OR VALUE ON STACK");
    code.add(asmcode::MOV, asmcode::RAX, CONSTANT_SPACE_POINTER);
    if (address)
      code.add(asmcode::ADD, asmcode::RAX, asmcode::NUMBER, address);
    code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::MEM_RAX);
    code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
    code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
    code.add(asmcode::COMMENT, "END PUSH VARIABLE ADDRESS OR VALUE ON STACK");
    }
  }

void compile_word_single_pass(asmcode& code, dictionary& d, expand_data& ed, compile_data& cd, token word)
  {
  dictionary_entry e;
  if (find(e, d, word.value))
    {
    switch (e.type)
      {
      case dictionary_entry::T_FUNCTION:
      {
      compile_words_single_pass(code, d, ed, cd, e.words);
      break;
      }
      case dictionary_entry::T_VARIABLE:
      {
      if (ed.create_called)  // overwriting existing variable
        {
        code.add(asmcode::COMMENT, "BEGIN OVERWRITE EXISTING VARIABLE");
        code.add(asmcode::MOV, asmcode::RAX, CONSTANT_SPACE_POINTER);
        code.add(asmcode::ADD, asmcode::RAX, asmcode::NUMBER, ed.binding_space_offset);
        code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::MEM_RAX);
        code.add(asmcode::SUB, asmcode::RAX, asmcode::NUMBER, (ed.binding_space_offset - e.address));
        code.add(asmcode::VMOVAPS, asmcode::MEM_RAX, AVX_REG0);
        code.add(asmcode::COMMENT, "END OVERWRITE EXISTING VARIABLE");
        ed.create_called = false;
        return;
        }
      else
        compile_variable_single_pass(code, ed, e.address);
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
    compile_primitive_single_pass(code, d, ed, cd, word);
    }
  }

void compile_float_single_pass(asmcode& code, token word)
  {
  code.add(asmcode::COMMENT, "BEGIN PUSH FLOAT ON THE STACK");
  assert(word.type == token::T_FLOAT);
  float f = to_float(word.value.c_str());
  uint32_t v = *(reinterpret_cast<uint32_t*>(&f));
  code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG0, DWORD_MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "BEGIN END FLOAT ON THE STACK");
  }

void compile_integer_single_pass(asmcode& code, token word)
  {
  code.add(asmcode::COMMENT, "BEGIN PUSH ADDRESS ON THE STACK");
  assert(word.type == token::T_INTEGER);
  std::stringstream ss;
  ss << word.value;
  int64_t v;
  ss >> v;
  code.add(asmcode::MOV, asmcode::RAX, asmcode::NUMBER, v);
  code.add(asmcode::MOV, MEM_STACK_REGISTER, -AVX_CELLS(1), asmcode::RAX);
  code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::COMMENT, "END PUSH FLOAT ON THE STACK");
  }

void compile_vector_single_pass(asmcode& code, const std::vector<token>& words)
  {
  code.add(asmcode::COMMENT, "BEGIN PUSH VECTOR ON THE STACK");
  assert(words.size() == AVX_LENGTH);
  for (int i = 0; i < AVX_LENGTH/2; ++i)
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
  code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::COMMENT, "END PUSH FLOAT ON THE STACK");
  }

void compile_definition_single_pass(dictionary& d, std::vector<token>& words, int line_nr, int column_nr)
  {
  if (words.empty())
    throw std::runtime_error(compile_error_text(VF_ERROR_EMPTY_DEFINITION, line_nr, column_nr).c_str());
  if (words.front().type != token::T_WORD)
    throw std::runtime_error(compile_error_text(VF_ERROR_INVALID_DEFINITION_NAME, words.front().line_nr, words.front().column_nr).c_str());
  register_definition(d, words);
  }

void compile_words_single_pass(asmcode& code, dictionary& d, expand_data& ed, compile_data& cd, std::vector<token>& words)
  {
  while (!words.empty())
    {
    token word = words.back();
    words.pop_back();
    switch (word.type)
      {
      case token::T_WORD:
      {
      compile_word_single_pass(code, d, ed, cd, word);
      break;
      }
      case token::T_PRIMITIVE:
      {
      compile_primitive_single_pass(code, d, ed, cd, word);
      break;
      }
      case token::T_FLOAT:
      {
      compile_float_single_pass(code, word);
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
      compile_vector_single_pass(code, vector_words);
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
      compile_definition_single_pass(d, definition_words, word.line_nr, word.column_nr);
      break;
      }
      case token::T_SEMICOLON:
      {
      throw std::runtime_error(compile_error_text(VF_ERROR_NO_CORRESPONDING_COLON, word.line_nr, word.column_nr).c_str());
      break;
      }
      case token::T_INTEGER:
      {
      compile_integer_single_pass(code, word);
      break;
      }
      default:
        throw std::runtime_error("not implemented");
      }
    }
  }

void compile_single_pass(asmcode& code, dictionary& d, expand_data& ed, std::vector<token> words)
  {
  assert(!ed.to_called);
  assert(!ed.create_called);
  std::reverse(words.begin(), words.end());

  compile_data cd = create_compile_data();

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
  Align stack with 64 byte boundary
  */
  code.add(asmcode::AND, asmcode::RSP, asmcode::NUMBER, 0xFFFFFFFFFFFFFFC0);

  code.add(asmcode::MOV, RSP_TOP, asmcode::RSP);

  compile_words_single_pass(code, d, ed, cd, words);

  code.add(asmcode::MOV, STACK_POINTER, STACK_REGISTER);

  /*Restore the registers to their original state*/
  load_registers(code);

  /*Return to the caller*/
  code.add(asmcode::RET);

  if (ed.to_called)
    throw std::runtime_error(compile_error_text(VF_ERROR_UNCLEAR_TARGET_FOR_TO, -1, -1).c_str());
  if (ed.create_called)
    throw std::runtime_error(compile_error_text(VF_ERROR_UNCLEAR_TARGET_FOR_CREATE, -1, -1).c_str());
  }








/////////////////////////////////////////////////////////////
// Multipass compiler
/////////////////////////////////////////////////////////////




void compile_words(asmcode& code, compile_data& cd, std::vector<expanded_token>& words)
  {
  while (!words.empty())
    {
    expanded_token word = words.back();
    words.pop_back();
    switch (word.t)
      {
      case expanded_token::ET_PRIMITIVE:
      {
      word.prim(code, cd);
      break;
      }
      case expanded_token::ET_SUPEROPERATOR:
      {
      word.supop(code, word);
      break;
      }
      case expanded_token::ET_INTEGER:
      {
      code.add(asmcode::COMMENT, "BEGIN PUSH ADDRESS ON THE STACK");
      code.add(asmcode::MOV, asmcode::RAX, asmcode::NUMBER, word.int_value);
      code.add(asmcode::MOV, MEM_STACK_REGISTER, -AVX_CELLS(1), asmcode::RAX);
      code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
      code.add(asmcode::COMMENT, "END PUSH ADDRESS ON THE STACK");
      break;
      }
      case expanded_token::ET_FLOAT:
      {
      code.add(asmcode::COMMENT, "BEGIN PUSH FLOAT ON THE STACK");
      uint32_t v = *(reinterpret_cast<uint32_t*>(&word.f[0]));
      code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
      code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
      code.add(asmcode::VBROADCASTSS, AVX_REG0, DWORD_MEM_STACK_REGISTER);
      code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
      code.add(asmcode::COMMENT, "END PUSH FLOAT ON THE STACK");
      break;
      }
      case expanded_token::ET_FLOAT2:
      {
      code.add(asmcode::COMMENT, "BEGIN PUSH TWO FLOATS ON THE STACK");
      uint32_t v1 = *(reinterpret_cast<uint32_t*>(&word.f[1]));
      uint32_t v2 = *(reinterpret_cast<uint32_t*>(&word.f[0]));     
      code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, -AVX_CELLS(1), asmcode::NUMBER, v1);
      if (v2 == v1)
        {
        code.add(asmcode::VBROADCASTSS, AVX_REG0, DWORD_MEM_STACK_REGISTER, -AVX_CELLS(1));
        code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, -AVX_CELLS(1), AVX_REG0);
        code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, -AVX_CELLS(2), AVX_REG0);
        }
      else
        {
        code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, -AVX_CELLS(1) - 4, asmcode::NUMBER, v2);
        code.add(asmcode::VBROADCASTSS, AVX_REG0, DWORD_MEM_STACK_REGISTER, -AVX_CELLS(1));
        code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER, -AVX_CELLS(1) - 4);
        code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, -AVX_CELLS(1), AVX_REG1);
        code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, -AVX_CELLS(2), AVX_REG0);
        }
      code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(2));
      code.add(asmcode::COMMENT, "END PUSH TWO FLOATS ON THE STACK");
      break;
      }
      case expanded_token::ET_FLOAT3:
      {
      code.add(asmcode::COMMENT, "BEGIN PUSH THREE FLOATS ON THE STACK");
      uint32_t v1 = *(reinterpret_cast<uint32_t*>(&word.f[2]));
      uint32_t v2 = *(reinterpret_cast<uint32_t*>(&word.f[1]));
      uint32_t v3 = *(reinterpret_cast<uint32_t*>(&word.f[0]));
      code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, -AVX_CELLS(1), asmcode::NUMBER, v1);
      code.add(asmcode::VBROADCASTSS, AVX_REG0, DWORD_MEM_STACK_REGISTER, -AVX_CELLS(1));
      if (v1 == v2)
        {
        code.add(asmcode::VMOVAPS, AVX_REG1, AVX_REG0);
        }
      else
        {
        code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, -AVX_CELLS(1) - 4, asmcode::NUMBER, v2);
        code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER, -AVX_CELLS(1) - 4);
        }
      if (v1 == v3)
        {
        code.add(asmcode::VMOVAPS, AVX_REG2, AVX_REG0);
        }
      else if (v2 == v3)
        {
        code.add(asmcode::VMOVAPS, AVX_REG2, AVX_REG1);
        }
      else
        {
        code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, -AVX_CELLS(1) - 8, asmcode::NUMBER, v3);
        code.add(asmcode::VBROADCASTSS, AVX_REG2, DWORD_MEM_STACK_REGISTER, -AVX_CELLS(1) - 8);
        }
      code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, - AVX_CELLS(1), AVX_REG2);
      code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, - AVX_CELLS(2), AVX_REG1);
      code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, - AVX_CELLS(3), AVX_REG0);
      code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(3));
      code.add(asmcode::COMMENT, "END PUSH THREE FLOATS ON THE STACK");
      break;
      }
      case expanded_token::ET_FLOAT4:
      {
      code.add(asmcode::COMMENT, "BEGIN PUSH FOUR FLOATS ON THE STACK");
      uint32_t v1 = *(reinterpret_cast<uint32_t*>(&word.f[3]));
      uint32_t v2 = *(reinterpret_cast<uint32_t*>(&word.f[2]));
      uint32_t v3 = *(reinterpret_cast<uint32_t*>(&word.f[1]));
      uint32_t v4 = *(reinterpret_cast<uint32_t*>(&word.f[0]));
      code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, -AVX_CELLS(1), asmcode::NUMBER, v1);
      code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, -AVX_CELLS(1) - 4, asmcode::NUMBER, v2);
      code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, -AVX_CELLS(1) - 8, asmcode::NUMBER, v3);
      code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, -AVX_CELLS(1) - 12, asmcode::NUMBER, v4);
      code.add(asmcode::VBROADCASTSS, AVX_REG0, DWORD_MEM_STACK_REGISTER, -AVX_CELLS(1));
      code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER, -AVX_CELLS(1) - 4);
      code.add(asmcode::VBROADCASTSS, AVX_REG2, DWORD_MEM_STACK_REGISTER, -AVX_CELLS(1) - 8);
      code.add(asmcode::VBROADCASTSS, AVX_REG3, DWORD_MEM_STACK_REGISTER, -AVX_CELLS(1) - 12);
      code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, -AVX_CELLS(1), AVX_REG3);
      code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, -AVX_CELLS(2), AVX_REG2);
      code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, -AVX_CELLS(3), AVX_REG1);
      code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, -AVX_CELLS(4), AVX_REG0);
      code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(4));
      code.add(asmcode::COMMENT, "END PUSH FOUR FLOATS ON THE STACK");
      break;
      }
      case expanded_token::ET_FLOAT5:
      {
      code.add(asmcode::COMMENT, "BEGIN PUSH FIVE FLOATS ON THE STACK");
      uint32_t v1 = *(reinterpret_cast<uint32_t*>(&word.f[4]));
      uint32_t v2 = *(reinterpret_cast<uint32_t*>(&word.f[3]));
      uint32_t v3 = *(reinterpret_cast<uint32_t*>(&word.f[2]));
      uint32_t v4 = *(reinterpret_cast<uint32_t*>(&word.f[1]));
      uint32_t v5 = *(reinterpret_cast<uint32_t*>(&word.f[0]));
      code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, -AVX_CELLS(1), asmcode::NUMBER, v1);
      code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, -AVX_CELLS(1) - 4, asmcode::NUMBER, v2);
      code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, -AVX_CELLS(1) - 8, asmcode::NUMBER, v3);
      code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, -AVX_CELLS(1) - 12, asmcode::NUMBER, v4);
      code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, -AVX_CELLS(1) - 16, asmcode::NUMBER, v5);
      code.add(asmcode::VBROADCASTSS, AVX_REG0, DWORD_MEM_STACK_REGISTER, -AVX_CELLS(1));
      code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER, -AVX_CELLS(1) - 4);
      code.add(asmcode::VBROADCASTSS, AVX_REG2, DWORD_MEM_STACK_REGISTER, -AVX_CELLS(1) - 8);
      code.add(asmcode::VBROADCASTSS, AVX_REG3, DWORD_MEM_STACK_REGISTER, -AVX_CELLS(1) - 12);
      code.add(asmcode::VBROADCASTSS, AVX_REG4, DWORD_MEM_STACK_REGISTER, -AVX_CELLS(1) - 16);
      code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, -AVX_CELLS(1), AVX_REG4);
      code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, -AVX_CELLS(2), AVX_REG3);
      code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, -AVX_CELLS(3), AVX_REG2);
      code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, -AVX_CELLS(4), AVX_REG1);
      code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, -AVX_CELLS(5), AVX_REG0);
      code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(5));
      code.add(asmcode::COMMENT, "END PUSH FIVE FLOATS ON THE STACK");
      break;
      }
      case expanded_token::ET_FLOAT6:
      {
      code.add(asmcode::COMMENT, "BEGIN PUSH SIX FLOATS ON THE STACK");
      uint32_t v1 = *(reinterpret_cast<uint32_t*>(&word.f[5]));
      uint32_t v2 = *(reinterpret_cast<uint32_t*>(&word.f[4]));
      uint32_t v3 = *(reinterpret_cast<uint32_t*>(&word.f[3]));
      uint32_t v4 = *(reinterpret_cast<uint32_t*>(&word.f[2]));
      uint32_t v5 = *(reinterpret_cast<uint32_t*>(&word.f[1]));
      uint32_t v6 = *(reinterpret_cast<uint32_t*>(&word.f[0]));
      code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, -AVX_CELLS(1), asmcode::NUMBER, v1);
      code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, -AVX_CELLS(1) - 4, asmcode::NUMBER, v2);
      code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, -AVX_CELLS(1) - 8, asmcode::NUMBER, v3);
      code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, -AVX_CELLS(1) - 12, asmcode::NUMBER, v4);
      code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, -AVX_CELLS(1) - 16, asmcode::NUMBER, v5);
      code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, -AVX_CELLS(1) - 20, asmcode::NUMBER, v6);
      code.add(asmcode::VBROADCASTSS, AVX_REG0, DWORD_MEM_STACK_REGISTER, -AVX_CELLS(1));
      code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER, -AVX_CELLS(1) - 4);
      code.add(asmcode::VBROADCASTSS, AVX_REG2, DWORD_MEM_STACK_REGISTER, -AVX_CELLS(1) - 8);
      code.add(asmcode::VBROADCASTSS, AVX_REG3, DWORD_MEM_STACK_REGISTER, -AVX_CELLS(1) - 12);
      code.add(asmcode::VBROADCASTSS, AVX_REG4, DWORD_MEM_STACK_REGISTER, -AVX_CELLS(1) - 16);
      code.add(asmcode::VBROADCASTSS, AVX_REG5, DWORD_MEM_STACK_REGISTER, -AVX_CELLS(1) - 20);
      code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, -AVX_CELLS(1), AVX_REG5);
      code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, -AVX_CELLS(2), AVX_REG4);
      code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, -AVX_CELLS(3), AVX_REG3);
      code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, -AVX_CELLS(4), AVX_REG2);
      code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, -AVX_CELLS(5), AVX_REG1);
      code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, -AVX_CELLS(6), AVX_REG0);
      code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(6));
      code.add(asmcode::COMMENT, "END PUSH SIX FLOATS ON THE STACK");
      break;
      }
      case expanded_token::ET_VECTOR:
      {
      code.add(asmcode::COMMENT, "BEGIN PUSH FLOAT VECTOR ON THE STACK");
      for (int i = 0; i < AVX_LENGTH/2; ++i)
        {
        uint64_t v1_64, v2_64;
        float f = word.f[i * 2];
        uint32_t v = *(reinterpret_cast<uint32_t*>(&f));
        v1_64 = (uint64_t)v;
        f = word.f[i * 2 + 1];
        v = *(reinterpret_cast<uint32_t*>(&f));
        v2_64 = (uint64_t)v;
        uint64_t vv = (v1_64 << 32) | v2_64;
        code.add(asmcode::MOV, asmcode::RAX, asmcode::NUMBER, vv);
        code.add(asmcode::MOV, MEM_STACK_REGISTER, -CELLS(i + 1), asmcode::RAX);
        }
      code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
      code.add(asmcode::COMMENT, "END PUSH FLOAT VECTOR ON THE STACK");
      break;
      }
      case expanded_token::ET_OVERWRITE_VARIABLE:
      {
      code.add(asmcode::COMMENT, "BEGIN OVERWRITE EXISTING VARIABLE");
      code.add(asmcode::MOV, asmcode::RAX, CONSTANT_SPACE_POINTER);
      code.add(asmcode::ADD, asmcode::RAX, asmcode::NUMBER, word.binding_space_offset);
      code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::MEM_RAX);
      code.add(asmcode::SUB, asmcode::RAX, asmcode::NUMBER, (word.binding_space_offset - word.variable_address));
      code.add(asmcode::VMOVAPS, asmcode::MEM_RAX, AVX_REG0);
      code.add(asmcode::COMMENT, "END OVERWRITE EXISTING VARIABLE");
      break;
      }
      case expanded_token::ET_UPDATE_VARIABLE:
      {
      code.add(asmcode::COMMENT, "BEGIN UPDATE VARIABLE VALUE");
      code.add(asmcode::MOV, asmcode::RAX, CONSTANT_SPACE_POINTER);
      if (word.variable_address)
        code.add(asmcode::ADD, asmcode::RAX, asmcode::NUMBER, word.variable_address);
      code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
      code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
      code.add(asmcode::VMOVAPS, asmcode::MEM_RAX, AVX_REG0);
      code.add(asmcode::COMMENT, "END UPDATE VARIABLE VALUE");
      break;
      }
      case expanded_token::ET_VARIABLE:
      {
      code.add(asmcode::COMMENT, "BEGIN PUSH VARIABLE ADDRESS OR VALUE ON STACK");
      code.add(asmcode::MOV, asmcode::RAX, CONSTANT_SPACE_POINTER);
      if (word.variable_address)
        code.add(asmcode::ADD, asmcode::RAX, asmcode::NUMBER, word.variable_address);
      code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::MEM_RAX);
      code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
      code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
      code.add(asmcode::COMMENT, "END PUSH VARIABLE ADDRESS OR VALUE ON STACK");
      break;
      }
      case expanded_token::ET_CREATE_VARIABLE:
      {
      code.add(asmcode::COMMENT, "BEGIN CREATE VARIABLE");
      code.add(asmcode::MOV, asmcode::RAX, CONSTANT_SPACE_POINTER);
      if (word.binding_space_offset)
        code.add(asmcode::ADD, asmcode::RAX, asmcode::NUMBER, word.binding_space_offset);
      code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
      code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
      code.add(asmcode::VMOVAPS, asmcode::MEM_RAX, AVX_REG0);
      code.add(asmcode::COMMENT, "END CREATE VARIABLE");
      break;
      }      
      } // switch
    }
  }

void compile(ASM::asmcode& code, dictionary& d, expand_data& ed, const std::vector<token>& words)
  {
  compile_data cd = create_compile_data();

  std::vector<expanded_token> expanded;
  expand(expanded, d, ed, words);

  for (int iter = 0; iter < 2; ++iter)
    {
    strength_reduction(expanded);
    constant_folding(expanded);
    }
  superoperators(expanded);

  std::reverse(expanded.begin(), expanded.end());

  code.add(asmcode::GLOBAL, "forth_entry");
#ifdef _WIN32
  /*
  windows parameters calling convention: rcx, rdx, r8, r9
  First parameter (rcx) points to the context.
  We store the pointer to the context in register r10.
  */
  code.add(asmcode::COMMENT, "CONTEXT POINTER (INPUT) IS SAVED IN REGISTER R10");
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
  code.add(asmcode::COMMENT, "STORE REGISTERS AS REQUESTED BY THE CALLING CONVENTIONS");
  store_registers(code);

  code.add(asmcode::COMMENT, "SAVE THE STACK POINTER IN ITS ASSIGNED REGISTER");
  code.add(asmcode::MOV, STACK_REGISTER, STACK_POINTER);

  code.add(asmcode::COMMENT, "SAVE THE HERE POINTER (HEAP) IN ITS ASSIGNED REGISTER");
  code.add(asmcode::MOV, HERE, HERE_POINTER);

  /*
  Align stack with 64 byte boundary
  */
  code.add(asmcode::COMMENT, "ALIGN RSP TO 64 BYTE BOUNDARY");
  code.add(asmcode::AND, asmcode::RSP, asmcode::NUMBER, 0xFFFFFFFFFFFFFFC0);

  code.add(asmcode::COMMENT, "STORE THE RETURN STACK TOP");
  code.add(asmcode::MOV, RSP_TOP, asmcode::RSP);

  code.add(asmcode::COMMENT, "START COMPILATION OF WORDS");
  compile_words(code, cd, expanded);
  code.add(asmcode::COMMENT, "END COMPILATION OF WORDS");

  code.add(asmcode::COMMENT, "SAVE CURRENT STACK POINTER IN CONTEXT");
  code.add(asmcode::MOV, STACK_POINTER, STACK_REGISTER);

  /*Restore the registers to their original state*/
  code.add(asmcode::COMMENT, "RESTORE THE REGISTERS AS REQUESTED BY THE CALLING CONVENTIONS");
  load_registers(code);

  code.add(asmcode::COMMENT, "RETURN TO CALLER");
  /*Return to the caller*/
  code.add(asmcode::RET);
  }

VF_END