#pragma once

// finfuns library
//
//  Copyright Joanna Hulboj 2025. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <finfuns/payment_due_type.hpp>

#include <cmath>
#include <cstdint>

namespace finfuns
{

template <PaymentDueType due_type>
inline double fv(double rate, uint32_t periods, double pmt, double present_value)
{
    if (rate <= -1.0) [[unlikely]]
        return std::numeric_limits<double>::quiet_NaN();
    if (rate == 0.)
        return -pmt * periods - present_value;

    double fv_factor = std::pow(1.0 + rate, periods);
    double annuity_component = -pmt * (fv_factor - 1.0) / rate;

    if constexpr (due_type == PaymentDueType::EndOfPeriod)
        return annuity_component - present_value * fv_factor;
    else if constexpr (due_type == PaymentDueType::BeginningOfPeriod)
        return (annuity_component * (1.0 + rate)) - present_value * fv_factor;
    else
        []<bool flag = false>() { static_assert(flag, "Unsupported PaymentDueType"); }();
}

}
