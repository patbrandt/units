#ifndef UNITS_TRAITS_H
#define UNITS_TRAITS_H

// STL
#include <type_traits>
// Units
#include "units_fwd.h"

namespace units {

// Check if something is a unit type
template <typename T>
struct is_unit : std::false_type {};

template <typename rep, typename fraction, typename units_tag>
struct is_unit<units<rep, fraction, units_tag> > : std::true_type {};

// Disable unit conversion in the general case
template <typename u1, typename u2>
struct is_unit_convertible : std::false_type {};

// Only units with identical tags can be converted
template <typename rep1, typename fraction1,
          typename rep2, typename fraction2, typename units_tag>
struct is_unit_convertible<units<rep1, fraction1, units_tag>,
                           units<rep2, fraction2, units_tag> > : std::true_type {
    static_assert(std::is_convertible<rep1, rep2>::value,
                  "unit reps must be convertible");
    static_assert(std::is_floating_point<rep1>::value ||
                  !std::is_floating_point<rep2>::value,
                  "cannot mix floating and integral types in unit conversion");
};

} // namespace units

#endif//UNITS_TRAITS_H
