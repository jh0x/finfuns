// finfuns library
//
//  Copyright Joanna Hulboj 2025. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

// NOLINTBEGIN(clang-analyzer-cplusplus.NewDeleteLeaks)
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <finfunslib/finfunslib.h>

#include "../day_count_helper.hpp"
#include "../test_data.hpp"

TEST_CASE("pv lib")
{
    using namespace finfuns::test::pv;
    for (const auto & test : pv_cases)
    {
        CAPTURE(test.id);
        const auto result = (test.mode == PaymentDueType::EndOfPeriod) ? finfuns_pv_due_end(test.rate, test.periods, test.pmt, test.fv)
                                                                       : finfuns_pv_due_begin(test.rate, test.periods, test.pmt, test.fv);

        if (std::isnan(test.expected_result))
            CHECK(std::isnan(result));
        else
            CHECK(result == doctest::Approx(test.expected_result).epsilon(1e-6));
    }
}

TEST_CASE("fv_lib")
{
    using namespace finfuns::test::fv;
    for (const auto & test : fv_cases)
    {
        CAPTURE(test.id);
        const auto result = (test.mode == PaymentDueType::EndOfPeriod) ? finfuns_fv_due_end(test.rate, test.periods, test.pmt, test.pv)
                                                                       : finfuns_fv_due_begin(test.rate, test.periods, test.pmt, test.pv);

        if (std::isnan(test.expected_result))
            CHECK(std::isnan(result));
        else
            CHECK(result == doctest::Approx(test.expected_result).epsilon(1e-6));
    }
}


TEST_CASE("npv_lib")
{
    using namespace finfuns::test::npv;
    for (const auto & test : npv_cases)
    {
        CAPTURE(test.id);
        double value;
        const auto rc = (test.mode == IndexMode::ZeroBased) ? finfuns_npv(
                                                                  FinFunsIndexMode::FINFUNS_ZERO_BASED,
                                                                  test.rate,
                                                                  test.cashflows.data(),
                                                                  static_cast<unsigned>(test.cashflows.size()),
                                                                  &value)
                                                            : finfuns_npv(
                                                                  FinFunsIndexMode::FINFUNS_ONE_BASED,
                                                                  test.rate,
                                                                  test.cashflows.data(),
                                                                  static_cast<unsigned>(test.cashflows.size()),
                                                                  &value);

        if (test.expected_result.has_value())
        {
            REQUIRE(rc == FinFunsCode::FINFUNS_CODE_SUCCESS);
            if (std::isinf(test.expected_result.value()))
                CHECK(std::isinf(value));
            else
                CHECK(value == doctest::Approx(test.expected_result.value()).epsilon(1e-6));
        }
        else
        {
            REQUIRE(rc != FinFunsCode::FINFUNS_CODE_SUCCESS);
            // TODO: Check codes
        }
    }
}

TEST_CASE("irr_lib")
{
    using namespace finfuns::test::irr;
    for (const auto & test : irr_cases)
    {
        CAPTURE(test.id);
        std::span<const double> cashflows = test.cashflows;
        double value;
        const auto rc = finfuns_irr(cashflows.data(), static_cast<unsigned>(cashflows.size()), test.guess.value_or(0.1), &value);

        if (test.expected_result.has_value())
        {
            REQUIRE(rc == FinFunsCode::FINFUNS_CODE_SUCCESS);
            CHECK(value == doctest::Approx(test.expected_result.value()).epsilon(1e-6));
        }
        else
        {
            REQUIRE(rc != FinFunsCode::FINFUNS_CODE_SUCCESS);
            // TODO: Check codes
        }
    }
}

