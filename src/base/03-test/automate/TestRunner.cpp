#include "TestRunner.h"

#include <format>
#include <ostream>
#include <string>
#include <string_view>
#include <thread>
#include <vector>

#include "TestRegistry.h"

using std::format;
using std::lock_guard;
using std::mutex;
using std::ostream;
using std::string;
using std::string_view;
using std::thread;
using std::vector;

namespace yjcx::kunlun::base::test::automate
{

using basic::TestRegistry;

TestRunner::TestRunner(ostream& output, bool verbose)
    : output_(&output), verbose_(verbose), mutex_out_()
{
}

void TestRunner::setOutput(ostream& stream) noexcept { output_ = &stream; }

void TestRunner::executeAll()
{
    auto& registry = TestRegistry::instance();
    const auto& suites = registry.suites();

    result_.total_suites = suites.size();

    for (const auto& suite : suites)
    {
        displaySuiteHeader(*suite);
        executeParallel(*suite);
    }

    reportSummary();
}
void TestRunner::executeSuite(std::string_view suite_name)
{
    auto& registry = TestRegistry::instance();
    TestSuite* suite = registry.getSuite(suite_name);

    if (!suite)
    {
        if (verbose_)
        {
            string message = format("TestSuite '{}' not found\n", suite_name);
            *output_ << message;
        }
        return;
    }

    displaySuiteHeader(*suite);
    executeParallel(*suite);
}
void TestRunner::executeSuites(const std::vector<std::string_view>& suite_names)
{
    for (const auto& suite_name : suite_names)
    {
        executeSuite(suite_name);
    }
}

void TestRunner::executeParallel(TestCase& test)
{
    test.run();
    size_t total_assertions = test.totalAssertionCount();
    size_t passed_assertions = test.passedAssertionCount();
    size_t failed_assertions = test.failedAssertionCount();

    {
        lock_guard<mutex> lock(mutex_statistics_);
        ++result_.total_tests;
        result_.total_assertions += total_assertions;
        result_.passed_assertions += passed_assertions;
        result_.failed_assertions += failed_assertions;
        if (test.passed())
        {
            ++result_.passed_tests;
        }
        else
        {
            ++result_.failed_tests;
        }
    }
    displayTestResult(test, test.passed());
}
void TestRunner::executeParallel(TestSuite& suite)
{
    auto tests = suite.tests();
    vector<thread> threads;
    for (auto* test : tests)
    {
        threads.emplace_back([this, test]() { executeParallel(*test); });
    }
    for (auto& thread : threads)
    {
        thread.join();
    }
}
void TestRunner::reportSummary() const
{
    displaySeparator();
    *output_ << "Test Summary\n";
    displaySeparator();
    string message = format(
        "Total Suites: {}\n"
        "Total Tests: {}\n"
        "Passed Tests: {}\n"
        "Failed Tests: {}\n"
        "Total Assertions: {}\n"
        "Passed Assertions: {}\n"
        "Failed Assertions: {}\n"
        "Pass Rate: {}%\n",
        result_.total_suites.load(), result_.total_tests.load(),
        result_.passed_tests.load(), result_.failed_tests.load(),
        result_.total_assertions.load(), result_.passed_assertions.load(),
        result_.failed_assertions.load(), result_.getPassRate() * 100);
    *output_ << message;
    displaySeparator();

    if (result_.isSuccessful())
    {
        *output_ << "SUCCESS: All tests passed!\n";
    }
    else
    {
        *output_ << "FAILURE: Some tests failed.\n";
    }
    displaySeparator();
}
void TestRunner::displaySuiteHeader(const TestSuite& suite) const
{
    if (verbose_)
    {
        displaySeparator();
        string message = format("Test Suite: {} ({} tests)\n", suite.name(),
                                suite.testCount());
        *output_ << message;
        displaySeparator();
    }
}
void TestRunner::displayTestResult(const TestCase& test, bool passed)
{
    if (verbose_)
    {
        lock_guard<mutex> lock(mutex_out_);
        if (passed)
        {
            *output_ << "[PASS] " << test.name() << "\n";
        }
        else
        {
            *output_ << "[FAIL] " << test.name() << "\n";
            for (const auto& error : test.errorMessages())
            {
                *output_ << error << "\n";
            }
            displaySeparator('-');
        }
    }
}
void TestRunner::displaySeparator(char c) const
{
    *output_ << string(60, c) << "\n";
}

}  // namespace yjcx::kunlun::base::test::automate
