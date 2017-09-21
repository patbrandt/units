#ifndef UNITS_CAST_H
#define UNITS_CAST_H

// STL
#include <ratio>
#include <type_traits>
// Units
#include "detail/units_cast_impl.h"

namespace units {

// units_cast
template <typename to_unit, typename rep, typename fraction, typename units_tag>
constexpr typename std::enable_if<is_unit<to_unit>::value, to_unit>::type
units_cast(const units<rep, fraction, units_tag> &u) {
    static_assert(is_unit_convertible<to_unit, units<rep, fraction, units_tag> >::value,
                  "units must be convertible in order to cast");
    using to_rep = typename to_unit::rep;
    using to_fraction = typename to_unit::fraction;
    using common_fraction = std::ratio_divide<fraction, to_fraction>;
    using common_rep = typename std::common_type<to_rep, rep>::type;
    using uc = detail::units_cast_impl<to_unit, common_fraction, common_rep,
        common_fraction::num == 1, common_fraction::den == 1>;

    return uc::cast(u);
}

} // namespace units

#endif//UNITS_CAST_H
