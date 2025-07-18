// finfuns library
//
//  Copyright Joanna Hulboj 2025. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \example ex_irr.cpp
/// This is an example demonstrating the use of finfuns::irr.
///
/// Expected output:
/// \code
/// The IRR is 7.71%
/// \endcode

#include <finfuns/irr.hpp>

#include <iomanip>
#include <iostream>
#include <vector>

using namespace finfuns;

int main()
{
    std::vector<double> cashflows = {-1000, 300, 300, 300, 300};
    auto guess = 0.1;

    auto result = irr(cashflows, guess);
    if (result.has_value())
    {
        std::cout << "The IRR is " << std::fixed << std::setprecision(2) << (result.value() * 100) << "%\n";
    }
    else
    {
        std::cout << "IRR problem: " << error_to_sv(result.error()) << "\n";
    }

    return 0;
}
