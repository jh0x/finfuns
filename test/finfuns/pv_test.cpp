// finfuns library
//
//  Copyright Joanna Hulboj 2025. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <finfuns/pv.hpp>

#include <limits>
#include <vector>

#include <doctest/doctest.h>

using namespace finfuns;

namespace
{

struct TestData
{
    int id;
    PaymentDueType mode;
    double rate;
    uint32_t periods;
    double pmt;
    double fv;
    double expected_result;
};

const std::vector<TestData> pv_cases = {
    // Basic cases (End of Period)
    {1, PaymentDueType::EndOfPeriod, 0.05, 10, -100.0, 0.0, 772.173492918482},
    {2, PaymentDueType::EndOfPeriod, 0.10, 5, -500.0, 1000.0, 1274.47206164507},

    // Beginning of Period cases
    {3, PaymentDueType::BeginningOfPeriod, 0.05, 10, -100.0, 0.0, 810.782167564406},
    {4, PaymentDueType::BeginningOfPeriod, 0.075, 15, -200.0, 500.0, 1728.84773562176},

    // Zero interest rate cases
    {5, PaymentDueType::EndOfPeriod, 0.0, 10, -100.0, 0.0, 1000},
    {6, PaymentDueType::BeginningOfPeriod, 0.0, 5, -200.0, 1000.0, 0},

    // Edge cases
    {7, PaymentDueType::EndOfPeriod, 0.000001, 1, -1000.0, 0.0, 999.998999939855},
    {8, PaymentDueType::BeginningOfPeriod, 0.50, 3, -100.0, 0.0, 211.111111111111},

    // Single period cases
    {9, PaymentDueType::EndOfPeriod, 0.10, 1, -100.0, 0.0, 90.9090909090909},
    {10, PaymentDueType::BeginningOfPeriod, 0.10, 1, -100.0, 0.0, 100},

    // Large numbers
    {11, PaymentDueType::EndOfPeriod, 0.03, 360, -1000.0, 0.0, 33332.5362974918},

    // Negative cases
    {12, PaymentDueType::EndOfPeriod, -0.02, 10, -100.0, 0.0, 1119.40571005706},

    {13, PaymentDueType::EndOfPeriod, 0.07 / 12, 36, -299.0, 0.0, 9683.55287103833},
    {14, PaymentDueType::BeginningOfPeriod, 0.06 / 12, 60, -500.0, 10000.0, 18578.3723149999},

    // Invalid rates
    {100, PaymentDueType::BeginningOfPeriod, -1, 60, -500.0, 10000.0, std::numeric_limits<double>::quiet_NaN()},
    {101, PaymentDueType::EndOfPeriod, -1, 60, -500.0, 10000.0, std::numeric_limits<double>::quiet_NaN()}};
}

TEST_CASE("pv")
{
    for (const auto & test : pv_cases)
    {
        CAPTURE(test.id);
        const auto result = (test.mode == PaymentDueType::EndOfPeriod)
            ? pv<PaymentDueType::EndOfPeriod>(test.rate, test.periods, test.pmt, test.fv)
            : pv<PaymentDueType::BeginningOfPeriod>(test.rate, test.periods, test.pmt, test.fv);

        if (std::isnan(test.expected_result))
            CHECK(std::isnan(result));
        else
            CHECK(result == doctest::Approx(test.expected_result).epsilon(1e-6));
    }
}
