// finfuns library
//
//  Copyright Joanna Hulboj 2025. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include "../day_count_helper.hpp"
#include "../test_data.hpp"

#include <finfuns/xnpv.hpp>

#include <doctest/doctest.h>

using namespace finfuns;
using namespace finfuns::test::xnpv;


TEST_CASE("xnpv_error_cases")
{
    for (const auto & test : error_cases)
    {
        CAPTURE(test.id);
        const auto dates = std::span(test.dates.data(), test.dates.size());
        const auto result = xnpv<DayCountConvention::ACT_365F>(test.rate, test.cashflows, dates);

        REQUIRE(not result.has_value());
        CHECK(result.error() == test.expected_result);
    }
}

template <typename DateType>
static constexpr auto invoke_xnpv(DayCountConvention dcc, double rate, std::span<const double> cashflows, std::span<const DateType> dates)
{
    switch (dcc)
    {
        case DayCountConvention::ACT_365F:
            return xnpv<DayCountConvention::ACT_365F>(rate, cashflows, dates);
        case DayCountConvention::ACT_365_25:
            return xnpv<DayCountConvention::ACT_365_25>(rate, cashflows, dates);
        default:
            throw std::invalid_argument("Unsupported DayCountConvention");
    }
}

DOCTEST_TEST_CASE_TEMPLATE("XNPV", T, SysDates, IntDates)
{
    for (const auto & test : xnpv_cases)
    {
        CAPTURE(test.id);
        const auto dates = T::process(test.dates);
        const auto dates_span = std::span(dates.data(), dates.size());
        for (std::size_t i = 0; i != test.expected_results.size(); ++i)
        {
            CAPTURE(i);
            const auto & ev = test.expected_results[i];
            const auto day_count = ev.day_count;
            const double expected_result = ev.value;
            const auto result = invoke_xnpv(day_count, test.rate, test.cashflows, dates_span);

            REQUIRE(result.has_value());
            if (std::isinf(expected_result))
                CHECK(std::isinf(result.value()));
            else
                CHECK(result.value() == doctest::Approx(expected_result).epsilon(1e-6));
        }
    }
}
