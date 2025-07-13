#pragma once

// finfuns library
//
//  Copyright Joanna Hulboj 2025. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)


#include "shared/finfunslib_export.hpp"

#include <cstdint>

extern "C" {

FINFUNSLIB_EXPORT double finfuns_pv_due_end(double rate, uint32_t periods, double pmt, double future_value) noexcept;
FINFUNSLIB_EXPORT double finfuns_pv_due_begin(double rate, uint32_t periods, double pmt, double future_value) noexcept;

FINFUNSLIB_EXPORT double finfuns_fv_due_end(double rate, uint32_t periods, double pmt, double present_value) noexcept;
FINFUNSLIB_EXPORT double finfuns_fv_due_begin(double rate, uint32_t periods, double pmt, double present_value) noexcept;

/**
 * @brief Error codes for IRR calculation
 */
typedef enum
{
    FINFUNS_IRR_SUCCESS = 0, ///< Calculation succeeded
    FINFUNS_IRR_NOT_ENOUGH_CASHFLOWS, ///< At least two cashflows are required
    FINFUNS_IRR_SAME_SIGN_CASHFLOWS, ///< All cashflows have the same sign
    FINFUNS_IRR_CANNOT_EVALUATE_VALUE = 100, ///< Numerical instability during evaluation
    FINFUNS_IRR_CANNOT_CONVERGE_ROUNDING, ///< Failed due to floating-point rounding errors
    FINFUNS_IRR_CANNOT_CONVERGE_ARGUMENTS, ///< Invalid mathematical arguments
    FINFUNS_IRR_NO_ROOT_IN_BRACKET, ///< No solution exists in search interval
    FINFUNS_IRR_OTHER_ERROR ///< Unspecified error condition
} FinFunsIRRCode;

/**
 * @brief Calculates Internal Rate of Return (IRR)
 *
 * @param cashflows Array of cash flows (must contain at least one negative and one positive value)
 * @param num_cashflows Number of elements in cashflows array (must be >= 2)
 * @param guess Initial guess for the IRR (recommended: 0.1 for 10%)
 * @param[out] out_result Calculated IRR (valid only when return code is IRR_SUCCESS)
 *
 * @return FinFunsIRRCode error code
 *
 * @note The function uses Newton-Raphson iteration with fallback to bracketing methods.
 *
 * @warning The first cashflow should typically be negative (initial investment).
 */
FinFunsIRRCode finfuns_irr(const double * cashflows, int num_cashflows, double guess, double * out_result) noexcept;

/**
 * @brief Error codes for NPV calculations
 */
typedef enum
{
    FINFUNS_NPV_SUCCESS = 0, ///< Operation completed successfully
    FINFUNS_NPV_INVALID_RATE, ///< Rate is NaN, infinite, or otherwise invalid
    FINFUNS_NPV_EMPTY_CASHFLOWS ///< No cashflows provided (nullptr or zero length)
} FinFunsNPVCode;

/**
 * @brief Time period indexing convention
 */
typedef enum
{
    FINFUNS_ZERO_BASED, ///< First period is index 0 (standard financial convention)
    FINFUNS_ONE_BASED ///< First period is index 1 (alternative convention)
} FinFunsIndexMode;

/**
 * @brief Calculate Net Present Value (NPV)
 *
 * @param[in] mode Time period convention (ZeroBased/OneBased)
 * @param[in] rate Discount rate per period (must be finite and >= -1.0)
 * @param[in] cashflows Array of cash flows (must contain at least one element)
 * @param[in] num_cashflows Number of cash flows (must be > 0)
 * @param[out] out_result Calculated NPV (valid only if return code is FINFUNS_NPV_SUCCESS)
 *
 * @return FinFunsNPVCode error code
 *
 * @note For ZeroBased mode, the first cashflow is considered to occur at time t=0.
 *       For OneBased mode, all cashflows are discounted (first occurs at t=1).
 */
FinFunsNPVCode finfuns_npv(FinFunsIndexMode mode, double rate, const double * cashflows, int num_cashflows, double * out_result) noexcept;
}
