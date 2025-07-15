// finfuns library
//
//  Copyright Joanna Hulboj 2025. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <finfuns/irr.hpp>

#include "../test_data.hpp"

#include <iostream>
#include <doctest/doctest.h>

using namespace finfuns;
using namespace finfuns::test::irr;

TEST_CASE("irr")
{
    for (const auto & test : irr_cases)
    {
        CAPTURE(test.id);
        const auto result = irr(test.cashflows, test.guess);

        if (test.expected_result.has_value())
        {
            if (!result.has_value())
            {
                std::cerr << error_to_sv(result.error()) << std::endl;
            }
            REQUIRE(result.has_value());
            CHECK(result.value() == doctest::Approx(test.expected_result.value()).epsilon(1e-6));
        }
        else
        {
            REQUIRE(not result.has_value());
            CHECK(result.error() == test.expected_result.error());
        }
    }
}
