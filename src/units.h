#ifndef UNITS_H
#define UNITS_H

// STL
#include <ratio>
#include <type_traits>

namespace units {

template <typename rep_, typename fraction_, typename units_tag_>
struct units;

namespace detail {

template <typename rep_>
struct units_base {
    using rep = rep_;

    constexpr units_base() = default;
    units_base(const units_base&) = default;
    units_base(units_base&&) = default;
    units_base& operator=(const units_base&) = default;
    units_base& operator=(units_base&&) = default;

    units_base(const rep& v) : value_(v) {}

    constexpr rep amount() const { return value_; }

    constexpr units_base operator+() const { return *this; }
    constexpr units_base operator-() const { return units_base(-value_); }

    units_base& operator++() {
        ++value_;
        return *this;
    }

    units_base operator++(int) {
        return units_base(value_++);
    }

    units_base& operator--() {
        --value_;
        return *this;
    }

    units_base operator--(int) {
        return units_base(value_--);
    }

    units_base operator+=(const units_base &u) {
        value_ += u.amount();
        return *this;
    }

    units_base operator-=(const units_base &u) {
        value_ -= u.amount();
        return *this;
    }

    units_base operator*=(const units_base &u) {
        value_ *= u.amount();
        return *this;
    }

    units_base operator/=(const units_base &u) {
        value_ /= u.amount();
        return *this;
    }

    template <typename rep2 = rep>
    typename std::enable_if<!std::is_floating_point<rep2>::value, units_base&>
    operator%=(const rep2 &rhs) {
        value_ %= rhs;
        return *this;
    }

    template <typename rep2 = rep>
    typename std::enable_if<!std::is_floating_point<rep2>::value, units_base&>
    operator%=(const units_base &u) {
        value_ %= u.amount();
        return *this;
    }

protected:
    ~units_base() noexcept = default;

private:
    rep value_;
};

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

// units template
template <typename rep_, typename fraction_, typename units_tag_>
struct units : public detail::units_base<rep_> {
    using rep = typename detail::units_base<rep_>::rep;
    using fraction = fraction_;
    using units_tag = units_tag_;

    constexpr units() = default;
    units(const units&) = default;
    units(units&&) = default;
    units& operator=(const units&) = default;
    units& operator=(units&&) = default;
    ~units() noexcept = default;

    template<typename rep2,
             typename = typename std::enable_if<
                 std::is_convertible<rep2, rep>::value &&
                 (std::is_floating_point<rep>::value ||
                  !std::is_floating_point<rep2>::value)>::type>
    constexpr explicit units(const rep2& v)
        : detail::units_base<rep>(static_cast<rep>(v)) {}

    template<typename rep2, typename fraction2,
             typename = typename std::enable_if<
                 std::is_floating_point<rep>::value ||
                 (std::ratio_multiply<fraction2, fraction>::den == 1 &&
                  !std::is_floating_point<rep2>::value)>::type>
    constexpr units(const units<rep2, fraction2, units_tag> &u)
        : detail::units_base<rep>(units_cast<units>(u).amount()) {}
};

} // namespace units

#endif//UNITS_H
