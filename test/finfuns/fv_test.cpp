// finfuns library
//
//  Copyright Joanna Hulboj 2025. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <finfuns/fv.hpp>

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
    double pv;
    double expected_result;
};

const std::vector<TestData> fv_cases = {
    // Basic cases (End of Period)
    {1, PaymentDueType::EndOfPeriod, 0.05, 10, -100.0, 0.0, 1257.78925355488},
    {2, PaymentDueType::EndOfPeriod, 0.10, 5, -500.0, 1000.0, 1442.04},

    // Beginning of Period cases
    {3, PaymentDueType::BeginningOfPeriod, 0.05, 10, -100.0, 0.0, 1320.67871623263},
    {4, PaymentDueType::BeginningOfPeriod, 0.075, 15, -200.0, 500.0, 4136.00973498246},

    // Zero interest rate cases
    {5, PaymentDueType::EndOfPeriod, 0.0, 10, -100.0, 0.0, 1000},
    {6, PaymentDueType::BeginningOfPeriod, 0.0, 5, -200.0, 1000.0, 0},

    // Edge cases
    {7, PaymentDueType::EndOfPeriod, 0.000001, 1, -1000.0, 0.0, 999.999999917733},
    {8, PaymentDueType::BeginningOfPeriod, 0.50, 3, -100.0, 0.0, 712.5},

    // Single period cases
    {9, PaymentDueType::EndOfPeriod, 0.10, 1, -100.0, 0.0, 100},
    {10, PaymentDueType::BeginningOfPeriod, 0.10, 1, -100.0, 0.0, 110},

    // Large numbers
    {11, PaymentDueType::EndOfPeriod, 0.03, 360, -1000.0, 0.0, 1394020802.27669},

    // Negative cases
    {12, PaymentDueType::EndOfPeriod, -0.02, 10, -100.0, 0.0, 914.635965562267},

    {13, PaymentDueType::EndOfPeriod, 0.07 / 12, 36, -299.0, 0.0, 11939.1001123888},
    {14, PaymentDueType::BeginningOfPeriod, 0.06 / 12, 60, -500.0, 10000.0, 21570.9388057123},

    // Invalid rates
    {100, PaymentDueType::BeginningOfPeriod, -1, 60, -500.0, 10000.0, std::numeric_limits<double>::quiet_NaN()},
    {101, PaymentDueType::EndOfPeriod, -1, 60, -500.0, 10000.0, std::numeric_limits<double>::quiet_NaN()}};


}

TEST_CASE("pv")
{
    for (const auto & test : fv_cases)
    {
        CAPTURE(test.id);
        const auto result = (test.mode == PaymentDueType::EndOfPeriod)
            ? fv<PaymentDueType::EndOfPeriod>(test.rate, test.periods, test.pmt, test.pv)
            : fv<PaymentDueType::BeginningOfPeriod>(test.rate, test.periods, test.pmt, test.pv);

        if (std::isnan(test.expected_result))
            CHECK(std::isnan(result));
        else
            CHECK(result == doctest::Approx(test.expected_result).epsilon(1e-6));
    }
}
