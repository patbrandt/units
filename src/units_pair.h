#ifndef UNITS_PAIR_H
#define UNITS_PAIR_H

#include <functional>
#include <type_traits>

#include "units.h"

namespace units {

template <typename units1, typename units2>
struct units_pair : public detail::units_base<
    typename std::common_type<typename units1::rep,
                              typename units2::rep>::type> {

    using rep = typename std::common_type<
        typename units1::rep, typename units2::rep>::type;
    using units_operator = std::function<rep(const rep&, const rep&)>;

    constexpr units_pair() = default;
    units_pair(const units_pair&) = default;
    units_pair(units_pair&&) = default;
    units_pair& operator=(const units_pair&) = default;
    units_pair& operator=(units_pair&&) = default;
    ~units_pair() noexcept = default;

    constexpr units_pair(const units1 &u1, const units2 &u2,
                         units_operator op = std::multiplies<rep>())
        : detail::units_base<rep>(op(units_cast<units1>(u1).amount(),
                                     units_cast<units2>(u2).amount())),
        op_(op) {}

    units_operator pair_operator() const { return op_; }

private:
    units_operator op_;
};

template <typename rep,
          typename fraction1, typename units_tag1,
          typename fraction2, typename units_tag2>
constexpr units_pair<units<rep, fraction1, units_tag1>,
                     units<rep, fraction2, units_tag2> >
operator*(const units<rep, fraction1, units_tag1> &u1,
          const units<rep, fraction2, units_tag2> &u2) {
    return units_pair<units<rep, fraction1, units_tag1>,
                      units<rep, fraction2, units_tag2> >(
        u1, u2, std::multiplies<rep>());
}

template <typename rep,
          typename fraction1, typename units_tag1,
          typename fraction2, typename units_tag2>
constexpr units_pair<units<rep, fraction1, units_tag1>,
                     units<rep, fraction2, units_tag2> >
operator/(const units<rep, fraction1, units_tag1> &u1,
          const units<rep, fraction2, units_tag2> &u2) {
    return units_pair<units<rep, fraction1, units_tag1>,
                      units<rep, fraction2, units_tag2> >(
        u1, u2, std::divides<rep>());
}

} // namespace units

#endif//UNITS_PAIR_H
