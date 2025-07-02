// finfuns library
//
//  Copyright Joanna Hulboj 2025. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include "day_count_helper.hpp"

#include <finfuns/xirr.hpp>

#include <chrono>
#include <expected>
#include <limits>
#include <vector>

#include <doctest/doctest.h>

using namespace std::chrono;
using namespace std::chrono_literals;
using namespace finfuns;

namespace
{

struct ErrorTestData
{
    int id;
    std::vector<double> cashflows;
    std::vector<std::chrono::sys_days> dates;
    XIRRError expected_result;
};

const std::vector<ErrorTestData> error_cases
    = {{100, {-1000.0}, {2024y / 1 / 1}, XIRRErrorCode::NotEnoughCashflows},
       {101, {-1000.0, 200.0}, {2024y / 1 / 1}, XIRRErrorCode::CashflowsDatesSizeMismatch},
       {102, {-1000.0, -200.0}, {2024y / 1 / 1, 2024y / 2 / 1}, XIRRErrorCode::SameSignCashflows}};

struct ExpectedResult
{
    DayCountConvention day_count;
    double value;
};

struct TestData
{
    int id;
    std::vector<double> cashflows;
    std::vector<std::chrono::sys_days> dates;
    std::vector<ExpectedResult> expected_results;
};

const std::vector<TestData> xirr_cases
    = {{1,
        {-10000, 5750, 4250, 3250},
        {2020y / 1 / 1, 2020y / 3 / 1, 2020y / 10 / 30, 2021y / 2 / 15},
        {{DayCountConvention::ACT_365F, 0.6342972615260243}, {DayCountConvention::ACT_365_25, 0.6348472087619019}}},
       {2,
        {100000, -110000},
        {2020y / 1 / 1, 2021y / 1 / 1},
        {{DayCountConvention::ACT_365F, 0.09971358593414113}, {DayCountConvention::ACT_365_25, 0.09978518245839686}}},
       {3,
        {-10000, 2750, 4250, 3250, 2750},
        {2008y / 1 / 1, 2008y / 3 / 1, 2008y / 10 / 30, 2009y / 2 / 15, 2009y / 4 / 1},
        {{DayCountConvention::ACT_365F, 0.37336253351883136}, {DayCountConvention::ACT_365_25, 0.3736610015164226}}}};

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
