#pragma once

// finfuns library
//
//  Copyright Joanna Hulboj 2025. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <finfuns/day_count.hpp>
#include <finfuns/expected.hpp>
#include <finfuns/xnpv_calculator.hpp>

#include <cmath>
#include <cstdint>
#include <string_view>

namespace finfuns
{

enum class XNPVError : int32_t
{
    InvalidRate, //!< rate is NaN/infinity
    EmptyCashflows, //!< cashflows.empty()
    CashflowsDatesSizeMismatch, //!< cashflows size does not match dates size
    UnsupportedDayCountConvention, //!< unsupported day count convention
};

constexpr std::string_view error_to_sv(XNPVError error)
{
    switch (error)
    {
        case XNPVError::InvalidRate:
            return "Invalid rate: NaN or infinity";
        case XNPVError::EmptyCashflows:
            return "Cashflows array is empty";
        case XNPVError::CashflowsDatesSizeMismatch:
            return "Cashflows and dates arrays must have the same size";
        case XNPVError::UnsupportedDayCountConvention:
            return "Unsupported day count convention";
        default:
            return "Unknown XNPV error";
    }
}

template <DayCountConvention day_count, typename DateType>
expected<double, XNPVError> xnpv(double rate, std::span<const double> cashflows, std::span<const DateType> dates)
{
    if (std::isnan(rate) || std::isinf(rate)) [[unlikely]]
        return unexpected(XNPVError::InvalidRate);
    if (cashflows.empty()) [[unlikely]]
        return unexpected(XNPVError::EmptyCashflows);
    if (cashflows.size() != dates.size()) [[unlikely]]
        return unexpected(XNPVError::CashflowsDatesSizeMismatch);
    auto calc = XnpvCalculator<DateType, day_count>(cashflows, dates);
    return calc.calculate(rate);
}

template <DayCountConvention day_count, typename DateContainer>
expected<double, XNPVError> xnpv(double rate, std::span<const double> cashflows, DateContainer && dates)
{
    using ContainedType = std::remove_cvref_t<decltype(*std::begin(dates))>;

    return xnpv<day_count>(
        rate,
        cashflows,
        std::span<const ContainedType>{dates.data(), static_cast<std::size_t>(std::distance(std::begin(dates), std::end(dates)))});
}

}
