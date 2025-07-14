#pragma once

// finfuns library
//
//  Copyright Joanna Hulboj 2025. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)


#if defined(_MSC_VER)
#    define FINFUNS_NOINLINE __declspec(noinline)
#elif defined(__GNUC__) || defined(__clang__)
#    define FINFUNS_NOINLINE __attribute__((noinline))
#else
#    define FINFUNS_NOINLINE
#endif
