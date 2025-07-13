// finfuns library
//
//  Copyright Joanna Hulboj 2025. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include "finfunslib.h"

#include <finfuns/finfuns.hpp>
#include <finfuns/overloaded.hpp>

#include <utility>

using namespace finfuns;

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

FinFunsIRRCode finfuns_irr(const double * cashflows, int num_cashflows, double guess, double * out_result) noexcept
{
    const auto cf_span = std::span<const double>(cashflows, num_cashflows);
    auto result = irr(cf_span, guess);
    if (result.has_value()) [[likely]]
    {
        *out_result = result.value();
        return FINFUNS_IRR_SUCCESS;
    }
    auto error = result.error();
    auto error_code = std::visit(
        overloaded{
            [](IRRErrorCode e) { return static_cast<FinFunsIRRCode>(std::to_underlying(e) + 1); },
            [](SolverErrorCode e) { return static_cast<FinFunsIRRCode>(std::to_underlying(e) + 100); } // Offset for solver errors
        },
        error);
    return error_code;
}

FinFunsNPVCode finfuns_npv(FinFunsIndexMode mode, double rate, const double * cashflows, int num_cashflows, double * out_result) noexcept
{
    const auto cf_span = std::span{cashflows, static_cast<size_t>(num_cashflows)};
    auto result = (mode == FINFUNS_ZERO_BASED) ? npv<IndexMode::ZeroBased>(rate, cf_span) : npv<IndexMode::OneBased>(rate, cf_span);
    if (result.has_value()) [[likely]]
    {
        *out_result = result.value();
        return FINFUNS_NPV_SUCCESS;
    }
    auto error = result.error();
    return static_cast<FinFunsNPVCode>(std::to_underlying((error)) + 1);
}

FINFUNSLIB_EXPORT FinFunsXNPVCode finfuns_xnpv(
    FinFunsDayCount day_count,
    double rate,
    const double * cashflows,
    const int * dates,
    int32_t num_cashflows,
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
                return std::unexpected(XNPVError::UnsupportedDayCountConvention);
        }
    }();
    if (result.has_value()) [[likely]]
    {
        *out_result = result.value();
        return FINFUNS_XNPV_SUCCESS;
    }
    auto error = result.error();
    return static_cast<FinFunsXNPVCode>(std::to_underlying((error)) + 1);
}

#ifdef __cplusplus
}
#endif