TEST_CASE("xnpv_lib")
{
    using namespace finfuns::test::xnpv;
    for (const auto & test : xnpv_cases)
    {
        CAPTURE(test.id);
        std::span<const double> cashflows = test.cashflows;
        auto int_dates = IntDates::process(test.dates);
        std::span<const int> dates = int_dates;
        const auto rate = test.rate;
        for (std::size_t i = 0; i != test.expected_results.size(); ++i)
        {
            CAPTURE(i);
            const auto & ev = test.expected_results[i];
            const auto day_count = ev.day_count;
            const double expected_result = ev.value;
            double value;
            const auto rc = [&]
            {
                switch (day_count)
                {
                    case DayCountConvention::ACT_365F:
                        return finfuns_xnpv(
                            FinFunsDayCount::FINFUNS_ACT_365F,
                            rate,
                            cashflows.data(),
                            dates.data(),
                            static_cast<unsigned>(cashflows.size()),
                            &value);
                    case DayCountConvention::ACT_365_25:
                        return finfuns_xnpv(
                            FinFunsDayCount::FINFUNS_ACT_365_25,
                            rate,
                            cashflows.data(),
                            dates.data(),
                            static_cast<unsigned>(cashflows.size()),
                            &value);
                }
                REQUIRE(false);
                throw std::logic_error("Unexpected day count");
            }();
            REQUIRE(rc == FinFunsCode::FINFUNS_CODE_SUCCESS);
            if (std::isinf(expected_result))
                CHECK(std::isinf(value));
            else
                CHECK(value == doctest::Approx(expected_result).epsilon(1e-6));
        }
    }

    for (const auto & test : error_cases)
    {
        if (test.expected_result == XNPVError::CashflowsDatesSizeMismatch)
            continue; // We don't check this - the API requires cashflows + data ptr + one size right now
        CAPTURE(test.id);
        std::span<const double> cashflows = test.cashflows;
        auto int_dates = IntDates::process(test.dates);
        std::span<const int> dates = int_dates;
        const auto rate = test.rate;
        double value;
        const auto rc = finfuns_xnpv(
            FinFunsDayCount::FINFUNS_ACT_365F, rate, cashflows.data(), dates.data(), static_cast<unsigned>(cashflows.size()), &value);

        REQUIRE(rc != FinFunsCode::FINFUNS_CODE_SUCCESS);
        // TODO: Check codes
    }
}

TEST_CASE("xirr_lib")
{
    using namespace finfuns::test::xirr;
    for (const auto & test : xirr_cases)
    {
        CAPTURE(test.id);
        std::span<const double> cashflows = test.cashflows;
        auto int_dates = IntDates::process(test.dates);
        std::span<const int> dates = int_dates;
        for (std::size_t i = 0; i != test.expected_results.size(); ++i)
        {
            CAPTURE(i);
            const auto & ev = test.expected_results[i];
            const auto day_count = ev.day_count;
            const double expected_result = ev.value;
            double value;
            const auto rc = [&]
            {
                switch (day_count)
                {
                    case DayCountConvention::ACT_365F:
                        return finfuns_xirr(
                            FinFunsDayCount::FINFUNS_ACT_365F,
                            cashflows.data(),
                            dates.data(),
                            static_cast<unsigned>(cashflows.size()),
                            0.1,
                            &value);
                    case DayCountConvention::ACT_365_25:
                        return finfuns_xirr(
                            FinFunsDayCount::FINFUNS_ACT_365_25,
                            cashflows.data(),
                            dates.data(),
                            static_cast<unsigned>(cashflows.size()),
                            0.1,
                            &value);
                }
                REQUIRE(false);
                throw std::logic_error("Unexpected day count");
            }();
            REQUIRE(rc == FinFunsCode::FINFUNS_CODE_SUCCESS);
            if (std::isinf(expected_result))
                CHECK(std::isinf(value));
            else
                CHECK(value == doctest::Approx(expected_result).epsilon(1e-6));
        }
    }

    for (const auto & test : error_cases)
    {
        if (test.expected_result == XIRRError{XIRRErrorCode::CashflowsDatesSizeMismatch})
            continue; // We don't check this - the API requires cashflows + data ptr + one size right now
        CAPTURE(test.id);
        std::span<const double> cashflows = test.cashflows;
        auto int_dates = IntDates::process(test.dates);
        std::span<const int> dates = int_dates;
        double value;
        const auto rc = finfuns_xirr(
            FinFunsDayCount::FINFUNS_ACT_365F, cashflows.data(), dates.data(), static_cast<unsigned>(cashflows.size()), 0.1, &value);

        REQUIRE(rc != FinFunsCode::FINFUNS_CODE_SUCCESS);
        // TODO: Check codes
    }
}
// NOLINTEND(clang-analyzer-cplusplus.NewDeleteLeaks)
