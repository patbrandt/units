#ifndef WEIGHT_H
#define WEIGHT_H

// STL
#include <ratio>
// Units
#include "units.h"

namespace units {

inline namespace weight {

namespace detail {

struct weight_tag {};

template <typename rep_, typename fraction_>
using weight = units<rep_, fraction_, weight_tag>;

} // namespace detail

template <typename rep>
using ounces = detail::weight<rep, std::ratio<1> >;
template <typename rep>
using pounds = detail::weight<
    rep, std::ratio_multiply<std::ratio<16>,
                             typename ounces<rep>::fraction> >;
template <typename rep>
using stones = detail::weight<
    rep, std::ratio_multiply<std::ratio<14>,
                             typename pounds<rep>::fraction> >;
template <typename rep>
using short_tons = detail::weight<
    rep, std::ratio_multiply<std::ratio<2000>,
                             typename pounds<rep>::fraction> >;
template <typename rep>
using long_tons = detail::weight<
    rep, std::ratio_multiply<std::ratio<2240>,
                             typename pounds<rep>::fraction> >;

template <typename rep>
using grams = detail::weight<
    rep, std::ratio_multiply<std::ratio<10000, 283495>,
                             typename ounces<rep>::fraction> >;
template <typename rep>
using micrograms = detail::weight<
    rep, std::ratio_multiply<std::micro,
                             typename grams<rep>::fraction> >;
template <typename rep>
using kilograms = detail::weight<
    rep, std::ratio_multiply<std::kilo,
                             typename grams<rep>::fraction> >;
template <typename rep>
using metric_tons = detail::weight<
    rep, std::ratio_multiply<std::mega,
                             typename grams<rep>::fraction> >;

} // namespace weight

} // namespace units

#endif//WEIGHT_H
