// finfuns library
//
//  Copyright Joanna Hulboj 2025. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \example ex_xirr.cpp
/// This is an example demonstrating the use of finfuns::xirr.
///
/// Expected output:
/// \code
/// The XIRR is 37.34%
/// \endcode

#include <finfuns/xirr.hpp>

#include <chrono>
#include <format>
#include <iostream>
#include <vector>

using namespace finfuns;

int main()
{
    using namespace std::chrono_literals;

    std::vector<double> cashflows = {-10000, 2750, 4250, 3250, 2750};
    // Could also be a vector<int> with days since epoch
    std::vector<std::chrono::sys_days> dates = {2008y / 1 / 1, 2008y / 3 / 1, 2008y / 10 / 30, 2009y / 2 / 15, 2009y / 4 / 1};
    auto guess = std::nullopt;

    auto result = xirr<DayCountConvention::ACT_365F>(cashflows, dates, guess);
    if (result.has_value())
    {
        std::println(std::cout, "The XIRR is {:.2f}%", result.value() * 100.);
    }
    else
    {
        std::println(std::cout, "XIRR problem: {}", error_to_sv(result.error()));
    }

    return 0;
}
