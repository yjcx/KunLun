#ifndef YJCX_KUNLUN_AUTOMATE_TESTRESULT_H
#define YJCX_KUNLUN_AUTOMATE_TESTRESULT_H

#include <atomic>
#include <cstddef>

namespace yjcx::kunlun::base::test::automate
{

struct TestResult
{
    std::atomic<size_t> total_suites;
    std::atomic<size_t> total_tests;
    std::atomic<size_t> passed_tests;
    std::atomic<size_t> failed_tests;
    std::atomic<size_t> total_assertions;
    std::atomic<size_t> passed_assertions;
    std::atomic<size_t> failed_assertions;

    TestResult();
    bool isSuccessful() const noexcept;
    bool hasFailures() const noexcept;
    double getPassRate() const noexcept;
};

}  // namespace yjcx::kunlun::base::test::automate

#endif
