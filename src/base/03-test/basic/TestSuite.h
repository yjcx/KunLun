#ifndef YJCX_KUNLUN_BASIC_TESTSUITE_H
#define YJCX_KUNLUN_BASIC_TESTSUITE_H

#include <memory>
#include <source_location>
#include <string>
#include <unordered_map>
#include <vector>

#include "TestCase.h"

namespace yjcx::kunlun::base::test::basic
{

class TestSuite
{
public:
    explicit TestSuite(
        std::string_view name,
        std::source_location location = std::source_location::current());
    virtual ~TestSuite() noexcept = default;

    TestSuite(const TestSuite&) = delete;
    TestSuite& operator=(const TestSuite&) = delete;
    TestSuite(TestSuite&&) = delete;
    TestSuite& operator=(TestSuite&&) = delete;

    void addTest(std::unique_ptr<TestCase> test);
    std::vector<TestCase*> tests() const;
    std::vector<TestCase*> testsByCategory(std::string_view category) const;
    std::vector<TestCase*> testsByName(std::string_view name) const;

    const std::string& name() const noexcept;
    std::source_location location() const;
    size_t testCount() const noexcept;

    virtual void suiteSetup() {}
    virtual void suiteTeardown() {}

private:
    std::string name_;
    std::source_location location_;
    std::vector<std::unique_ptr<TestCase>> tests_;
    std::unordered_map<std::string_view, std::vector<TestCase*>> category_map_;
    std::unordered_map<std::string_view, TestCase*> name_map_;

    void checkTestValidity(TestCase* test);
};

}  // namespace yjcx::kunlun::base::test::basic

#endif
