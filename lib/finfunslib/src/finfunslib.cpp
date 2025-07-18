// finfuns library
//
//  Copyright Joanna Hulboj 2025. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <finfunslib/finfunslib.h>

#include <finfuns/finfuns.hpp>
#include <finfuns/overloaded.hpp>
#include <finfuns/preprocessor.hpp>

#include <utility>


using namespace finfuns;

namespace
{
FINFUNS_NOINLINE FinFunsCode make_error_code(SolverErrorCode err)
{
    switch (err)
    {
        case SolverErrorCode::CANNOT_EVALUATE_VALUE:
            return FINFUNS_CODE_CANNOT_EVALUATE_VALUE;
        case SolverErrorCode::CANNOT_CONVERGE_DUE_TO_ROUNDING_ERRORS:
            return FINFUNS_CODE_CANNOT_CONVERGE_ROUNDING;
        case SolverErrorCode::CANNOT_CONVERGE_DUE_TO_INVALID_ARGUMENTS:
            return FINFUNS_CODE_CANNOT_CONVERGE_ARGUMENTS;
        case SolverErrorCode::NO_ROOT_FOUND_IN_BRACKET:
            return FINFUNS_CODE_NO_ROOT_IN_BRACKET;
        case SolverErrorCode::OTHER_ERROR:
            return FINFUNS_CODE_SOLVER_OTHER_ERROR;
    }
    return FINFUNS_CODE_UNEXPECTED_ERROR;
}

FINFUNS_NOINLINE FinFunsCode make_error_code(NPVError err)
{
    switch (err)
    {
        case NPVError::InvalidRate:
            return FINFUNS_CODE_INVALID_RATE;
        case NPVError::EmptyCashflows:
            return FINFUNS_CODE_EMPTY_CASHFLOWS;
    }
    return FINFUNS_CODE_UNEXPECTED_ERROR;
}

FINFUNS_NOINLINE FinFunsCode make_error_code(XNPVError err)
{
    switch (err)
    {
        case XNPVError::InvalidRate:
            return FINFUNS_CODE_INVALID_RATE;
        case XNPVError::EmptyCashflows:
            return FINFUNS_CODE_EMPTY_CASHFLOWS;
        case XNPVError::CashflowsDatesSizeMismatch:
            return FINFUNS_CODE_SIZE_MISMATCH;
        case XNPVError::UnsupportedDayCountConvention:
            return FINFUNS_CODE_UNSUPPORTED_DAYCOUNT;
    }
    return FINFUNS_CODE_UNEXPECTED_ERROR;
}


FINFUNS_NOINLINE FinFunsCode make_error_code(IRRErrorCode err)
{
    switch (err)
    {
        case IRRErrorCode::NotEnoughCashflows:
            return FINFUNS_CODE_NOT_ENOUGH_CASHFLOWS;
        case IRRErrorCode::SameSignCashflows:
            return FINFUNS_CODE_SAME_SIGN_CASHFLOWS;
    }
    return FINFUNS_CODE_UNEXPECTED_ERROR;
}

FINFUNS_NOINLINE FinFunsCode make_error_code(XIRRErrorCode err)
{
    switch (err)
    {
        case XIRRErrorCode::NotEnoughCashflows:
            return FINFUNS_CODE_NOT_ENOUGH_CASHFLOWS;
        case XIRRErrorCode::SameSignCashflows:
            return FINFUNS_CODE_SAME_SIGN_CASHFLOWS;
        case XIRRErrorCode::CashflowsDatesSizeMismatch:
            return FINFUNS_CODE_SIZE_MISMATCH;
        case XIRRErrorCode::UnsupportedDayCountConvention:
            return FINFUNS_CODE_UNSUPPORTED_DAYCOUNT;
    }
    return FINFUNS_CODE_UNEXPECTED_ERROR;
}

}

