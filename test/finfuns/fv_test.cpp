// finfuns library
//
//  Copyright Joanna Hulboj 2025. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <finfuns/fv.hpp>

#include "../test_data.hpp"

#include <doctest/doctest.h>

using namespace finfuns;
using namespace finfuns::test::fv;

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
