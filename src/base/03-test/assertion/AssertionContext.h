#ifndef YJCX_KUNLUN_ASSERTION_ASSERTIONCONTEXT_H
#define YJCX_KUNLUN_ASSERTION_ASSERTIONCONTEXT_H

#include <string_view>

#include "TestCase.h"

namespace yjcx::kunlun::base::test::assertion
{

class AssertionContext
{
public:
    AssertionContext() = delete;
    ~AssertionContext() = delete;

    static void setCase(const basic::TestCase* pCase);
    static basic::TestCase* getCase();
    static void record(bool passed, std::string_view message);

private:
    thread_local static basic::TestCase* pCase_;
};

}  // namespace yjcx::kunlun::base::test::assertion

#endif
