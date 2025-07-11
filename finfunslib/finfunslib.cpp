// finfuns library
//
//  Copyright Joanna Hulboj 2025. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include "finfunslib.h"

#include <finfuns/finfuns.hpp>

using namespace finfuns;

extern "C" {

double finfuns_pv_due_end(double rate, uint32_t periods, double pmt, double future_value)
{
    return pv<PaymentDueType::EndOfPeriod>(rate, periods, pmt, future_value);
}

double finfuns_pv_due_begin(double rate, uint32_t periods, double pmt, double future_value)
{
    return pv<PaymentDueType::BeginningOfPeriod>(rate, periods, pmt, future_value);
}

double finfuns_fv_due_end(double rate, uint32_t periods, double pmt, double present_value)
{
    return fv<PaymentDueType::EndOfPeriod>(rate, periods, pmt, present_value);
}

double finfuns_fv_due_begin(double rate, uint32_t periods, double pmt, double present_value)
{
    return fv<PaymentDueType::BeginningOfPeriod>(rate, periods, pmt, present_value);
}
}
