#pragma once

#include <sstream>
#include <string>

#define TEST_OUTPUT_LINE(...) \
      { printf(__VA_ARGS__); printf("\n"); }

#define TEST_EQ_STR(exp, val) TestEqStr(exp, val, __FILE__, __LINE__)

#ifdef _WIN32
#define TEST_ASSERT(val) TestEq(true, val, __FILE__, __LINE__, __FUNCTION__)
#define TEST_EQ(exp, val) TestEq(exp, val, __FILE__, __LINE__, __FUNCTION__)
#define TEST_EQ_CLOSE(exp, val, tol) TestEqClose(exp, val, tol, __FILE__, __LINE__, __FUNCTION__)
#else
#define TEST_ASSERT(val) TestEq(true, val, __FILE__, __LINE__, __PRETTY_FUNCTION__)
#define TEST_EQ(exp, val) TestEq(exp, val, __FILE__, __LINE__, __PRETTY_FUNCTION__)
#define TEST_EQ_CLOSE(exp, val, tol) TestEqClose(exp, val, tol, __FILE__, __LINE__, __PRETTY_FUNCTION__)
#endif

extern int testing_fails;
extern int testing_success;

// Listener of TestFail, like 'gtest::OnTestPartResult' event handler.
// Called in TestFail after a failed assertion.
typedef bool(*TestFailEventListener)(const char *expval, const char *val,
  const char *file, int line, const char *func);

// Prepare test engine (MSVC assertion setup, etc).
// listener - this function will be notified on each TestFail call.
void InitTestEngine(TestFailEventListener listener = nullptr);

// Release all test-engine resources.
// Prints or schedule a debug report if all test passed.
// Returns 0 if all tests passed or 1 otherwise.
// Memory leak report: MEMORY_LEAK_TRACKING && _MSC_VER && !NDEBUG.
int CloseTestEngine(bool force_report = false);

// Write captured state to a log and terminate test run.
void TestFail(const char *expval, const char *val, const char *file, int line, const char *func = 0);

void TestEqStr(const char *expval, const char *val, const char *file, int line);

template<typename T, typename U>
void TestEq(T expval, U val, const char *file, int line, const char *func = 0)
  {
  if (U(expval) != val)
    {
    std::stringstream ss1;
    ss1 << expval;
    std::stringstream ss2;
    ss2 << val;
    TestFail(ss1.str().c_str(), ss2.str().c_str(), file, line, func);
    }
  else
    ++testing_success;
  }

template<typename T, typename U, typename V>
void TestEqClose(T expval, U val, V tol, const char *file, int line, const char *func = 0)
  {
  if (std::abs(V(expval) - V(val)) > tol)
    {
    std::stringstream ss1;
    ss1 << expval;
    std::stringstream ss2;
    ss2 << val;
    TestFail(ss1.str().c_str(), ss2.str().c_str(), file, line, func);
    }
  else
    ++testing_success;
  }