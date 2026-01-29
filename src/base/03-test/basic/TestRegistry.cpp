#include "TestRegistry.h"

#include <format>
#include <vector>

#include "LocationUtil.h"

using std::format;
using std::invalid_argument;
using std::string;
using std::string_view;
using std::unique_ptr;
using std ::vector;
using yjcx::kunlun::base::utils::location::basicFormatLocation;

namespace yjcx::kunlun::base::test::basic
{

TestRegistry& TestRegistry::instance()
{
    static TestRegistry singleton;
    return singleton;
}

void TestRegistry::registerSuite(unique_ptr<TestSuite> suite,
                                 std::source_location location)
{
    if (!suite)
    {
        string message = format(
            "TestSuite can't be null\n"
            "At: {}",
            basicFormatLocation(location));
        throw invalid_argument(message);
    }

    checkSuiteValidity(suite.get());

    suite_map_[suite->name()] = suite.get();
    suites_.push_back(std::move(suite));
}
const vector<unique_ptr<TestSuite>>& TestRegistry::suites() const noexcept
{
    return suites_;
}

size_t TestRegistry::suiteCount() const noexcept { return suites_.size(); }
size_t TestRegistry::testCount() const noexcept
{
    size_t total = 0;
    for (const auto& suite : suites_)
    {
        total += suite->testCount();
    }
    return total;
}
TestSuite* TestRegistry::getSuite(string_view suite_name) const
{
    auto it = suite_map_.find(suite_name);
    if (it != suite_map_.end())
    {
        return it->second;
    }
    return nullptr;
}

vector<TestSuite*> TestRegistry::findSuites(string_view pattern) const
{
    vector<TestSuite*> result;
    result.reserve(suites_.size());

    const bool match_all = pattern.empty();
    for (const auto& suite : suites_)
    {
        string_view name = suite->name();
        if (match_all || name.find(pattern) != string_view::npos)
        {
            result.push_back(suite.get());
        }
    }
    return result;
}
vector<TestCase*> TestRegistry::findTestByTag(string_view tag) const
{
    vector<TestCase*> result;
    for (const auto& suite : suites_)
    {
        auto tests = suite->testsByCategory(tag);
        result.insert(result.end(), tests.begin(), tests.end());
    }
    return result;
}
vector<TestCase*> TestRegistry::findTestByName(string_view pattern) const
{
    vector<TestCase*> result;

    bool match_all = pattern.empty();
    for (const auto& suite : suites_)
    {
        for (const auto& test : suite->tests())
        {
            string_view name = test->name();
            if (match_all || name.find(pattern) != string_view::npos)
            {
                result.push_back(test);
            }
        }
    }
    return result;
}

void TestRegistry::checkSuiteValidity(TestSuite* suite)
{
    auto it = suite_map_.find(suite->name());
    if (it != suite_map_.end())
    {
        string error = format(
            "Duplicate suite name '{}'\n"
            "First define at: {}\n"
            "Duplicate    at: {}",
            suite->name(), basicFormatLocation(it->second->location()),
            basicFormatLocation(suite->location()));
        throw std::invalid_argument(error);
    }
}

}  // namespace yjcx::kunlun::base::test::basic
