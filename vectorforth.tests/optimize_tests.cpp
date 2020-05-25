#include "optimize_tests.h"
#include "test_assert.h"


#include <vectorforth/context.h>
#include <vectorforth/compiler.h>
#include <vectorforth/debug.h>
#include <vectorforth/dictionary.h>
#include <vectorforth/expand.h>
#include <vectorforth/expand_data.h>
#include <vectorforth/stdlib.h>
#include <vectorforth/tokenize.h>
#include <vectorforth/constant_folding.h>
#include <vectorforth/strength_reduction.h>

VF_BEGIN

struct compile_fixture
  {
  context ctxt;
  std::string last_error;
  dictionary dict;
  expand_data ed;
  
  compile_fixture()
    {
    ctxt = create_context(1024 * 1024, 1024, 1024 * 1024);
    add_stdlib_to_dictionary(dict);
    ed = create_expand_data();
    }

  ~compile_fixture()
    {
    destroy_context(ctxt);
    }

  std::vector<expanded_token> run(const std::string& script)
    {
    auto words = tokenize(script);
    std::vector<expanded_token> expanded;
    expand(expanded, dict, ed, words);

    for (int iter = 0; iter < 2; ++iter)
      {
      strength_reduction(expanded);
      constant_folding(expanded);
      }

    return expanded;
    }

  void print_asm(std::ostream& os, const std::string& script)
    {
    ASM::asmcode code;
    auto words = tokenize(script);
    compile(code, dict, ed, words);
    code.stream(os);
    }
  
  };

struct optimize_1 : public compile_fixture
  {
  void test()
    {
    auto res = run("3 4 > if 2 > 1 if 10 else 11 then else 7 8 < if 2 else 20 then then");
    TEST_EQ((int)1, (int)res.size());
    TEST_ASSERT(res.front().t == expanded_token::ET_FLOAT);
    TEST_EQ(2.f, res.front().f[0]);

    res = run("1 2 3 4 5 6 7 8 9 10 + + + + + + + + +");
    TEST_EQ((int)1, (int)res.size());
    TEST_ASSERT(res.front().t == expanded_token::ET_FLOAT);
    TEST_EQ(55.f, res.front().f[0]);

    print_asm(std::cout, "1");
    }
  };

VF_END

void run_all_optimize_tests()
  {
  using namespace VF;
  optimize_1().test();
  }