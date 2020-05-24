#include "debug_tests.h"
#include "test_assert.h"

#include <vectorforth/context.h>
#include <vectorforth/compiler.h>
#include <vectorforth/debug.h>
#include <vectorforth/dictionary.h>
#include <vectorforth/expand.h>
#include <vectorforth/expand_data.h>
#include <vectorforth/stdlib.h>
#include <vectorforth/tokenize.h>

#include <sstream>


using namespace ASM;

VF_BEGIN

namespace
  {
  struct compile_fixture
    {
    context ctxt;
    typedef void(*fun_ptr)(void*);
    std::vector<std::pair<fun_ptr, uint64_t>> compiled_functions;
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
      for (auto& f : compiled_functions)
        free_assembled_function((void*)f.first, f.second);
      destroy_context(ctxt);
      }

    asmcode get_asmcode(const std::string& script)
      {
      asmcode code;
      auto words = tokenize(script);
      compile(code, dict, ed, words);
      return code;
      }

    char* run(const std::string& script)
      {
      asmcode code;
      try
        {
        code = get_asmcode(script);
        }
      catch (std::runtime_error e)
        {
        last_error = e.what();
        std::cout << last_error << std::endl;
        return nullptr;
        }
      first_pass_data d;
      uint64_t size;
      fun_ptr f = (fun_ptr)assemble(size, d, code);
      f(&ctxt);
      compiled_functions.emplace_back(f, size);
      return ctxt.stack_pointer;
      }

    };
  } // namespace

struct print_stack_test : public compile_fixture
  {
  void test()
    {
    std::stringstream ss1, ss2;
    run("1 2 3");
    print_stack(ss1, ctxt);
#ifdef AVX512
    ss2 << "stack dump\n";
    ss2 << "----------\n";
    ss2 << "[0] 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 \n";
    ss2 << "[1] 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 \n";
    ss2 << "[2] 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 \n\n";
#else
    ss2 << "stack dump\n";
    ss2 << "----------\n";
    ss2 << "[0] 1 1 1 1 1 1 1 1 \n";
    ss2 << "[1] 2 2 2 2 2 2 2 2 \n";
    ss2 << "[2] 3 3 3 3 3 3 3 3 \n\n";
#endif
    TEST_ASSERT(ss1.str() == ss2.str());
    }
  };

struct print_data_space_test : public compile_fixture
  {
  void test()
    {
    std::stringstream ss1, ss2;
    run("vec3 v 1 2 3 v vec3!");
    print_data_space(ss1, ctxt);
#ifdef AVX512
    ss2 << "data space dump\n";
    ss2 << "---------------\n";
    ss2 << "[0] 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 \n";
    ss2 << "[1] 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 \n";
    ss2 << "[2] 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 \n\n";
#else
    ss2 << "data space dump\n";
    ss2 << "---------------\n";
    ss2 << "[0] 1 1 1 1 1 1 1 1 \n";
    ss2 << "[1] 2 2 2 2 2 2 2 2 \n";
    ss2 << "[2] 3 3 3 3 3 3 3 3 \n\n";
#endif
    TEST_ASSERT(ss1.str() == ss2.str());
    }
  };

struct print_bytecode_test : public compile_fixture
  {
  void test()
    {
    std::string script = "vec3 v 1 2 3 v vec3!";
    auto words = tokenize(script);
    std::vector<expanded_token> expanded;
    expand(expanded, dict, ed, words);
    }
  };

VF_END

void run_all_debug_tests()
  {
  using namespace VF;
  print_stack_test().test();
  print_data_space_test().test();
  //print_bytecode_test().test();
  }