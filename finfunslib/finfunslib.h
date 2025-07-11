#pragma once

// finfuns library
//
//  Copyright Joanna Hulboj 2025. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)


#include "shared/finfunslib_export.hpp"

#include <cstdint>

extern "C" {

FINFUNSLIB_EXPORT double finfuns_pv_due_end(double rate, uint32_t periods, double pmt, double future_value);
FINFUNSLIB_EXPORT double finfuns_pv_due_begin(double rate, uint32_t periods, double pmt, double future_value);

FINFUNSLIB_EXPORT double finfuns_fv_due_end(double rate, uint32_t periods, double pmt, double present_value);
FINFUNSLIB_EXPORT double finfuns_fv_due_begin(double rate, uint32_t periods, double pmt, double present_value);
}
