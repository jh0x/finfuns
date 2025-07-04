# FinFuns

A collection of financial functions that I wrote while adding support for
them to
[ClickHouse (PR #81599)](https://github.com/ClickHouse/ClickHouse/pull/81599).

Header only library is provided in `include/finfuns` (`finfuns::finfuns` in `CMakeLists`)

Examples are provided in `examples`.

## Quick Start

- Use `finfuns::finfuns` CMake target in your project...
- or directly include the headers from `include/finfuns`

Example usage:

```cpp
#include <finfuns/xirr.hpp>
using namespace finfuns;
//...
std::vector<double> cashflows = {-10000, 2750, 4250, 3250, 2750};
std::vector<std::chrono::sys_days> dates = {2008y / 1 / 1, 2008y / 3 / 1, 2008y / 10 / 30, 2009y / 2 / 15, 2009y / 4 / 1};
auto guess = std::nullopt;
auto result = xirr<DayCountConvention::ACT_365F>(cashflows, dates, guess);
if (result.has_value()) [[likely]]
    // use result.value()
else
    // use result.error(), e.g. error_to_sv(result.error())
```

## Development

```sh
cmake -B build -Dfinfuns_DEVELOPER_MODE=TRUE -Dfinfuns_BUILD_EXAMPLES=TRUE
cmake --build build
```

## Quick API reference

### `npv` (Net Present Value)

```cpp
template <IndexMode index_mode>
std::expected<double, NPVError> npv(double rate, std::span<const double> cashflows)
```

Calculates the Net Present Value (NPV) of a series of cash flows assuming equal time intervals between each cash flow.

- `IndexMode::ZeroBased` variant:

$$
\sum_{i=0}^{N-1} \frac{values_i}{(1 + rate)^i}
$$

- Excel-compatible variant `IndexMode::OneBased`:

$$
\sum_{i=1}^{N} \frac{values_i}{(1 + rate)^i}
$$

### `irr` (Internal Rate of Return)

```cpp
std::expected<double, IRRError> irr(std::span<const double> cashflows, std::optional<double> guess)
```

Calculates the Internal Rate of Return (IRR) for a series of cash flows occurring at regular intervals. IRR is the discount rate at which the Net Present Value (NPV) equals zero.

IRR attempts to solve the following equation:

$$
\sum_{i=0}^n \frac{cashflow_i}{(1 + irr)^i} = 0
$$

### `xnpv` (Extended Net Present Value)

```cpp
template <DayCountConvention day_count, typename DateContainer>
std::expected<double, XNPVError> xnpv(double rate, std::span<const double> cashflows, DateContainer && dates)

// dates can be std::chrono::sys_days or int (e.g. days since epoch)
```

Calculates the Extended Net Present Value (XNPV) for a series of cash flows occurring at irregular intervals. XNPV considers the specific timing of each cash flow when calculating present value.

XNPV equation for `ACT_365F`:

$$
XNPV=\sum_{i=1}^n \frac{cashflow_i}{(1 + rate)^{(date_i - date_0)/365}}
$$

### `xirr` (Extended Internal Rate of Return)

```cpp
template <DayCountConvention day_count, typename DateContainer>
std::expected<double, XIRRError> xirr(std::span<const double> cashflows, DateContainer && dates, std::optional<double> guess)

// dates can be std::chrono::sys_days or int (e.g. days since epoch)
```

Calculates the Extended Internal Rate of Return (XIRR) for a series of cash flows occurring at irregular intervals. XIRR is the discount rate at which the net present value (NPV) of all cash flows equals zero.

XIRR attempts to solve the following equation (example for `ACT_365F`):

$$
\sum_{i=0}^n \frac{cashflow_i}{(1 + rate)^{(date_i - date_0)/365}} = 0
$$

### Supported `DayCountConventions`

```cpp
enum class DayCountConvention : int32_t
{
    ACT_365F,
    ACT_365_25,
};
```
