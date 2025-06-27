#pragma once

// finfuns library
//
//  Copyright Joanna Hulboj 2025. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <cstdint>
#include <string_view>

namespace finfuns
{

enum class DayCountConvention : int32_t
{
    ACT_365F,
    ACT_365_25,
};

inline constexpr std::string_view day_count_to_string(DayCountConvention dcc)
{
    switch (dcc)
    {
        case DayCountConvention::ACT_365F:
            return "ACT/365F";
        case DayCountConvention::ACT_365_25:
            return "ACT/365.25";
        default:
            return "Unknown";
    }
}

inline constexpr int days_between_act(int d1, int d2)
{
    return d2 - d1;
}

template <DayCountConvention dcc, typename D>
inline constexpr double year_fraction(int d1, int d2)
{
    if constexpr (dcc == DayCountConvention::ACT_365F)
        return days_between_act(d1, d2) / 365.0;
    else if constexpr (dcc == DayCountConvention::ACT_365_25)
        return days_between_act(d1, d2) / 365.25;
    else
        []<bool flag = false>() { static_assert(flag, "Unsupported DayCountConvention"); }();
}

}
