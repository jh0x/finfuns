// finfuns library
//
//  Copyright Joanna Hulboj 2025. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include "day_count_helper.hpp"

#include <finfuns/xnpv.hpp>

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
    double rate;
    std::vector<double> cashflows;
    std::vector<std::chrono::sys_days> dates;
    XNPVError expected_result;
};

const std::vector<ErrorTestData> error_cases
    = {{100, std::numeric_limits<double>::quiet_NaN(), {-1000.0, 200.0}, {2024y / 1 / 1, 2024y / 7 / 1}, XNPVError::InvalidRate},
       {101, std::numeric_limits<double>::infinity(), {-1000.0, 200.0}, {2024y / 1 / 1, 2024y / 7 / 1}, XNPVError::InvalidRate},
       {102, -std::numeric_limits<double>::infinity(), {-1000.0, 200.0}, {2024y / 1 / 1, 2024y / 7 / 1}, XNPVError::InvalidRate},

       {110, 0.05, {}, {}, XNPVError::EmptyCashflows},

       {120, 0.05, {-1000.0, 200.0, 300.0}, {2024y / 1 / 1, 2024y / 7 / 1}, XNPVError::CashflowsDatesSizeMismatch},
       {121, 0.05, {-1000.0, 200.0}, {2024y / 1 / 1, 2024y / 7 / 1, 2025y / 1 / 1}, XNPVError::CashflowsDatesSizeMismatch}};

struct ExpectedResult
{
    DayCountConvention day_count;
    double value;
};

struct TestData
{
    int id;
    double rate;
    std::vector<double> cashflows;
    std::vector<std::chrono::sys_days> dates;
    std::vector<ExpectedResult> expected_results;
};

const std::vector<TestData> xnpv_cases
    = {{1,
        0.05,
        {-1000.0, 200.0, 300.0, 600.0},
        {2024y / 1 / 1, 2024y / 7 / 1, 2025y / 1 / 1, 2025y / 7 / 1},
        {{DayCountConvention::ACT_365F, 38.56361367825309}, {DayCountConvention::ACT_365_25, 38.60434201600265}}},
       {2, 0.07, {-1000.0, 1000.0}, {2024y / 1 / 1, 2024y / 1 / 1}, {{DayCountConvention::ACT_365F, 0.}}},
       {3,
        0.09,
        {-10000.0, 2750.0, 4250.0, 3250.0, 2750.0},
        {2008y / 1 / 1, 2008y / 3 / 1, 2008y / 10 / 30, 2009y / 2 / 15, 2009y / 4 / 1},
        {{DayCountConvention::ACT_365F, 2086.647602031535}, {DayCountConvention::ACT_365_25, 2087.245513772147}}},
       {4,
        0.1,
        {-10000.0, 5750.0, 4250.0, 3250.0},
        {2020y / 1 / 1, 2020y / 3 / 1, 2020y / 10 / 30, 2021y / 2 / 15},
        {{DayCountConvention::ACT_365F, 2506.579458169746}, {DayCountConvention::ACT_365_25, 2507.067268742502}}},
       {5,
        -1.,
        {-10000.0, 5750.0, 4250.0, 3250.0},
        {2020y / 1 / 1, 2020y / 3 / 1, 2020y / 10 / 30, 2021y / 2 / 15},
        {{DayCountConvention::ACT_365F, std::numeric_limits<double>::infinity()},
         {DayCountConvention::ACT_365_25, std::numeric_limits<double>::infinity()}}}};

}


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
        const auto day_count = test.expected_results[0].day_count;
        const double expected_result = test.expected_results[0].value;
        const auto result = invoke_xnpv(day_count, test.rate, test.cashflows, dates_span);

        REQUIRE(result.has_value());
        if (std::isinf(expected_result))
            CHECK(std::isinf(result.value()));
        else
            CHECK(result.value() == doctest::Approx(expected_result).epsilon(1e-6));
    }
}
