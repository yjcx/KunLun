#ifndef YJCX_KUNLUN_BASIC_TESTREGISTRY_H
#define YJCX_KUNLUN_BASIC_TESTREGISTRY_H

#include <memory>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "TestSuite.h"

namespace yjcx::kunlun::base::test::basic
{

class TestRegistry
{
public:
    static TestRegistry& instance();

    TestRegistry(const TestRegistry&) = delete;
    TestRegistry& operator=(const TestRegistry&) = delete;
    TestRegistry(TestRegistry&&) = delete;
    TestRegistry& operator=(TestRegistry&&) = delete;

    void registerSuite(
        std::unique_ptr<TestSuite> suite,
        std::source_location location = std::source_location::current());
    const std::vector<std::unique_ptr<TestSuite>>& suites() const noexcept;

    size_t suiteCount() const noexcept;
    size_t testCount() const noexcept;
    TestSuite* getSuite(std::string_view suite_name) const;

    std::vector<TestSuite*> findSuites(std::string_view pattern) const;
    std::vector<TestCase*> findTestByTag(std::string_view tag) const;
    std::vector<TestCase*> findTestByName(std::string_view pattern) const;

private:
    TestRegistry() = default;
    std::vector<std::unique_ptr<TestSuite>> suites_;
    std::unordered_map<std::string_view, TestSuite*> suite_map_;

    void checkSuiteValidity(TestSuite* suite);
};

}  // namespace yjcx::kunlun::base::test::basic

#endif
