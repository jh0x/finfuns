#pragma once

// finfuns library
//
//  Copyright Joanna Hulboj 2025. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <finfuns/npv_calculator.hpp>

#include <cmath>
#include <cstdint>
#include <expected>

namespace finfuns
{

enum class NPVError : int32_t
{
    InvalidRate, //!< rate is NaN/infinity
    EmptyCashflows, //!< cashflows.empty()
};

template <IndexMode index_mode>
std::expected<double, NPVError> npv(double rate, std::span<const double> cashflows)
{
    if (std::isnan(rate) || std::isinf(rate)) [[unlikely]]
        return std::unexpected(NPVError::InvalidRate);
    if (cashflows.empty()) [[unlikely]]
        return std::unexpected(NPVError::EmptyCashflows);
    auto calc = NpvCalculator(cashflows);
    return calc.template calculate<index_mode>(rate);
}

}
