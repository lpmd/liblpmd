/*
 *
 *
 *
 */

#ifndef __LPUNIT_MACROS_H__
#define __LPUNIT_MACROS_H__

#include <lpunit/test.h>

#define LPUNIT_ASSERT(a, s_a) \
        if (! a) { std::cout << "\n[Fail] Assertion " << s_a << " failed.\n"; \
        lpunit::Assert(false); }

#define LPUNIT_EQUAL(a, b, s_a, s_b) \
        if (a != b) { std::cout << "\n[Fail] Expected " << s_a << " to be " << b << ", is " << a << " instead.\n"; lpunit::Assert(false); }

#define LPUNIT_NOTEQUAL(a, b, s_a, s_b) \
        if (a == b) { std::cout << "\n[Fail] Expected " << s_a << " to be different from " << s_b << ", but they are both equal to " << a << '\n'; \
        lpunit::Assert(false); }

#define LPUNIT_APPROX(a, b, s_a, s_b, tol) \
        if (fabs(a-b) >= tol) { std::cout << "\n[Fail] Expected " << s_a << " to be " << b << ", is " << a << " instead, tolerance: " << tol << '\n'; \
        lpunit::Assert(false); }

#define LPUNIT_NOTAPPROX(a, b, s_a, s_b, tol) \
        if (fabs(a-b) < tol) { std::cout << "\n[Fail] Expected " << s_a << " to be different from " << s_b << ", but they are both equal to " << a << ", tolerance: " << tol << '\n'; \
        lpunit::Assert(false); }

#endif


