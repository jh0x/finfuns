// finfuns library
//
//  Copyright Joanna Hulboj 2025. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see http://www.boost.org/libs/assign/
//

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
    DayCountConvention day_count;
    double rate;
    std::vector<double> cashflows;
    std::vector<std::chrono::sys_days> dates;
    XNPVError expected_result;
};

const std::vector<ErrorTestData> error_cases
    = {{100,
        DayCountConvention::ACT_365F,
        std::numeric_limits<double>::quiet_NaN(),
        {-1000.0, 200.0},
        {2024y / 1 / 1, 2024y / 7 / 1},
        XNPVError::InvalidRate},
       {101,
        DayCountConvention::ACT_365F,
        std::numeric_limits<double>::infinity(),
        {-1000.0, 200.0},
        {2024y / 1 / 1, 2024y / 7 / 1},
        XNPVError::InvalidRate},
       {102,
        DayCountConvention::ACT_365F,
        -std::numeric_limits<double>::infinity(),
        {-1000.0, 200.0},
        {2024y / 1 / 1, 2024y / 7 / 1},
        XNPVError::InvalidRate},

       {110, DayCountConvention::ACT_365F, 0.05, {}, {}, XNPVError::EmptyCashflows},

       {120,
        DayCountConvention::ACT_365F,
        0.05,
        {-1000.0, 200.0, 300.0},
        {2024y / 1 / 1, 2024y / 7 / 1},
        XNPVError::CashflowsDatesSizeMismatch},
       {121,
        DayCountConvention::ACT_365F,
        0.05,
        {-1000.0, 200.0},
        {2024y / 1 / 1, 2024y / 7 / 1, 2025y / 1 / 1},
        XNPVError::CashflowsDatesSizeMismatch}};

struct TestData
{
    int id;
    DayCountConvention day_count;
    double rate;
    std::vector<double> cashflows;
    std::vector<std::chrono::sys_days> dates;
    double expected_result;
};

const std::vector<TestData> xnpv_cases
    = {{1,
        DayCountConvention::ACT_365F,
        0.05,
        {-1000.0, 200.0, 300.0, 600.0},
        {2024y / 1 / 1, 2024y / 7 / 1, 2025y / 1 / 1, 2025y / 7 / 1},
        38.56361367825309},
       {2, DayCountConvention::ACT_365F, 0.07, {-1000.0, 1000.0}, {2024y / 1 / 1, 2024y / 1 / 1}, 0.}};

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

TEST_CASE("xnpv")
{
    for (const auto & test : xnpv_cases)
    {
        CAPTURE(test.id);
        const auto dates = std::span(test.dates.data(), test.dates.size());
        const auto result = xnpv<DayCountConvention::ACT_365F>(test.rate, test.cashflows, dates);

        REQUIRE(result.has_value());
        if (std::isinf(test.expected_result))
            CHECK(std::isinf(result.value()));
        else
            CHECK(result.value() == doctest::Approx(test.expected_result).epsilon(1e-6));
    }
}
