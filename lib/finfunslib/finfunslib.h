#pragma once

// finfuns library
//
//  Copyright Joanna Hulboj 2025. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)


#include "shared/finfunslib_export.hpp"

#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Calculates the present value (PV) of an annuity with end-of-period payments.
 *
 * Computes the present value of a series of equal payments (an annuity), optionally
 * including a future lump sum, assuming payments are made at the end of each period.
 *
 * @param rate           The periodic interest rate (as a decimal, e.g., 0.05 for 5%).
 * @param periods        The total number of payment periods.
 * @param pmt            The payment amount made each period (outflow).
 * @param future_value   A future value lump sum (inflow) received at the end of the last period.
 *
 * @return The present value of the annuity.
 *
 * @note Assumes end-of-period payments. For beginning-of-period (due), use a different function.
 */
FINFUNSLIB_EXPORT double finfuns_pv_due_end(double rate, uint32_t periods, double pmt, double future_value) noexcept;

/**
 * @brief Calculates the present value (PV) of an annuity with begin-of-period payments.
 *
 * Computes the present value of a series of equal payments (an annuity), optionally
 * including a future lump sum, assuming payments are made at the beginning of each period.
 *
 * @param rate           The periodic interest rate (as a decimal, e.g., 0.05 for 5%).
 * @param periods        The total number of payment periods.
 * @param pmt            The payment amount made each period (outflow).
 * @param future_value   A future value lump sum (inflow) received at the end of the last period.
 *
 * @return The present value of the annuity.
 *
 * @note Assumes begin-of-period payments. For end-of-period, use a different function.
 */
FINFUNSLIB_EXPORT double finfuns_pv_due_begin(double rate, uint32_t periods, double pmt, double future_value) noexcept;


/**
 * @brief Calculates the future value (FV) of an annuity with end-of-period payments.
 *
 * Computes the future value of a series of equal payments (an annuity), optionally
 * including a present value lump sum, assuming payments are made at the end of each period.
 *
 * @param rate            The periodic interest rate (as a decimal, e.g., 0.05 for 5%).
 * @param periods         The total number of payment periods.
 * @param pmt             The payment amount made each period (outflow).
 * @param present_value   The initial present value or lump sum (inflow) at period zero.
 *
 * @return The future value of the annuity at the end of the final period.
 *
 * @note Assumes end-of-period payments. For beginning-of-period (due), use a different function.
 */
FINFUNSLIB_EXPORT double finfuns_fv_due_end(double rate, uint32_t periods, double pmt, double present_value) noexcept;

/**
 * @brief Calculates the future value (FV) of an annuity with begin-of-period payments.
 *
 * Computes the future value of a series of equal payments (an annuity), optionally
 * including a present value lump sum, assuming payments are made at the beginning of each period.
 *
 * @param rate            The periodic interest rate (as a decimal, e.g., 0.05 for 5%).
 * @param periods         The total number of payment periods.
 * @param pmt             The payment amount made each period (outflow).
 * @param present_value   The initial present value or lump sum (inflow) at period zero.
 *
 * @return The future value of the annuity at the end of the final period.
 *
 * @note Assumes begin-of-period payments. For end-of-period, use a different function.
 */
FINFUNSLIB_EXPORT double finfuns_fv_due_begin(double rate, uint32_t periods, double pmt, double present_value) noexcept;

/**
 * @brief Error codes for finfuns calculations
 *
 * @note Codes <100 indicate input validation errors,
 *       >=100 indicate numerical convergence failures
 */
