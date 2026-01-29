#include "TestResult.h"

namespace yjcx::kunlun::base::test::automate
{

TestResult::TestResult()
    : total_suites(0),
      total_tests(0),
      passed_tests(0),
      failed_tests(0),
      total_assertions(0),
      passed_assertions(0),
      failed_assertions(0)
{
}
bool TestResult::isSuccessful() const noexcept
{
    return failed_tests.load() == 0;
}
bool TestResult::hasFailures() const noexcept
{
    return failed_tests.load() > 0;
}
double TestResult::getPassRate() const noexcept
{
    return total_tests.load() > 0
               ? static_cast<double>(passed_tests.load()) / total_tests.load()
               : 0.0;
}

}  // namespace yjcx::kunlun::base::test::automate
