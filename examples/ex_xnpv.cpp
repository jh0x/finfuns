// finfuns library
//
//  Copyright Joanna Hulboj 2025. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \example ex_xnpv.cpp
/// This is an example demonstrating the use of finfuns::xnpv.
///
/// Expected output:
/// \code
/// The XNPV is 2506.58
/// \endcode

#include <finfuns/xnpv.hpp>

#include <chrono>
#include <format>
#include <iostream>
#include <vector>

using namespace finfuns;

int main()
{
    using namespace std::chrono_literals;

    std::vector<double> cashflows = {-10000.0, 5750.0, 4250.0, 3250.0};
    // Could also be a vector<int> with days since epoch
    std::vector<std::chrono::sys_days> dates = {2020y / 1 / 1, 2020y / 3 / 1, 2020y / 10 / 30, 2021y / 2 / 15};
    auto rate = 0.1;

    auto result = xnpv<DayCountConvention::ACT_365F>(rate, cashflows, dates);
    if (result.has_value())
    {
        std::println(std::cout, "The XNPV is {:.2f}", result.value());
    }
    else
    {
        std::println(std::cout, "XNPV problem: {}", error_to_sv(result.error()));
    }

    return 0;
}
