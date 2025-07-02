#pragma once
// finfuns library
//
//  Copyright Joanna Hulboj 2025. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <chrono>
#include <span>
#include <vector>

struct SysDates
{
    static constexpr std::vector<std::chrono::sys_days> process(std::span<const std::chrono::sys_days> dates)
    {
        return std::vector<std::chrono::sys_days>(dates.begin(), dates.end());
    }
};
struct IntDates
{
    static constexpr std::vector<int> process(std::span<const std::chrono::sys_days> dates)
    {
        std::vector<int> result;
        result.reserve(dates.size());
        for (const auto & date : dates)
            result.push_back(static_cast<int>(date.time_since_epoch().count()));
        return result;
    }
};