typedef enum
{
    FINFUNS_CODE_SUCCESS = 0, ///< Operation completed successfully

    // Common errors
    FINFUNS_CODE_NOT_ENOUGH_CASHFLOWS, ///< At least two cashflows are required
    FINFUNS_CODE_SAME_SIGN_CASHFLOWS, ///< All cashflows have the same sign
    FINFUNS_CODE_INVALID_RATE, ///< Rate is NaN, infinite, or otherwise invalid
    FINFUNS_CODE_EMPTY_CASHFLOWS, ///< No cashflows provided (nullptr or zero length)
    FINFUNS_CODE_SIZE_MISMATCH, ///< Cashflows/dates size mismatch
    FINFUNS_CODE_UNSUPPORTED_DAYCOUNT, ///< Unsupported or invalid day count convention

    // Numerical errors
    FINFUNS_CODE_CANNOT_EVALUATE_VALUE = 100, ///< Numerical instability during evaluation
    FINFUNS_CODE_CANNOT_CONVERGE_ROUNDING, ///< Failed due to floating-point rounding errors
    FINFUNS_CODE_CANNOT_CONVERGE_ARGUMENTS, ///< Invalid mathematical arguments
    FINFUNS_CODE_NO_ROOT_IN_BRACKET, ///< No solution exists in search interval
    FINFUNS_CODE_SOLVER_OTHER_ERROR, ///< Unspecified error condition

    // Unexpected error
    FINFUNS_CODE_UNEXPECTED_ERROR = 999,
} FinFunsCode;


/**
 * @brief Calculates Internal Rate of Return (IRR)
 *
 * @param cashflows Array of cash flows (must contain at least one negative and one positive value)
 * @param num_cashflows Number of elements in cashflows array (must be >= 2)
 * @param guess Initial guess for the IRR (recommended: 0.1 for 10%)
 * @param[out] out_result Calculated IRR (valid only when return code is IRR_SUCCESS)
 *
 * @return FinFunsCode error code
 *
 * @note The function uses Newton-Raphson iteration with fallback to bracketing methods.
 *
 * @warning The first cashflow should typically be negative (initial investment).
 */
FINFUNSLIB_EXPORT [[nodiscard]] FinFunsCode
finfuns_irr(const double * cashflows, int num_cashflows, double guess, double * out_result) noexcept;

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
 * @return FinFunsCode error code
 *
 * @note For ZeroBased mode, the first cashflow is considered to occur at time t=0.
 *       For OneBased mode, all cashflows are discounted (first occurs at t=1).
 */
FINFUNSLIB_EXPORT [[nodiscard]] FinFunsCode
finfuns_npv(FinFunsIndexMode mode, double rate, const double * cashflows, int num_cashflows, double * out_result) noexcept;


/**
 * @brief Day count conventions
 */
typedef enum
{
    FINFUNS_ACT_365F, ///< Actual days / 365-day year (ISDA)
    FINFUNS_ACT_365_25, ///< Actual days / 365.25-day year (ISDA)
} FinFunsDayCount;

/**
 * @brief Calculates XNPV with dates as days since epoch (1970-01-01). Could also jus use relative days if the given date convention supports it.
 *
 * @param day_count Day count convention (see FinFunsDayCount)
 * @param rate Discount rate (must be >= -1.0 and finite)
 * @param cashflows Array of cash flows
 * @param dates Array of dates (days since epoch)
 * @param num_dates Length of cashflows / dates array
 * @param[out] out_result Calculated XNPV (valid only if return code is SUCCESS)
 *
 * @return FinFunsXNPVError error code
 *
 * @warning First cashflow (typically the investment) should be negative
 */
FINFUNSLIB_EXPORT [[nodiscard]] FinFunsCode finfuns_xnpv(
    FinFunsDayCount day_count, double rate, const double * cashflows, const int * dates, int num_cashflows, double * out_result) noexcept;

/**
 * @brief Calculates XIRR with dates as days since epoch (1970-01-01). Could also jus use relative days if the given date convention supports it.
 *
 * @param day_count Day count convention (see FinFunsXIRRDayCount)
 * @param cashflows Array of cash flows (must contain both positive/negative values)
 * @param dates Array of dates (days since epoch)
 * @param num_cashflows Length of cashflows array (must be >= 2)
 * @param guess Initial guess for the rate (suggested: 0.1 for 10%)
 * @param[out] out_result Calculated XIRR (valid only if return code is SUCCESS)
 *
 * @return FinFunsCode error code
 *
 * @note Uses modified Newton-Raphson with fallback to bracketing when needed
 * @warning First cashflow should typically be negative (initial investment)
 */
FINFUNSLIB_EXPORT [[nodiscard]] FinFunsCode finfuns_xirr(
    FinFunsDayCount day_count, const double * cashflows, const int * dates, int num_cashflows, double guess, double * out_result) noexcept;


#ifdef __cplusplus
}
#endif
