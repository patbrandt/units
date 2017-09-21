#ifndef UNITS_H
#define UNITS_H

// STL
#include <ratio>
#include <type_traits>
// Units
#include "units_cast.h"
#include "units_traits.h"

namespace units {

// units template
template <typename rep_, typename fraction_, typename units_tag_>
struct units {
    using rep = rep_;
    using fraction = fraction_;
    using units_tag = units_tag_;

    constexpr units() = default;
    units(const units&) = default;
    units(units&&) = default;
    units& operator=(const units&) = default;
    units& operator=(units&&) = default;
    ~units() noexcept = default;

    template <typename rep2,
              typename = typename std::enable_if<
                  std::is_convertible<rep2, rep>::value &&
                  (std::is_floating_point<rep>::value ||
                   !std::is_floating_point<rep2>::value)>::type>
    constexpr explicit units(const rep2& v)
        : value_(static_cast<rep>(v)) {}

    template <typename rep2, typename frac2,
              typename = typename std::enable_if<
                  std::is_floating_point<rep>::value ||
                  (std::ratio_multiply<frac2, fraction>::den == 1 &&
                   !std::is_floating_point<rep2>::value)>::type>
    constexpr units(const units<rep2, frac2, units_tag> &other)
        : value_(static_cast<rep>(units_cast<units>(other).amount())) {}

    constexpr rep amount() const { return value_; }

    constexpr units operator+() const { return *this; }
    constexpr units operator-() const { return units(-value_); }

    // Addition and subtraction of units result in the same unit
    template <typename rep2, typename frac2>
    units& operator+=(const units<rep2, frac2, units_tag> &u2) {
        value_ += units_cast<units>(u2).amount();
        return *this;
    }

    template <typename rep2, typename frac2>
    units& operator-=(const units<rep2, frac2, units_tag> &u2) {
        value_ -= units_cast<units>(u2).amount();
        return *this;
    }

    // Scalar adjustment of units?
    units& operator+=(const rep &c) {
        value_ += c;
        return *this;
    }

    units& operator-=(const rep &c) {
        value_ -= c;
        return *this;
    }

    units& operator*=(const rep &c) {
        value_ *= c;
        return *this;
    }

    units& operator/=(const rep &c) {
        value_ /= c;
        return *this;
    }

private:
    rep value_;
};

// Basic comparison operators for units
template <typename rep, typename frac, typename ut>
bool operator==(const units<rep, frac, ut> &ub1,
                const units<rep, frac, ut> &ub2) noexcept {
    return ub1.amount() == ub2.amount();
}

template <typename rep, typename frac, typename ut>
bool operator!=(const units<rep, frac, ut> &ub1,
                const units<rep, frac, ut> &ub2) noexcept {
    return !(ub1 == ub2);
}

template <typename rep, typename frac, typename ut>
bool operator>=(const units<rep, frac, ut> &ub1,
                const units<rep, frac, ut> &ub2) noexcept {
    return ub1.amount() >= ub2.amount();
}

template <typename rep, typename frac, typename ut>
bool operator<=(const units<rep, frac, ut> &ub1,
                const units<rep, frac, ut> &ub2) noexcept {
    return ub1.amount() <= ub2.amount();
}

template <typename rep, typename frac, typename ut>
bool operator>(const units<rep, frac, ut> &ub1,
               const units<rep, frac, ut> &ub2) noexcept {
    return ub1.amount() > ub2.amount();
}

template <typename rep, typename frac, typename ut>
bool operator<(const units<rep, frac, ut> &ub1,
               const units<rep, frac, ut> &ub2) noexcept {
    return ub1.amount() < ub2.amount();
}

// Unit scalar math operations
template <typename rep, typename frac, typename ut>
units<rep, frac, ut> operator+(
    units<rep, frac, ut> ub1, const rep &s) noexcept {
    return ub1 += s;
}

template <typename rep, typename frac, typename ut>
units<rep, frac, ut> operator-(
    units<rep, frac, ut> ub1, const rep &s) noexcept {
    return ub1 -= s;
}

template <typename rep, typename frac, typename ut>
units<rep, frac, ut> operator*(
    units<rep, frac, ut> ub1, const rep &s) noexcept {
    return ub1 *= s;
}

template <typename rep, typename frac, typename ut>
units<rep, frac, ut> operator/(
    units<rep, frac, ut> ub1, const rep &s) noexcept {
    return ub1 /= s;
}

// Unit addition and subtraction
template <typename rep, typename frac, typename ut>
units<rep, frac, ut> operator+(
    units<rep, frac, ut> ub1, const units<rep, frac, ut> &ub2) noexcept {
    return ub1 += ub2;
}

template <typename rep, typename frac, typename ut>
units<rep, frac, ut> operator-(
    units<rep, frac, ut> ub1, const units<rep, frac, ut> &ub2) noexcept {
    return ub1 -= ub2;
}

// Dividing an operator by a like unit results in a scalar ratio
template <typename rep, typename frac1, typename frac2, typename ut>
rep operator/(const units<rep, frac1, ut> &ub1,
              const units<rep, frac2, ut> &ub2) noexcept {
    return ub1.amount() / units_cast<units<rep, frac1, ut> >(ub2).amount();
}

} // namespace units

#endif//UNITS_H
