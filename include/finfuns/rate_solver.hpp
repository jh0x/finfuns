#pragma once

// finfuns library
//
//  Copyright Joanna Hulboj 2025. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <finfuns/expected.hpp>

#include <boost/math/tools/roots.hpp>

#include <cmath>
#include <limits>
#include <string_view>
#include <utility>

namespace finfuns
{

enum class SolverErrorCode
{
    CANNOT_EVALUATE_VALUE,
    CANNOT_CONVERGE_DUE_TO_ROUNDING_ERRORS,
    CANNOT_CONVERGE_DUE_TO_INVALID_ARGUMENTS,
    NO_ROOT_FOUND_IN_BRACKET,
    OTHER_ERROR
};

constexpr std::string_view error_to_sv(SolverErrorCode error)
{
    switch (error)
    {
        case SolverErrorCode::CANNOT_EVALUATE_VALUE:
            return "Cannot evaluate value";
        case SolverErrorCode::CANNOT_CONVERGE_DUE_TO_ROUNDING_ERRORS:
            return "Cannot converge due to rounding errors";
        case SolverErrorCode::CANNOT_CONVERGE_DUE_TO_INVALID_ARGUMENTS:
            return "Cannot converge due to invalid arguments";
        case SolverErrorCode::NO_ROOT_FOUND_IN_BRACKET:
            return "No root found in the specified bracket";
        case SolverErrorCode::OTHER_ERROR:
            return "Other error occurred";
    }
    return "Unknown error code";
}

template <typename Calculator>
struct CachedFunction
{
    Calculator & _calculator;
    double _last_rate = std::numeric_limits<double>::quiet_NaN();
    std::pair<double, double> _last_result;

    explicit CachedFunction(Calculator & c)
        : _calculator{c}
    {
    }

    std::pair<double, double> operator()(double rate)
    {
        if (rate != _last_rate)
        {
            _last_result = _calculator.calculate_with_derivative(rate);
            _last_rate = rate;
        }
        return _last_result;
    }
};

template <typename Calculator>
expected<double, SolverErrorCode> rate_solver(Calculator && calculator, double guess)
{
    constexpr int max_iterations = 100;
    constexpr double start_lower_bound = -0.999999; // Avoid the rate of -1.
    constexpr double start_upper_bound = 100.0; // Reasonable upper bound for financial applications IRR/XIRR
    constexpr double tolerance = 1e-6; // Tolerance for the result check
    try
    {
        boost::uintmax_t max_iter = max_iterations;

        // 53 - 4 bits of precision => ~14.75 decimal digits - accurate enough for our purpose
        constexpr auto binary_precision = std::numeric_limits<double>::digits - 4;

        auto cached_fun = CachedFunction(calculator);
        auto f0 = [&cached_fun](double rate) { return cached_fun(rate).first; };
        auto f1 = [&cached_fun](double rate) { return cached_fun(rate).second; };

        double result = boost::math::tools::newton_raphson_iterate(
            [&f0, &f1](double x) { return std::make_tuple(f0(x), f1(x)); },
            guess,
            start_lower_bound,
            start_upper_bound,
            binary_precision,
            max_iter);

        auto fun = [&calculator](double rate) { return calculator.calculate(rate); };
        if (result >= start_lower_bound && result <= start_upper_bound && std::abs(fun(result)) < tolerance)
            return result;

        // Fallback to TOMS748
        const double f_lower = fun(start_lower_bound);
        const double f_upper = fun(start_upper_bound);

        if (f_lower * f_upper >= 0.0) [[unlikely]]
            return unexpected(SolverErrorCode::NO_ROOT_FOUND_IN_BRACKET);

        max_iter = max_iterations;
        boost::math::tools::eps_tolerance<double> tol(std::numeric_limits<double>::digits - 4);
        auto toms_result = boost::math::tools::toms748_solve(fun, start_lower_bound, start_upper_bound, tol, max_iter);

        return toms_result.first;
    }
    catch (const boost::math::evaluation_error &)
    {
        return unexpected(SolverErrorCode::CANNOT_EVALUATE_VALUE);
    }
    catch (const boost::math::rounding_error &)
    {
        return unexpected(SolverErrorCode::CANNOT_CONVERGE_DUE_TO_ROUNDING_ERRORS);
    }
    catch (const std::domain_error &)
    {
        return unexpected(SolverErrorCode::CANNOT_CONVERGE_DUE_TO_INVALID_ARGUMENTS);
    }
    catch (...)
    {
        return unexpected(SolverErrorCode::OTHER_ERROR);
    }
}

}
