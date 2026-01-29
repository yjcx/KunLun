#include "AssertionContext.h"

#include <format>
#include <stdexcept>
#include <string>

namespace yjcx::kunlun::base::test::assertion
{

using basic::TestCase;
using std::format;
using std::runtime_error;
using std::string;
using std::string_view;

thread_local TestCase* AssertionContext::pCase_ = nullptr;

void AssertionContext::setCase(const TestCase* pCase)
{
    pCase_ = const_cast<TestCase*>(pCase);
}
TestCase* AssertionContext::getCase() { return pCase_; }
void AssertionContext::record(bool passed, string_view message)
{
    if (!pCase_)
    {
        string error = format("Assertion outside of test case: {}", message);
        throw runtime_error(error);
    }

    pCase_->recordAssertion(passed);
    if (!passed)
    {
        pCase_->markFailed(message.data());
    }
}

}  // namespace yjcx::kunlun::base::test::assertion
