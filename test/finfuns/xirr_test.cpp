// finfuns library
//
//  Copyright Joanna Hulboj 2025. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include "../day_count_helper.hpp"
#include "../test_data.hpp"

#include <finfuns/xirr.hpp>

#include <doctest/doctest.h>

using namespace finfuns;
using namespace finfuns::test::xirr;

template <typename DateType>
static constexpr auto invoke_xirr(DayCountConvention dcc, std::span<const double> cashflows, std::span<const DateType> dates)
{
    switch (dcc)
    {
        case DayCountConvention::ACT_365F:
            return xirr<DayCountConvention::ACT_365F>(cashflows, dates, std::nullopt);
        case DayCountConvention::ACT_365_25:
            return xirr<DayCountConvention::ACT_365_25>(cashflows, dates, std::nullopt);
        default:
            throw std::invalid_argument("Unsupported DayCountConvention");
    }
}

TEST_CASE("xirr_error_cases")
{
    for (const auto & test : error_cases)
    {
        CAPTURE(test.id);
        const auto dates = std::span(test.dates.data(), test.dates.size());
        const auto result = xirr<DayCountConvention::ACT_365F>(test.cashflows, dates, std::nullopt);

        REQUIRE(not result.has_value());
        CHECK(result.error() == test.expected_result);
    }
}


DOCTEST_TEST_CASE_TEMPLATE("XIRR", T, SysDates, IntDates)
{
    for (const auto & test : xirr_cases)
    {
        CAPTURE(test.id);
        const auto dates = T::process(test.dates);
        const auto dates_span = std::span(dates.data(), dates.size());
        const auto day_count = test.expected_results[0].day_count;
        const double expected_result = test.expected_results[0].value;
        const auto result = invoke_xirr(day_count, test.cashflows, dates_span);

        REQUIRE(result.has_value());
        if (std::isinf(expected_result))
            CHECK(std::isinf(result.value()));
        else
            CHECK(result.value() == doctest::Approx(expected_result).epsilon(1e-6));
    }
}
