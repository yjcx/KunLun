#ifndef YJCX_KUNLUN_AUTOMATE_TESTRUNNER_H
#define YJCX_KUNLUN_AUTOMATE_TESTRUNNER_H

#include <iostream>
#include <mutex>
#include <vector>

#include "TestCase.h"
#include "TestResult.h"
#include "TestSuite.h"

namespace yjcx::kunlun::base::test::automate
{

using basic::TestCase;
using basic::TestSuite;

class TestRunner
{
public:
    TestRunner(std::ostream& output = std::cout, bool verbose = false);

    void setOutput(std::ostream& stream) noexcept;

    void executeAll();
    void executeSuite(std::string_view suite_name);
    void executeSuites(const std::vector<std::string_view>& suite_names);

private:
    std::ostream* output_;
    bool verbose_;
    std::mutex mutex_out_;
    std::mutex mutex_statistics_;
    TestResult result_;

    void executeParallel(TestCase& test);
    void executeParallel(TestSuite& suite);
    void reportSummary() const;
    void displaySuiteHeader(const TestSuite& suite) const;
    void displayTestResult(const TestCase& test, bool passed);
    void displaySeparator(char c = '=') const;
};

}  // namespace yjcx::kunlun::base::test::automate

#endif
