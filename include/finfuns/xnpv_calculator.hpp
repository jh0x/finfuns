#pragma once

// finfuns library
//
//  Copyright Joanna Hulboj 2025. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <finfuns/day_count.hpp>

#include <cmath>
#include <expected>
#include <span>

namespace finfuns
{

template <typename DateType, DayCountConvention day_count>
struct XnpvCalculator
{
    std::span<const double> _cashflows;
    std::span<const DateType> _dates;

    XnpvCalculator(std::span<const double> cashflows, std::span<const DateType> dates)
        : _cashflows(cashflows)
        , _dates(dates)
    {
    }

    double calculate(double rate) const
    {
        if (rate <= -1.0)
            return std::numeric_limits<double>::infinity();

        double npv = 0.0;
        const double one_plus_rate = 1.0 + rate;
        const double log1pr = std::log(one_plus_rate);

        for (size_t i = 0; i < _cashflows.size(); ++i)
        {
            double time = year_fraction<day_count>(_dates[0], _dates[i]);
            if (time == 0.0)
                npv += _cashflows[i];
            else
                npv += _cashflows[i] / std::exp(time * log1pr);
        }

        return npv;
    }

    double derivative(double rate) const
    {
        if (rate <= -1.0)
            return std::numeric_limits<double>::infinity();

        double derivative = 0.0;
        const double one_plus_rate = 1.0 + rate;
        const double log1pr = std::log(one_plus_rate);

        for (size_t i = 0; i < _cashflows.size(); ++i)
        {
            double time = year_fraction<day_count>(_dates[0], _dates[i]);
            if (time != 0.0)
                derivative -= _cashflows[i] * time / (one_plus_rate * std::exp(time * log1pr));
        }

        return derivative;
    }
};

}
