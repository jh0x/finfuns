// finfuns library
//
//  Copyright Joanna Hulboj 2025. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <finfuns/npv.hpp>

#include <expected>
#include <limits>
#include <vector>

#include <doctest/doctest.h>

using namespace finfuns;

namespace
{

struct TestData
{
    int id;
    IndexMode mode;
    double rate;
    std::vector<double> cashflows;
    std::expected<double, NPVError> expected_result;
};

const std::vector<TestData> npv_cases
    = {{1, IndexMode::ZeroBased, std::numeric_limits<double>::quiet_NaN(), {100.0}, std::unexpected(NPVError::InvalidRate)},
       {2, IndexMode::OneBased, std::numeric_limits<double>::quiet_NaN(), {100.0}, std::unexpected(NPVError::InvalidRate)},
       {3, IndexMode::ZeroBased, std::numeric_limits<double>::infinity(), {100.0}, std::unexpected(NPVError::InvalidRate)},
       {4, IndexMode::OneBased, std::numeric_limits<double>::infinity(), {100.0}, std::unexpected(NPVError::InvalidRate)},
       {5, IndexMode::ZeroBased, 0.08, {}, std::unexpected(NPVError::EmptyCashflows)},
       {6, IndexMode::OneBased, 0.08, {}, std::unexpected(NPVError::EmptyCashflows)},
       {100, IndexMode::ZeroBased, 0.08, {100.0}, 100.0}, // NPV = 100.0 (t=0)
       {101, IndexMode::OneBased, 0.08, {100.0}, 100.0 / 1.08}, // NPV = 100.0 discounted (t=1)
       {200, IndexMode::ZeroBased, 0.1, {-10000.0, 3000.0, 4200.0, 6800.0}, 1307.287753568743},
       {201, IndexMode::OneBased, 0.1, {-10000.0, 3000.0, 4200.0, 6800.0}, 1188.4434123352207},
       {300, IndexMode::ZeroBased, 0.08, {8000., 9200., 10000., 12000., 14500.}, 45275.82647932696},
       {301, IndexMode::OneBased, 0.08, {8000., 9200., 10000., 12000., 14500.}, 41922.06155493236},
       {401, IndexMode::ZeroBased, -1.8, {8000., 9200., 10000., 12000., 14500.}, std::numeric_limits<double>::infinity()},
       {402, IndexMode::OneBased, -1.0, {8000., 9200., 10000., 12000., 14500.}, std::numeric_limits<double>::infinity()}};

}

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
            REQUIRE(!result.has_value());
            CHECK(result.error() == test.expected_result.error());
        }
    }
}
