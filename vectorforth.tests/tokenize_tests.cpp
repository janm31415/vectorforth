#include "tokenize_tests.h"
#include "test_assert.h"

#include <vectorforth/tokenize.h>

VF_BEGIN

namespace
  {
  bool equal(const token& t, const std::string& value, const token::e_type& type)
    {
    if (t.type != type)
      return false;
    if (t.value != value)
      return false;
    return true;
    }
  }

void test_tokenize()
  {
#ifdef AVX512
  auto words = tokenize("double 1 3.4 : ; ( bla bla \n\t    bla\n) tureluut 1.e-8 4509.3498.234234 v16 \\ this is comment");
#else
  auto words = tokenize("double 1 3.4 : ; ( bla bla \n\t    bla\n) tureluut 1.e-8 4509.3498.234234 v8 \\ this is comment");
#endif
  TEST_EQ(9, (int)words.size());

  TEST_ASSERT(equal(words[0], "double", token::T_WORD));
  TEST_ASSERT(equal(words[1], "1", token::T_FLOAT));
  TEST_ASSERT(equal(words[2], "3.4", token::T_FLOAT));
  TEST_ASSERT(equal(words[3], ":", token::T_COLON));
  TEST_ASSERT(equal(words[4], ";", token::T_SEMICOLON));
  TEST_ASSERT(equal(words[5], "tureluut", token::T_WORD));
  TEST_ASSERT(equal(words[6], "1.e-8", token::T_FLOAT));
  TEST_ASSERT(equal(words[7], "4509.3498.234234", token::T_WORD));
#ifdef AVX512
  TEST_ASSERT(equal(words[8], "v16", token::T_VECTOR16));
#else
  TEST_ASSERT(equal(words[8], "v8", token::T_VECTOR8));
#endif

  TEST_EQ(1.f, to_float(words[1].value.c_str()));
  TEST_EQ(3.4f, to_float(words[2].value.c_str()));
  TEST_EQ(1.e-8f, to_float(words[6].value.c_str()));
  }

VF_END

void run_all_tokenize_tests()
  {
  using namespace VF;
  test_tokenize();
  }