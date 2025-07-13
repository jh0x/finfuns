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

extern "C" {

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
}
