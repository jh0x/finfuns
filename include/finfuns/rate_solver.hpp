#pragma once

// finfuns library
//
//  Copyright Joanna Hulboj 2025. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/math/tools/roots.hpp>

#include <cmath>
#include <expected>
#include <limits>
#include <utility>

namespace finfuns
{

enum class SolverErrorCode
{
    CANNOT_EVALUATE_VALUE,
    CANNOT_CONVERGE_DUE_TO_ROUNDING_ERRORS,
    CANNOT_CONVERGE_DUE_TO_INVALID_ARGUMENTS,
    CANNOT_CONVERGE_TOO_MANY_ITERATIONS,
    INPUT_DATES_NOT_SORTED_UNIQUE,
    NO_ROOT_FOUND_IN_BRACKET,
    OTHER_ERROR
};

template <typename Function, typename Derivative>
std::expected<double, SolverErrorCode> rate_solver(Function && fun, Derivative && der, double guess)
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

        double result = boost::math::tools::newton_raphson_iterate(
            [&fun, &der](double x) { return std::make_tuple(fun(x), der(x)); },
            guess,
            start_lower_bound,
            start_upper_bound,
            binary_precision,
            max_iter);

        if (result >= start_lower_bound && result <= start_upper_bound && std::abs(fun(result)) < tolerance)
            return result;

        // Fallback to TOMS748
        const double f_lower = fun(start_lower_bound);
        const double f_upper = fun(start_upper_bound);

        if (f_lower * f_upper >= 0.0) [[unlikely]]
            return std::unexpected(SolverErrorCode::NO_ROOT_FOUND_IN_BRACKET);

        max_iter = max_iterations;
        boost::math::tools::eps_tolerance<double> tol(std::numeric_limits<double>::digits - 4);
        auto toms_result = boost::math::tools::toms748_solve(fun, start_lower_bound, start_upper_bound, tol, max_iter);

        return toms_result.first;
    }
    catch (const boost::math::evaluation_error &)
    {
        return std::unexpected(SolverErrorCode::CANNOT_EVALUATE_VALUE);
    }
    catch (const boost::math::rounding_error &)
    {
        return std::unexpected(SolverErrorCode::CANNOT_CONVERGE_DUE_TO_ROUNDING_ERRORS);
    }
    catch (const std::domain_error &)
    {
        return std::unexpected(SolverErrorCode::CANNOT_CONVERGE_DUE_TO_INVALID_ARGUMENTS);
    }
    catch (...)
    {
        return std::unexpected(SolverErrorCode::OTHER_ERROR);
    }
}

}
