#ifndef UNITS_H
#define UNITS_H

#include <iostream>
#include <ratio>
#include <type_traits>

namespace units {

template <typename rep_, typename fraction_, typename units_tag_>
struct units;

// Check if something is a unit type
template <typename T>
struct is_unit : std::false_type {};

template <typename rep, typename fraction, typename units_tag>
struct is_unit<units<rep, fraction, units_tag> > : std::true_type {};

// Disable unit conversion in the general case
template <typename u1, typename u2>
struct is_unit_convertible : std::false_type {};

// Only units with identical type tags can be converted
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

namespace detail {

// unit_cast implementation: general case
template <typename to_unit, typename common_fraction, typename common_rep,
          bool num_one = false, bool den_one = false>
struct unit_cast_impl {
    template <typename rep_, typename fraction_, typename units_tag_>
    static constexpr to_unit cast(const units<rep_, fraction_, units_tag_> &u) {
        typedef typename to_unit::rep to_rep;
        return to_unit(static_cast<to_rep>(static_cast<common_rep>(u.amount()) *
                                           static_cast<common_rep>(common_fraction::num) /
                                           static_cast<common_rep>(common_fraction::den)));
    }
};

// unit_cast implementation: fraction is unity (1/1)
template <typename to_unit, typename common_fraction, typename common_rep>
struct unit_cast_impl<to_unit, common_fraction, common_rep, true, true> {
    template <typename rep_, typename fraction_, typename units_tag_>
    static constexpr to_unit cast(const units<rep_, fraction_, units_tag_> &u) {
        typedef typename to_unit::rep to_rep;
        return to_unit(static_cast<to_rep>(u.amount()));
    }
};

// unit_cast implementation: fraction only has denominator
template <typename to_unit, typename common_fraction, typename common_rep>
struct unit_cast_impl<to_unit, common_fraction, common_rep, true, false> {
    template <typename rep_, typename fraction_, typename units_tag_>
    static constexpr to_unit cast(const units<rep_, fraction_, units_tag_> &u) {
        typedef typename to_unit::rep to_rep;
        return to_unit(static_cast<to_rep>(static_cast<common_rep>(u.amount()) /
                                           static_cast<common_rep>(common_fraction::den)));
    }
};

// unit_cast implementation: fraction only has numerator
template <typename to_unit, typename common_fraction, typename common_rep>
struct unit_cast_impl<to_unit, common_fraction, common_rep, false, true> {
    template <typename rep_, typename fraction_, typename units_tag_>
    static constexpr to_unit cast(const units<rep_, fraction_, units_tag_> &u) {
        typedef typename to_unit::rep to_rep;
        return to_unit(static_cast<to_rep>(static_cast<common_rep>(u.amount()) *
                                           static_cast<common_rep>(common_fraction::num)));
    }
};

} // namespace detail

// units_cast
template <typename to_unit, typename rep, typename fraction, typename units_tag>
constexpr typename std::enable_if<is_unit<to_unit>::value, to_unit>::type
units_cast(const units<rep, fraction, units_tag> &u) {
    static_assert(is_unit_convertible<to_unit, units<rep, fraction, units_tag> >::value,
                  "units must be convertible in order to cast");
    typedef typename to_unit::rep to_rep;
    typedef typename to_unit::fraction to_fraction;
    typedef std::ratio_divide<fraction, to_fraction> common_fraction;
    typedef typename std::common_type<to_rep, rep, std::intmax_t>::type common_rep;
    typedef detail::unit_cast_impl<to_unit, common_fraction, common_rep,
        common_fraction::num==1, common_fraction::den==1> uc;

    return uc::cast(u);
}

// units template
template <typename rep_, typename fraction_, typename units_tag_>
struct units {
    typedef rep_ rep;
    typedef fraction_ fraction;
    typedef units_tag_ units_tag;

    constexpr units() = default;
    units(const units&) = default;
    units& operator=(const units&) = default;
    ~units() = default;

    template<typename rep2,
             typename = typename std::enable_if<
                 std::is_convertible<rep2, rep>::value &&
                 (std::is_floating_point<rep>::value ||
                  !std::is_floating_point<rep2>::value)>::type>
    constexpr explicit units(const rep2& rep2_)
        : value_(static_cast<rep>(rep2_)) {}

    template<typename rep2, typename fraction2,
             typename = typename std::enable_if<
                 std::is_floating_point<rep>::value ||
                 (std::ratio_multiply<fraction2, fraction>::den == 1 &&
                  !std::is_floating_point<rep2>::value)>::type>
    constexpr units(const units<rep2, fraction2, units_tag>& d)
        : value_(units_cast<units>(d).amount()) {}

    constexpr rep amount() const { return value_; }

    constexpr units operator+() const { return *this; }
    constexpr units operator-() const { return units(-value_); }

    units& operator++() {
        ++value_;
        return *this;
    }

    units operator++(int) {
        return units(value_++);
    }

    units& operator--() {
        --value_;
        return *this;
    }

    units operator--(int) {
        return units(value_--);
    }

    units operator+=(const units &u) {
        value_ += u.amount();
        return *this;
    }

    units operator-=(const units &u) {
        value_ -= u.amount();
        return *this;
    }

    units operator*=(const units &u) {
        value_ *= u.amount();
        return *this;
    }

    units operator/=(const units &u) {
        value_ /= u.amount();
        return *this;
    }

    template <typename rep2 = rep>
    typename std::enable_if<!std::is_floating_point<rep2>::value, units&>
    operator%=(const rep2 &rhs) {
        value_ %= rhs;
        return *this;
    }

    template <typename rep2 = rep>
    typename std::enable_if<!std::is_floating_point<rep2>::value, units&>
    operator%=(const units &u) {
        value_ %= u.amount();
        return *this;
    }

private:
    rep value_;
};

// template <class rep1, class fraction, class rep2>
// inline constexpr
// typename std::enable_if <!is_unit<rep2>,
//                          typename unit_divide_result<
//                              unit<rep1, fraction>, Rep2>::type
//                          >::type
// operator/(const units<rep1, fraction> &u, const rep2& s)
// {
//     typedef typename common_type<rep1, rep2>::type common_rep;
//     typedef units<common_rep, fraction> common_unit;

//     return common_unit(common_unit(u).count()/static_cast<common_rep>(s));
// }

} // namespace units

#endif//UNITS_H
