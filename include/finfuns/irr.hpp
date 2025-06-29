#pragma once

// finfuns library
//
//  Copyright Joanna Hulboj 2025. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <finfuns/npv_calculator.hpp>
#include <finfuns/rate_solver.hpp>

#include <optional>
#include <variant>

namespace finfuns
{

enum class IRRErrorCode : int32_t
{
    NotEnoughCashflows, //!< At least two cashflows are required
    SameSignCashflows, //!< All cashflows have the same sign (all positive or all negative)
};

using IRRError = std::variant<IRRErrorCode, SolverErrorCode>;

std::expected<double, IRRError> irr(std::span<const double> cashflows, std::optional<double> guess)
{
    if (cashflows.size() <= 1) [[unlikely]]
        return std::unexpected(IRRErrorCode::NotEnoughCashflows);

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
        return std::unexpected(IRRErrorCode::SameSignCashflows);

    const double guess_value = guess.value_or(0.1);
    auto npv = NpvCalculator(cashflows);
    auto npv_function = [&](double rate) { return npv.calculate(rate); };
    auto npv_derivative = [&](double rate) { return npv.derivative(rate); };

    auto res = rate_solver(npv_function, npv_derivative, guess_value);
    if (res.has_value()) [[likely]]
        return res.value();
    return std::unexpected(res.error());
}

}