#ifdef __cplusplus
extern "C" {
#endif

double finfuns_pv_due_end(double rate, uint32_t periods, double pmt, double future_value) noexcept
{
    return pv<PaymentDueType::EndOfPeriod>(rate, periods, pmt, future_value);
}

double finfuns_pv_due_begin(double rate, uint32_t periods, double pmt, double future_value) noexcept
{
    return pv<PaymentDueType::BeginningOfPeriod>(rate, periods, pmt, future_value);
}

double finfuns_fv_due_end(double rate, uint32_t periods, double pmt, double present_value) noexcept
{
    return fv<PaymentDueType::EndOfPeriod>(rate, periods, pmt, present_value);
}

double finfuns_fv_due_begin(double rate, uint32_t periods, double pmt, double present_value) noexcept
{
    return fv<PaymentDueType::BeginningOfPeriod>(rate, periods, pmt, present_value);
}

FinFunsCode finfuns_irr(const double * cashflows, unsigned num_cashflows, double guess, double * out_result) noexcept
{
    const auto cf_span = std::span<const double>(cashflows, num_cashflows);
    auto result = irr(cf_span, guess);
    if (result.has_value()) [[likely]]
    {
        *out_result = result.value();
        return FINFUNS_CODE_SUCCESS;
    }
    auto error = result.error();
    auto error_code = std::visit([](auto e) { return make_error_code(e); }, error);
    return error_code;
}

FinFunsCode finfuns_npv(FinFunsIndexMode mode, double rate, const double * cashflows, unsigned num_cashflows, double * out_result) noexcept
{
    const auto cf_span = std::span{cashflows, static_cast<size_t>(num_cashflows)};
    auto result = (mode == FINFUNS_ZERO_BASED) ? npv<IndexMode::ZeroBased>(rate, cf_span) : npv<IndexMode::OneBased>(rate, cf_span);
    if (result.has_value()) [[likely]]
    {
        *out_result = result.value();
        return FINFUNS_CODE_SUCCESS;
    }
    auto error = result.error();
    return make_error_code(error);
}

FINFUNSLIB_EXPORT FinFunsCode finfuns_xnpv(
    FinFunsDayCount day_count,
    double rate,
    const double * cashflows,
    const int * dates,
    unsigned num_cashflows,
    double * out_result) noexcept
{
    const auto cf_span = std::span(cashflows, num_cashflows);
    const auto date_span = std::span(dates, num_cashflows);
    auto result = [&]() -> std::expected<double, XNPVError>
    {
        switch (day_count)
        {
            case FinFunsDayCount::FINFUNS_ACT_365F:
                return xnpv<DayCountConvention::ACT_365F>(rate, cf_span, date_span);
            case FinFunsDayCount::FINFUNS_ACT_365_25:
                return xnpv<DayCountConvention::ACT_365_25>(rate, cf_span, date_span);
            default:
                [[unlikely]] return std::unexpected(XNPVError::UnsupportedDayCountConvention);
        }
    }();
    if (result.has_value()) [[likely]]
    {
        *out_result = result.value();
        return FINFUNS_CODE_SUCCESS;
    }
    auto error = result.error();
    return make_error_code(error);
}

FINFUNSLIB_EXPORT FinFunsCode finfuns_xirr(
    FinFunsDayCount day_count,
    const double * cashflows,
    const int * dates,
    unsigned num_cashflows,
    double guess,
    double * out_result) noexcept
{
    const auto cf_span = std::span(cashflows, num_cashflows);
    const auto date_span = std::span(dates, num_cashflows);
    auto result = [&]() -> std::expected<double, XIRRError>
    {
        switch (day_count)
        {
            case FinFunsDayCount::FINFUNS_ACT_365F:
                return xirr<DayCountConvention::ACT_365F>(cf_span, date_span, guess);
            case FinFunsDayCount::FINFUNS_ACT_365_25:
                return xirr<DayCountConvention::ACT_365_25>(cf_span, date_span, guess);
            default:
                [[unlikely]] return std::unexpected(XIRRErrorCode::UnsupportedDayCountConvention);
        }
    }();
    if (result.has_value()) [[likely]]
    {
        *out_result = result.value();
        return FINFUNS_CODE_SUCCESS;
    }
    auto error = result.error();
    auto error_code = std::visit([](auto e) { return make_error_code(e); }, error);
    return error_code;
}

#ifdef __cplusplus
}
#endif
