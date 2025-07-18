// finfuns library
//
//  Copyright Joanna Hulboj 2025. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// include/finfuns/expected.hpp
#pragma once

#pragma once

#if __cplusplus >= 202302L
#    include <version>
#    ifdef __cpp_lib_expected
#        include <expected>
namespace finfuns
{
using std::expected;
using std::unexpected;
}
#        define FINFUNS_USE_STD_EXPECTED 1
#    endif
#endif

#ifndef FINFUNS_USE_STD_EXPECTED
#    include <tl/expected.hpp>
namespace finfuns
{
using tl::expected;
using tl::unexpected;
}
#endif
