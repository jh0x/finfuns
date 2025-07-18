# FinFuns

A collection of financial functions that I wrote while adding support for
them to
[ClickHouse (PR #81599)](https://github.com/ClickHouse/ClickHouse/pull/81599).

Header only library (`finfuns::headers`) is provided in `include/finfuns` (`finfuns::headers` in `CMakeLists`)

`finfuns::lib` is the compiled library component of finfuns, providing similar functionality - but behind a simple `C` style API. Internally it depends on `finfuns::headers`.

Examples are provided in `examples`.

## Quick Start

- Use `finfuns::headers` CMake target in your project...
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
finfuns::expectedcted<double, NPVError> npv(double rate, std::span<const double> cashflows)
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
finfuns::expectedcted<double, IRRError> irr(std::span<const double> cashflows, std::optional<double> guess)
```

Calculates the Internal Rate of Return (IRR) for a series of cash flows occurring at regular intervals. IRR is the discount rate at which the Net Present Value (NPV) equals zero.

IRR attempts to solve the following equation:

$$
\sum_{i=0}^n \frac{cashflow_i}{(1 + irr)^i} = 0
$$

### `xnpv` (Extended Net Present Value)

```cpp
template <DayCountConvention day_count, typename DateContainer>
finfuns::expectedcted<double, XNPVError> xnpv(double rate, std::span<const double> cashflows, DateContainer && dates)

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
finfuns::expectedcted<double, XIRRError> xirr(std::span<const double> cashflows, DateContainer && dates, std::optional<double> guess)

// dates can be std::chrono::sys_days or int (e.g. days since epoch)
```

Calculates the Extended Internal Rate of Return (XIRR) for a series of cash flows occurring at irregular intervals. XIRR is the discount rate at which the net present value (NPV) of all cash flows equals zero.

XIRR attempts to solve the following equation (example for `ACT_365F`):

$$
\sum_{i=0}^n \frac{cashflow_i}{(1 + rate)^{(date_i - date_0)/365}} = 0
$$

### `pv` (Present Value of an Annuity)

```cpp
template <PaymentDueType due_type>
double pv(double rate, uint32_t periods, double pmt, double future_value = 0.0)
```

Calculates the present value (PV) of an annuity - a series of equal payments (optionally including a future value inflow), and accounting for payment timing (end or beginning of period).

#### Example

You are taking a loan where you will pay $500/month for 3 years (36 months), with an interest rate of 6% annually (0.5% per month):

- `rate` - `0.005` (we have monthly intervals)
- `periods` - `36` (3 years x 12 months)
- `pmt` - `-500` (you pay back the loan)

#### Equations

- End of Period (`PaymentDueType::EndOfPeriod`):

$$
PV = -PMT \times \frac{1 - (1 + r)^{-n}}{r} - FV \times (1 + r)^{-n}
$$

- Beginning of Period (`PaymentDueType::BeginningOfPeriod`):

$$
PV = -PMT \times \frac{1 - (1 + r)^{-n}}{r} \times (1 + r) - FV \times (1 + r)^{-n}
$$

- Zero Rate Special Case:

$$
PV = -PMT \times n - FV
$$

### `fv` (Future Value of an Annuity)

```cpp
template <PaymentDueType due_type>
double fv(double rate, uint32_t periods, double pmt, double present_value = 0.0)
```

Calculates the future value (FV) of an annuity - a series of equal payments (optionally including a present value), and accounting for whether payments are made at the beginning or end of each period.

#### Example

You invest $100 per month for 10 years in an account earning 6% annually (0.5% monthly):

- `rate` - `0.005` (monthly interest rate)
- `periods` - `120` (10 years x 12 months)
- `pmt` - `-100` (you pay into the account each month)
- `present_value` - `0.0` (no initial deposit)

#### Equations

- End of Period (PaymentDueType::EndOfPeriod):

$$
FV = -PMT \times \frac{(1+r)^{n}-1}{r} - PV \times (1+r)^{n}
$$

- Beginning of Period (PaymentDueType::BeginningOfPeriod):

$$
FV = -PMT \times \frac{(1+r)^{n}-1}{r} \times (1+r)-PV \times (1+r)^{n}
$$

- Zero Rate Special Case (r = 0):

$$
FV=-PMT \times n - PV
$$

### Supported `DayCountConventions`

```cpp
enum class DayCountConvention : int32_t
{
    ACT_365F,
    ACT_365_25,
};
```
