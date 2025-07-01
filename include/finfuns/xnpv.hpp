#pragma once

// finfuns library
//
//  Copyright Joanna Hulboj 2025. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see http://www.boost.org/libs/assign/
//

#include <finfuns/day_count.hpp>
#include <finfuns/xnpv_calculator.hpp>

#include <cmath>
#include <cstdint>
#include <expected>

namespace finfuns
{

enum class XNPVError : int32_t
{
    InvalidRate, //!< rate is NaN/infinity
    EmptyCashflows, //!< cashflows.empty()
    CashflowsDatesSizeMismatch, //!< cashflows size does not match dates size
};

template <DayCountConvention day_count, typename DateType>
std::expected<double, XNPVError> xnpv(double rate, std::span<const double> cashflows, std::span<const DateType> dates)
{
    if (std::isnan(rate) || std::isinf(rate)) [[unlikely]]
        return std::unexpected(XNPVError::InvalidRate);
    if (cashflows.empty()) [[unlikely]]
        return std::unexpected(XNPVError::EmptyCashflows);
    if (cashflows.size() != dates.size()) [[unlikely]]
        return std::unexpected(XNPVError::CashflowsDatesSizeMismatch);
    auto calc = XnpvCalculator<DateType, day_count>(cashflows, dates);
    return calc.calculate(rate);
}

}
