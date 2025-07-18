#pragma once

// finfuns library
//
//  Copyright Joanna Hulboj 2025. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <finfuns/rate_solver.hpp>
#include <finfuns/xnpv_calculator.hpp>

#include <expected>
#include <optional>
#include <string_view>
#include <variant>

namespace finfuns
{

enum class XIRRErrorCode : int32_t
{
    NotEnoughCashflows, //!< At least two cashflows are required
    SameSignCashflows, //!< All cashflows have the same sign (all positive or all negative)
    CashflowsDatesSizeMismatch, //!< cashflows size does not match dates size
    UnsupportedDayCountConvention, //!< unsupported day count convention
};

using XIRRError = std::variant<XIRRErrorCode, SolverErrorCode>;

constexpr std::string_view error_to_sv(XIRRErrorCode error)
{
    switch (error)
    {
        case XIRRErrorCode::NotEnoughCashflows:
            return "Not enough cashflows: at least two are required";
        case XIRRErrorCode::SameSignCashflows:
            return "All cashflows have the same sign (all positive or all negative)";
        case XIRRErrorCode::CashflowsDatesSizeMismatch:
            return "Cashflows and dates arrays must have the same size";
        case XIRRErrorCode::UnsupportedDayCountConvention:
            return "Unsupported day count convention";
        default:
            return "Unknown XIRR error";
    }
}

constexpr std::string_view error_to_sv(const XIRRError & error)
{
    return std::visit([](const auto & e) { return error_to_sv(e); }, error);
}

template <DayCountConvention day_count, typename DateType>
std::expected<double, XIRRError> xirr(std::span<const double> cashflows, std::span<const DateType> dates, std::optional<double> guess)
{
    if (cashflows.size() <= 1) [[unlikely]]
        return std::unexpected(XIRRErrorCode::NotEnoughCashflows);

    if (cashflows.size() != dates.size()) [[unlikely]]
        return std::unexpected(XIRRErrorCode::CashflowsDatesSizeMismatch);

    bool has_positive = false;
    bool has_negative = false;

    for (double cf : cashflows)
    {
        if (cf > 0)
            has_positive = true;
        if (cf < 0)
            has_negative = true;
        if (has_positive && has_negative)
            break;
    }
    if (not(has_negative && has_positive)) [[unlikely]]
        return std::unexpected(XIRRErrorCode::SameSignCashflows);

    const double guess_value = guess.value_or(0.1);
    auto xnpv = XnpvCalculator<DateType, day_count>(cashflows, dates);

    auto res = rate_solver(std::move(xnpv), guess_value);
    if (res.has_value()) [[likely]]
        return res.value();
    return std::unexpected(res.error());
}

template <DayCountConvention day_count, typename DateContainer>
std::expected<double, XIRRError> xirr(std::span<const double> cashflows, DateContainer && dates, std::optional<double> guess)
{
    using ContainedType = std::remove_cvref_t<decltype(*std::begin(dates))>;

    return xirr<day_count>(
        cashflows,
        std::span<const ContainedType>{dates.data(), static_cast<std::size_t>(std::distance(std::begin(dates), std::end(dates)))},
        guess);
}

}
