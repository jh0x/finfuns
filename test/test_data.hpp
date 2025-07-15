#pragma once

// finfuns library
//
//  Copyright Joanna Hulboj 2025. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <finfuns/finfuns.hpp>

#include <chrono>
#include <cmath>
#include <cstdint>
#include <expected>
#include <limits>
#include <vector>

using namespace std::chrono;
using namespace std::chrono_literals;
using namespace finfuns;

namespace finfuns::test::pv
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

namespace finfuns::test::fv
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

namespace finfuns::test::npv
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

namespace finfuns::test::irr
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

namespace finfuns::test::xnpv
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

namespace finfuns::test::xirr
{

struct ErrorTestData
{
    int id;
    std::vector<double> cashflows;
    std::vector<std::chrono::sys_days> dates;
    XIRRError expected_result;
};

const std::vector<ErrorTestData> error_cases
    = {{100, {-1000.0}, {2024y / 1 / 1}, XIRRErrorCode::NotEnoughCashflows},
       {101, {-1000.0, 200.0}, {2024y / 1 / 1}, XIRRErrorCode::CashflowsDatesSizeMismatch},
       {102, {-1000.0, -200.0}, {2024y / 1 / 1, 2024y / 2 / 1}, XIRRErrorCode::SameSignCashflows}};

struct ExpectedResult
{
    DayCountConvention day_count;
    double value;
};

struct TestData
{
    int id;
    std::vector<double> cashflows;
    std::vector<std::chrono::sys_days> dates;
    std::vector<ExpectedResult> expected_results;
};

const std::vector<TestData> xirr_cases
    = {{1,
        {-10000, 5750, 4250, 3250},
        {2020y / 1 / 1, 2020y / 3 / 1, 2020y / 10 / 30, 2021y / 2 / 15},
        {{DayCountConvention::ACT_365F, 0.6342972615260243}, {DayCountConvention::ACT_365_25, 0.6348472087619019}}},
       {2,
        {100000, -110000},
        {2020y / 1 / 1, 2021y / 1 / 1},
        {{DayCountConvention::ACT_365F, 0.09971358593414113}, {DayCountConvention::ACT_365_25, 0.09978518245839686}}},
       {3,
        {-10000, 2750, 4250, 3250, 2750},
        {2008y / 1 / 1, 2008y / 3 / 1, 2008y / 10 / 30, 2009y / 2 / 15, 2009y / 4 / 1},
        {{DayCountConvention::ACT_365F, 0.37336253351883136}, {DayCountConvention::ACT_365_25, 0.3736610015164226}}}};

}
