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

    std::string script = R"(
: u st@ #160 #- @ ;
: v st@ #192 #- @ ;
: t st@ #224 #- @ ;
0.5 t u + cos +
0.5 t v 2 + + cos +
0.5 t u 4 + + cos +
)";

    std::string script2 = R"(
: u st@ #160 #- @ ;
: v st@ #192 #- @ ;
: t st@ #224 #- @ ;
: x u ;
: y v ;

( Make harmonique )
: harm ( a b c d x - a+b*sin[x*d+c] ) * + sin * + ;

( Fourier Series )
0
1 3 t 2 / + 8 t x + harm
.7 2 t 3 * + 3 t 4 / x + harm
.5 3 t 7 * + 17 t x + harm
.7 6 t 11 * + 13 t 13 / x + harm
5 / .5 + y - abs 0.01 - 1 - negate abs dup 4 ** swap 20 **
)";

    std::string script3 = R"(
: u st@ #160 #- @ ;
: v st@ #192 #- @ ;
: rx st@ #96 #- @ ;
: ry st@ #128 #- @ ;
: t st@ #224 #- @ ;
: x u rx ry / * rx ry / 1 - 2 / -;
: y v ;

: d dup ;
: m 0.4 * 0.5 + - ;
: a y - abs 0.1 f< x 0 f> * x 1 f< * x;
: r t swap / d floor - 1.96 * 0.98 - d 0 f> 2 * 1 - * 0.01 + ;
: e 2.9 r 0.94 * 0.03 + ;
: f 3.7 r ;
: p f 0.9 * 0.05 + d t ;
e x - d * f y - d * + sqrt 0.02 f<
p 3 * sin m e * - a 0.03 f< * 
p 2 * cos m 1 e - * - a 0.97 f> * 
+ + d d
)";
    //print_asm(std::cout, script3);
    }
  };

VF_END

void run_all_optimize_tests()
  {
  using namespace VF;
  optimize_1().test();
  }