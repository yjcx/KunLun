#include "TestSuite.h"

#include <format>
#include <stdexcept>

#include "LocationUtil.h"

using std::format;
using std::invalid_argument;
using std::source_location;
using std::string;
using std::string_view;
using std::unique_ptr;
using std::vector;
using yjcx::kunlun::base::utils::location::basicFormatLocation;

namespace yjcx::kunlun::base::test::basic
{

TestSuite::TestSuite(string_view name, source_location location)
    : name_(name), location_(location)
{
    if (name.empty())
    {
        string message = format(
            "The name of TestSuite can't be empty\n"
            "At: {}",
            basicFormatLocation(location_));
        throw invalid_argument(message);
    }
}

void TestSuite::addTest(unique_ptr<TestCase> test)
{
    if (!test)
    {
        throw invalid_argument("Test can't be null");
    }

    checkTestValidity(test.get());

    name_map_[test->name()] = test.get();
    category_map_[test->category()].push_back(test.get());
    tests_.push_back(std::move(test));
}
vector<TestCase*> TestSuite::tests() const
{
    vector<TestCase*> result;
    for (auto& test : tests_)
    {
        result.push_back(test.get());
    }
    return result;
}
vector<TestCase*> TestSuite::testsByCategory(std::string_view category) const
{
    auto it = category_map_.find(category);
    if (it != category_map_.end())
    {
        return it->second;
    }
    return {};
}
vector<TestCase*> TestSuite::testsByName(string_view name) const
{
    auto it = name_map_.find(name);
    if (it != name_map_.end())
    {
        return {it->second};
    }
    return {};
}

const string& TestSuite::name() const noexcept { return name_; }
source_location TestSuite::location() const { return location_; }
size_t TestSuite::testCount() const noexcept { return tests_.size(); }

void TestSuite::checkTestValidity(TestCase* test)
{
    auto it = name_map_.find(test->name());
    if (it != name_map_.end())
    {
        string error = format(
            "Duplicate test name '{}' in suite '{}'\n"
            "First define at: {}\n"
            "Duplicate    at: {}",
            test->name(), name(), basicFormatLocation(it->second->location()),
            basicFormatLocation(test->location()));
        throw std::invalid_argument(error);
    }
}

}  // namespace yjcx::kunlun::base::test::basic
