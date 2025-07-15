// finfuns library
//
//  Copyright Joanna Hulboj 2025. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <finfuns/npv.hpp>

#include "../test_data.hpp"

#include <doctest/doctest.h>

using namespace finfuns;
using namespace finfuns::test::npv;

TEST_CASE("npv")
{
    for (const auto & test : npv_cases)
    {
        CAPTURE(test.id);
        const auto result = (test.mode == IndexMode::ZeroBased) ? npv<IndexMode::ZeroBased>(test.rate, test.cashflows)
                                                                : npv<IndexMode::OneBased>(test.rate, test.cashflows);

        if (test.expected_result.has_value())
        {
            REQUIRE(result.has_value());
            if (std::isinf(test.expected_result.value()))
                CHECK(std::isinf(result.value()));
            else
                CHECK(result.value() == doctest::Approx(test.expected_result.value()).epsilon(1e-6));
        }
        else
        {
            REQUIRE(not result.has_value());
            CHECK(result.error() == test.expected_result.error());
        }
    }
}
