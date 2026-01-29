#ifndef YJCX_KUNLUN_ASSERTION_ASSERTION_H
#define YJCX_KUNLUN_ASSERTION_ASSERTION_H

#include <format>
#include <source_location>
#include <string_view>

#include "AssertionContext.h"
#include "LocationUtil.h"

namespace yjcx::kunlun::base::test::assertion
{

template <typename T, typename U>
void assertEqual(
    const T& expected, const U& actual, std::string_view assertion = {},
    std::source_location location = std::source_location::current())
{
    bool passed = (expected == actual);
    if (!passed)
    {
        std::string error = std::format(
            "type      : assert equal\n"
            "Assertion : {}\n"
            "Expected  : {}\n"
            "Actual    : {}\n"
            "Location  : {}\n",
            assertion, expected, actual,
            utils::location::basicFormatLocation(location));
        AssertionContext::record(false, error);
    }
    else
    {
        AssertionContext::record(true, assertion);
    }
}

template <typename T, typename U>
void assertNotEqual(
    const T& expected, const U& actual, std::string_view assertion = {},
    std::source_location location = std::source_location::current())
{
    bool passed = !(expected == actual);
    if (!passed)
    {
        std::string error = std::format(
            "type      : assert not equal\n"
            "Assertion : {}\n"
            "Expected  : {}\n"
            "Actual    : {}\n"
            "Location  : {}\n",
            assertion, expected, actual,
            utils::location::basicFormatLocation(location));
        AssertionContext::record(false, error);
    }
    else
    {
        AssertionContext::record(true, assertion);
    }
}

template <typename T>
void assertTrue(T&& condition, std::string_view assertion = {},
                std::source_location location = std::source_location::current())
{
    bool passed = static_cast<bool>(condition);
    if (!passed)
    {
        std::string error = std::format(
            "type      : assert true\n"
            "Assertion : {}\n"
            "Expected  : true\n"
            "Actual    : {}\n"
            "Location  : {}\n",
            assertion, condition,
            utils::location::basicFormatLocation(location));
        AssertionContext::record(false, error);
    }
    else
    {
        AssertionContext::record(true, assertion);
    }
}

template <typename T>
void assertFalse(
    T&& condition, std::string_view assertion = {},
    std::source_location location = std::source_location::current())
{
    bool passed = !static_cast<bool>(condition);
    if (!passed)
    {
        std::string error = std::format(
            "type      : assert false\n"
            "Assertion : {}\n"
            "Expected  : false\n"
            "Actual    : {}\n"
            "Location  : {}\n",
            assertion, condition,
            utils::location::basicFormatLocation(location));
        AssertionContext::record(false, error);
    }
    else
    {
        AssertionContext::record(true, assertion);
    }
}

template <typename T, typename U>
void assertNear(const T& expected, const U& actual, double epsilon = 1e-6,
                std::string_view assertion = {},
                std::source_location location = std::source_location::current())
{
    bool passed = std::abs(expected - actual) <= epsilon;
    if (!passed)
    {
        std::string error = std::format(
            "type      : assert near\n"
            "Assertion : {}\n"
            "Expected  : {}\n"
            "Actual    : {}\n"
            "Difference: {}\n"
            "Epsilon   : {}\n"
            "Location  : {}\n",
            assertion, expected, actual, std::abs(expected - actual), epsilon,
            utils::location::basicFormatLocation(location));
        AssertionContext::record(false, error);
    }
    else
    {
        AssertionContext::record(true, assertion);
    }
}

}  // namespace yjcx::kunlun::base::test::assertion

#endif
