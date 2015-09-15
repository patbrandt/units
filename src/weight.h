#ifndef WEIGHT_H
#define WEIGHT_H

#include "units.h"

namespace weight {

namespace detail {

struct weight_tag {};

template <typename rep_, typename fraction_>
using weight = units::units<rep_, fraction_, weight_tag>;

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
using kilogram = detail::weight<
    rep, std::ratio_multiply<std::kilo,
                             typename grams<rep>::fraction> >;

template <typename rep>
using metric_tons = detail::weight<
    rep, std::ratio_multiply<std::mega,
                             typename grams<rep>::fraction> >;


} // namespace weight

#endif//WEIGHT_H
