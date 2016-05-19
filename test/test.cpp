#include "gtest/gtest.h"

#include "distance.h"
#include "units_pair.h"
#include "weight.h"

using namespace units;

TEST(UnitsTest, UnitsOperators) {
    using inches = distance::inches<float>;
    inches i1{5.25}, i2{4};

    // Assert true comparisons
    EXPECT_EQ(1, i1 == i1);
    EXPECT_EQ(1, i1 != i2);
    EXPECT_EQ(1, i1 > i2);
    EXPECT_EQ(1, i1 >= i1);
    EXPECT_EQ(1, i2 < i1);
    EXPECT_EQ(1, i2 <= i1);

    // Assert false comparisons
    EXPECT_EQ(0, i1 == i2);
    EXPECT_EQ(0, i1 != i1);
    EXPECT_EQ(0, i1 < i1);
    EXPECT_EQ(0, i2 > i1);
    EXPECT_EQ(0, i2 >= i1);
    EXPECT_EQ(0, i1 <= i2);

    // Test scaling operations
    EXPECT_EQ(inches{7}, i1 + 1.75f);
    EXPECT_EQ(inches{3}, i1 - 2.25f);
    EXPECT_EQ(inches{10.5}, i1 * 2.f);
    EXPECT_EQ(inches{2}, i2 / 2.f);

    // Test unit math operations
    EXPECT_EQ(inches{9.2500f}, i1 + i2);
    EXPECT_EQ(inches{1.2500f}, i1 - i2);
    EXPECT_EQ(1.3125f, i1 / i2);

    EXPECT_EQ(inches{9.2500f}, inches{i1} += i2);
    EXPECT_EQ(inches{1.2500f}, inches{i1} -= i2);
}

TEST(UnitsTest, Distance) {
    using millimeters = distance::millimeters<float>;
    using inches = distance::inches<float>;
    auto mm = millimeters{200};
    EXPECT_FLOAT_EQ(7.8740158f, units_cast<inches>(mm).amount());

    using nautical_miles = distance::nautical_miles<float>;
    using kilometers = distance::kilometers<float>;
    auto nm = nautical_miles{1};
    EXPECT_FLOAT_EQ(1.852f, units_cast<kilometers>(nm).amount());

    auto in = inches{5};
    EXPECT_FLOAT_EQ(127.0f, units_cast<millimeters>(in).amount());

    using miles = distance::miles<float>;
    auto mi = miles{1};
    EXPECT_FLOAT_EQ(1.609344f, units_cast<kilometers>(mi).amount());

    using yards = distance::yards<float>;
    using meters = distance::meters<float>;
    auto yd = yards{3};
    EXPECT_FLOAT_EQ(2.7432f, units_cast<meters>(yd).amount());

    auto m = meters{1};
    EXPECT_FLOAT_EQ(39.370079f, units_cast<inches>(m).amount());
}

TEST(UnitsTest, Weights) {
    using grams = weight::grams<float>;
    using pounds = weight::pounds<float>;
    auto g = grams{200};
    EXPECT_FLOAT_EQ(0.440925f, units_cast<pounds>(g).amount());

    using short_tons = weight::short_tons<float>;
    using metric_tons = weight::metric_tons<float>;
    auto st = short_tons{1};
    EXPECT_FLOAT_EQ(0.907184f, units_cast<metric_tons>(st).amount());
}

TEST(UnitsTest, UnitsPair) {
    // Test basic construction via multiplication
    using grams = weight::grams<float>;
    using yards = distance::yards<float>;
    auto gy = grams{5} * yards{3};
    EXPECT_FLOAT_EQ(15.f, gy.amount());

    // Test basic construction via division
    using stones = weight::stones<float>;
    using inches = distance::inches<float>;
    auto si = stones{10} / inches{2};
    EXPECT_FLOAT_EQ(5.f, si.amount());

    // Test manual conversion
    using pounds = weight::pounds<float>;
    using meters = distance::meters<float>;
    using poundmeters = units_pair<pounds, meters>;
    poundmeters pm(grams{5}, yards{3});
    EXPECT_FLOAT_EQ(0.0302386280, pm.amount());
}

TEST(UnitsTest, UnitsPairOperators) {
    using inches = distance::inches<float>;
    using stones = weight::stones<float>;
    auto si1 = stones{20} / inches{2};
    auto si2 = stones{10} / inches{2};

    // Assert true comparisons
    EXPECT_EQ(1, si1 == si1);
    EXPECT_EQ(1, si1 != si2);
    EXPECT_EQ(1, si1 > si2);
    EXPECT_EQ(1, si1 >= si1);
    EXPECT_EQ(1, si2 < si1);
    EXPECT_EQ(1, si2 <= si1);

    // Assert false comparisons
    EXPECT_EQ(0, si1 == si2);
    EXPECT_EQ(0, si1 != si1);
    EXPECT_EQ(0, si1 < si1);
    EXPECT_EQ(0, si2 > si1);
    EXPECT_EQ(0, si2 >= si1);
    EXPECT_EQ(0, si1 <= si2);

    // Test scaling operations
    using stones_per_inch = units_pair<stones, inches>;
    EXPECT_EQ((stones_per_inch{11.75f, std::divides<float>{}}), si1 + 1.75f);
    EXPECT_EQ((stones_per_inch{7.75f, std::divides<float>()}), si1 - 2.25f);
    EXPECT_EQ((stones_per_inch{20.f, std::divides<float>()}), si1 * 2.f);
    EXPECT_EQ((stones_per_inch{2.5f, std::divides<float>()}), si2 / 2.f);

    // Test unit math operations
    EXPECT_EQ((stones_per_inch{15.f}), si1 + si2);
    EXPECT_EQ((stones_per_inch{5.f}), si1 - si2);
    EXPECT_EQ(2.f, si1 / si2);

    EXPECT_EQ((stones_per_inch{15.f, std::divides<float>()}), stones_per_inch{si1} += si2);
    EXPECT_EQ((stones_per_inch{5.f, std::divides<float>()}), stones_per_inch{si1} -= si2);
}
