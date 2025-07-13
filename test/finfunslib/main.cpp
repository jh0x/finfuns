#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <finfunslib/finfunslib.h>

TEST_CASE("pv lib")
{
    const double result = finfuns_pv_due_end(0.05, 10, -100.0, 0.0);
    CHECK(result == doctest::Approx(772.173492918482));
}
