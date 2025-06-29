// finfuns library
//
//  Copyright Joanna Hulboj 2025. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <finfuns/irr.hpp>

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
    std::vector<double> cashflows;
    std::optional<double> guess;
    std::expected<double, IRRError> expected_result;
};

const std::vector<TestData> irr_cases = {
    {100, {-1000, 300, 300, 300, 300}, 0.1, 0.07713847295208363},
    {101, {-5000, 1500, 1500, 1500, 1500}, std::nullopt, 0.07713847295208363},
    {102, {-2000, 1000, 800, 600, 200}, 0.05, 0.14488844278585591},
    {103, {-100, 20, 40, 50, 30}, std::nullopt, 0.13924615432257503},
    {104, {-1000, 0, 0, 0, 1200}, 0.0, 0.04663513939210552},
    {105, {-100, 50, 50, 50, -10}, 0.1, 0.2033837618800027},
    {106, {-100, 50, 50, 50, 50}, 1000.0, 0.3490344565611563},
    {107, {-70000, 12000, 15000, 18000, 21000, 26000}, 0.1, 0.0866309480365315},

    {1001, {1000}, std::nullopt, std::unexpected(IRRErrorCode::NotEnoughCashflows)},
    {1002, {100, 200, 300}, std::nullopt, std::unexpected(IRRErrorCode::SameSignCashflows)},
    {1003, {-100, -200, -300}, 0.1, std::unexpected(IRRErrorCode::SameSignCashflows)},
    {1004, {}, std::nullopt, std::unexpected(IRRErrorCode::NotEnoughCashflows)},
};
}

TEST_CASE("irr")
{
    for (const auto & test : irr_cases)
    {
        CAPTURE(test.id);
        const auto result = irr(test.cashflows, test.guess);

        if (test.expected_result.has_value())
        {
            REQUIRE(result.has_value());
            CHECK(result.value() == doctest::Approx(test.expected_result.value()).epsilon(1e-6));
        }
        else
        {
            REQUIRE(!result.has_value());
            CHECK(result.error() == test.expected_result.error());
        }
    }
}
