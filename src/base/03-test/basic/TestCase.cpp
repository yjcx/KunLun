#include "TestCase.h"

#include <exception>
#include <format>

#include "AssertionContext.h"
#include "LocationUtil.h"

using std::exception;
using std::format;
using std::function;
using std::invalid_argument;
using std::source_location;
using std::string;
using std::string_view;
using std::vector;
using yjcx::kunlun::base::utils::location::basicFormatLocation;

namespace yjcx::kunlun::base::test::basic
{

TestCase::TestCase(string_view name, string_view category,
                   function<void()> function, source_location location)
    : name_(name),
      category_(category),
      function_(function),
      location_(location),
      error_messages_(),
      passed_(true),
      total_assertion_count_(0),
      passed_assertion_count_(0)
{
    if (name.empty())
    {
        string error = format(
            "The name of TestCase can't be empty\n"
            "At: {}",
            basicFormatLocation(location_));
        throw invalid_argument(error);
    }
    if (!function_)
    {
        string error = format(
            "The function of TestCase can't be null\n"
            "At: {}",
            basicFormatLocation(location_));
        throw invalid_argument(error);
    }
}

void TestCase::run()
{
    assertion::AssertionContext::setCase(this);
    try
    {
        function_();
    }
    catch (const exception& e)
    {
        markFailed(e.what());
    }
    catch (...)
    {
        markFailed("Unknown exception");
    }
    assertion::AssertionContext::setCase(nullptr);
}
void TestCase::markFailed(const string_view& message)
{
    passed_ = false;
    error_messages_.push_back(message.data());
}
void TestCase::recordAssertion(bool passed) noexcept
{
    ++total_assertion_count_;
    if (passed)
    {
        ++passed_assertion_count_;
    }
}

const string& TestCase::name() const noexcept { return name_; }
const string& TestCase::category() const noexcept { return category_; }
const source_location& TestCase::location() const noexcept { return location_; }
const vector<string>& TestCase::errorMessages() const noexcept
{
    return error_messages_;
}
bool TestCase::passed() const noexcept { return passed_; }
size_t TestCase::totalAssertionCount() const noexcept
{
    return total_assertion_count_;
}
size_t TestCase::passedAssertionCount() const noexcept
{
    return passed_assertion_count_;
}
size_t TestCase::failedAssertionCount() const noexcept
{
    return total_assertion_count_ - passed_assertion_count_;
}

}  // namespace yjcx::kunlun::base::test::basic
