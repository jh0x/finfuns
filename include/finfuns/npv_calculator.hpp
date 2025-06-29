#pragma once

// finfuns library
//
//  Copyright Joanna Hulboj 2025. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <cmath>
#include <cstdint>
#include <expected>
#include <numeric>
#include <span>

namespace finfuns
{

enum class IndexMode : int32_t
{
    ZeroBased, //!< Cashflows are indexed starting from 0
    OneBased //!< Cashflows are indexed starting from 1 (Excel style)
};

struct NpvCalculator
{
    std::span<const double> _cashflows;

    explicit NpvCalculator(std::span<const double> cashflows)
        : _cashflows{cashflows}
    {
    }

    template <IndexMode index_mode = IndexMode::ZeroBased>
    double calculate(double rate) const
    {
        if (rate == 0)
            return std::accumulate(std::begin(_cashflows), std::end(_cashflows), 0.0);
        if (rate <= -1.0) [[unlikely]]
            return std::numeric_limits<double>::infinity();

        double npv = 0.0;
        const double growth_factor = 1.0 + rate;
        if constexpr (index_mode == IndexMode::ZeroBased)
        {
            // First cashflow (t=0) is not discounted
            npv = _cashflows[0];

            // Discount subsequent cashflows (t=1, t=2, ...)
            double discount_factor = growth_factor; // (1+r)^1
            for (size_t i = 1; i < _cashflows.size(); ++i)
            {
                npv += _cashflows[i] / discount_factor;
                discount_factor *= growth_factor;
            }
        }
        else
        {
            // IndexMode::OneBased
            // All cashflows are discounted (t=1, t=2, ...)
            double discount_factor = growth_factor; // Start with (1+r)^1 for t=1
            for (const auto cf : _cashflows)
            {
                npv += cf / discount_factor;
                discount_factor *= growth_factor;
            }
        }
        return npv;
    }

    // Used only for IRR calculation, hence just ZeroBased
    double derivative(double rate) const
    {
        if (rate <= -1.0)
            return std::numeric_limits<double>::quiet_NaN();

        double derivative = 0.0;
        double compound = (1.0 + rate);

        for (size_t i = 1; i < _cashflows.size(); ++i)
        {
            compound *= (1.0 + rate);
            derivative -= _cashflows[i] * static_cast<double>(i) / compound;
        }
        return derivative;
    }
};

}
