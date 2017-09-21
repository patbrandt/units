#ifndef UNITS_CAST_IMPL_H
#define UNITS_CAST_IMPL_H

#include "units_fwd.h"
#include "units_traits.h"

namespace units {

namespace detail {

// unit_cast implementation: general case
template <typename to_unit, typename common_fraction, typename common_rep,
          bool num_one = false, bool den_one = false>
struct units_cast_impl {
    template <typename rep_, typename fraction_, typename units_tag_>
    static constexpr to_unit cast(const units<rep_, fraction_, units_tag_> &u) {
        using to_rep = typename to_unit::rep;
        return to_unit(static_cast<to_rep>(static_cast<common_rep>(u.amount()) *
                                           static_cast<common_rep>(common_fraction::num) /
                                           static_cast<common_rep>(common_fraction::den)));
    }
};

// unit_cast implementation: fraction is unity (1/1)
template <typename to_unit, typename common_fraction, typename common_rep>
struct units_cast_impl<to_unit, common_fraction, common_rep, true, true> {
    template <typename rep_, typename fraction_, typename units_tag_>
    static constexpr to_unit cast(const units<rep_, fraction_, units_tag_> &u) {
        using to_rep = typename to_unit::rep;
        return to_unit(static_cast<to_rep>(u.amount()));
    }
};

// unit_cast implementation: fraction only has denominator
template <typename to_unit, typename common_fraction, typename common_rep>
struct units_cast_impl<to_unit, common_fraction, common_rep, true, false> {
    template <typename rep_, typename fraction_, typename units_tag_>
    static constexpr to_unit cast(const units<rep_, fraction_, units_tag_> &u) {
        using to_rep = typename to_unit::rep;
        return to_unit(static_cast<to_rep>(static_cast<common_rep>(u.amount()) /
                                           static_cast<common_rep>(common_fraction::den)));
    }
};

// unit_cast implementation: fraction only has numerator
template <typename to_unit, typename common_fraction, typename common_rep>
struct units_cast_impl<to_unit, common_fraction, common_rep, false, true> {
    template <typename rep_, typename fraction_, typename units_tag_>
    static constexpr to_unit cast(const units<rep_, fraction_, units_tag_> &u) {
        using to_rep = typename to_unit::rep;
        return to_unit(static_cast<to_rep>(static_cast<common_rep>(u.amount()) *
                                           static_cast<common_rep>(common_fraction::num)));
    }
};

} // namespace detail

} // namespace units

#endif//UNITS_CAST_IMPL_H
