#ifndef DISTANCE_H
#define DISTANCE_H

// STL
#include <ratio>
// Units
#include "units.h"

namespace units {

inline namespace distance {

namespace detail {

struct distance_tag {};

template <typename rep_, typename fraction_>
using distance = units<rep_, fraction_, distance_tag>;

} // namespace detail

template <typename rep>
using inches = detail::distance<rep, std::ratio<1> >;
template <typename rep>
using feet = detail::distance<
    rep, std::ratio_multiply<std::ratio<12>,
                             typename inches<rep>::fraction> >;
template <typename rep>
using yards = detail::distance<
    rep, std::ratio_multiply<std::ratio<3>,
                             typename feet<rep>::fraction> >;
template <typename rep>
using miles = detail::distance<
    rep, std::ratio_multiply<std::ratio<1760>,
                             typename yards<rep>::fraction> >;
template <typename rep>
using nautical_miles = detail::distance<
    rep, std::ratio_multiply<std::ratio<729134, 10>,
                             typename inches<rep>::fraction> >;


template <typename rep>
using meters = detail::distance<
    rep, std::ratio_multiply<std::ratio<10000, 254>,
                             typename inches<rep>::fraction> >;
template <typename rep>
using micrometers = detail::distance<
    rep, std::ratio_multiply<std::micro,
                             typename meters<rep>::fraction> >;
template <typename rep>
using millimeters = detail::distance<
    rep, std::ratio_multiply<std::milli,
                             typename meters<rep>::fraction> >;
template <typename rep>
using centimeters = detail::distance<
    rep, std::ratio_multiply<std::centi,
                             typename meters<rep>::fraction> >;
template <typename rep>
using kilometers = detail::distance<
    rep, std::ratio_multiply<std::kilo,
                             typename meters<rep>::fraction> >;

} // namespace distance

} // namespace units

#endif//DISTANCE_H
