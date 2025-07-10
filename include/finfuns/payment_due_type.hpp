#pragma once

// finfuns library
//
//  Copyright Joanna Hulboj 2025. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <cstdint>

namespace finfuns
{

enum class PaymentDueType : int32_t
{
    EndOfPeriod, //!< Payment is due at the end of the period
    BeginningOfPeriod //!< Payment is due at the beginning of the period
};

}
