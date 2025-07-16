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
#include <limits>

namespace finfuns
{

template <PaymentDueType due_type>
inline double pv(double rate, uint32_t periods, double pmt, double future_value)
{
    if (rate <= -1.0) [[unlikely]]
        return std::numeric_limits<double>::quiet_NaN();
    if (rate == 0.)
        return -pmt * periods - future_value;

    double pv_factor = 1.0 / std::pow(1.0 + rate, periods);
    double annuity_component = -pmt * (1.0 - pv_factor) / rate;

    if constexpr (due_type == PaymentDueType::EndOfPeriod)
        return annuity_component - future_value * pv_factor;
    else if constexpr (due_type == PaymentDueType::BeginningOfPeriod)
        return (annuity_component * (1.0 + rate)) - future_value * pv_factor;
    else
        []<bool flag = false>() { static_assert(flag, "Unsupported PaymentDueType"); }();
}

}
