#ifndef YJCX_KUNLUN_ADVANCED_TESTMACROS_H
#define YJCX_KUNLUN_ADVANCED_TESTMACROS_H

#include "Assertion.h"
#include "TestCase.h"
#include "TestRegistry.h"
#include "TestSuite.h"

using yjcx::kunlun::base::test::basic::TestCase;
using yjcx::kunlun::base::test::basic::TestRegistry;
using yjcx::kunlun::base::test::basic::TestSuite;

namespace yjcx::kunlun::base::test::automate
{

#define TEST_SUITE(suite_name)                                          \
    static inline TestSuite& getCurrentSuiteRef()                       \
    {                                                                   \
        static TestSuite* pInstance = []() -> TestSuite*                \
        {                                                               \
            auto* pSuite = new TestSuite(#suite_name);                  \
            auto& registry = TestRegistry::instance();                  \
            registry.registerSuite(std::unique_ptr<TestSuite>(pSuite)); \
            return pSuite;                                              \
        }();                                                            \
        return *pInstance;                                              \
    }

#define TEST_CASE(test_name, category, ...)                             \
    namespace                                                           \
    {                                                                   \
    struct test_name##_registrar_t                                      \
    {                                                                   \
        test_name##_registrar_t()                                       \
        {                                                               \
            getCurrentSuiteRef().addTest(std::unique_ptr<TestCase>(     \
                new TestCase(#test_name, category, []() __VA_ARGS__))); \
        }                                                               \
    };                                                                  \
    static test_name##_registrar_t test_name##_registrar;               \
    }

#define ASSERT_EQ(expected, actual)                                        \
    yjcx::kunlun::base::test::assertion::assertEqual((expected), (actual), \
                                                     #expected " == " #actual)

#define ASSERT_NE(expected, actual)                      \
    yjcx::kunlun::base::test::assertion::assertNotEqual( \
        (expected), (actual), #expected " != " #actual)

#define ASSERT_TRUE(condition) \
    yjcx::kunlun::base::test::assertion::assertTrue((condition), #condition)

#define ASSERT_FALSE(condition) \
    yjcx::kunlun::base::test::assertion::assertFalse((condition), #condition)

#define ASSERT_NEAR(expected, actual, epsilon)       \
    yjcx::kunlun::base::test::assertion::assertNear( \
        (expected), (actual), (epsilon),             \
        #expected " ≈ " #actual " (ε=" #epsilon ")")

template <typename T>
void suppressWarning(T)
{
    (void)sizeof(basic::TestCase);
    (void)sizeof(basic::TestRegistry);
    (void)sizeof(basic::TestSuite);

    using yjcx::kunlun::base::test::assertion::assertNotEqual;
}

}  // namespace yjcx::kunlun::base::test::automate

#endif
