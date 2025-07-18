// finfuns library
//
//  Copyright Joanna Hulboj 2025. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \example ex_npv.cpp
/// This is an example demonstrating the use of finfuns::npv.
///
/// Expected output:
/// \code
/// The (zero-based) NPV is 1307.29%
/// The (one-based) NPV is 1188.44%
/// \endcode

#include <finfuns/npv.hpp>

#include <iomanip>
#include <iostream>
#include <vector>

using namespace finfuns;

int main()
{
    std::vector<double> cashflows = {-10000.0, 3000.0, 4200.0, 6800.0};
    auto rate = 0.1;

    {
        auto result_zero_based = npv<IndexMode::ZeroBased>(rate, cashflows);
        if (result_zero_based.has_value())
        {
            std::cout << "The (zero-based) NPV is " << std::fixed << std::setprecision(2) << result_zero_based.value() << "\n";
        }
        else
        {
            std::cout << "NPV problem: " << error_to_sv(result_zero_based.error()) << "\n";
        }
    }
    {
        auto result_one_based = npv<IndexMode::OneBased>(rate, cashflows);
        if (result_one_based.has_value())
        {
            std::cout << "The (one-based) NPV is " << std::fixed << std::setprecision(2) << result_one_based.value() << "\n";
        }
        else
        {
            std::cout << "NPV problem: " << error_to_sv(result_one_based.error()) << "\n";
        }
    }

    return 0;
}
