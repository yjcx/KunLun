#ifndef YJCX_KUNLUN_BASIC_TESTCASE_H
#define YJCX_KUNLUN_BASIC_TESTCASE_H

#include <functional>
#include <source_location>
#include <string>

namespace yjcx::kunlun::base::test::basic
{

class TestCase
{
public:
    TestCase(std::string_view name, std::string_view category,
             std::function<void()> function,
             std::source_location location = std::source_location::current());
    virtual ~TestCase() noexcept = default;

    TestCase(const TestCase&) = delete;
    TestCase& operator=(const TestCase&) = delete;
    TestCase(TestCase&&) = delete;
    TestCase& operator=(TestCase&&) = delete;

    void run();
    void markFailed(const std::string_view& message);
    void recordAssertion(bool passed) noexcept;

    const std::string& name() const noexcept;
    const std::string& category() const noexcept;
    const std::source_location& location() const noexcept;
    const std::vector<std::string>& errorMessages() const noexcept;
    bool passed() const noexcept;
    size_t totalAssertionCount() const noexcept;
    size_t passedAssertionCount() const noexcept;
    size_t failedAssertionCount() const noexcept;

private:
    std::string name_;
    std::string category_;
    std::function<void()> function_;
    std::source_location location_;
    std::vector<std::string> error_messages_;
    bool passed_;
    size_t total_assertion_count_;
    size_t passed_assertion_count_;
};

}  // namespace yjcx::kunlun::base::test::basic

#endif
