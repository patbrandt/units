#ifndef UNITS_PAIR_H
#define UNITS_PAIR_H

#include <functional>

#include "units.h"

namespace units {

template <typename rep_,
          typename fraction1_, typename fraction2_,
          typename units_tag1_, typename units_tag2_>
struct units_pair;

template <typename rep_,
          typename fraction1_, typename fraction2_,
          typename units_tag1_, typename units_tag2_>
struct units_pair : public detail::units_base<rep_> {

    using rep = typename detail::units_base<rep_>::rep;
    using units1 = units<rep_, fraction1_, units_tag1_>;
    using units2 = units<rep_, fraction2_, units_tag2_>;
    using units_operator = std::function<rep(const rep&, const rep&)>;

    constexpr units_pair() = default;
    units_pair(const units_pair&) = default;
    units_pair& operator=(const units_pair&) = default;
    ~units_pair() = default;

    template<typename rep2,
             typename = typename std::enable_if<
                 std::is_convertible<rep2, rep>::value &&
                 (std::is_floating_point<rep>::value ||
                  !std::is_floating_point<rep2>::value)>::type>
    constexpr explicit units_pair(const rep2& rep2_)
        : detail::units_base<rep>(static_cast<rep>(rep2_)) {}

    template<typename rep2, typename fraction1, typename fraction2,
             typename units_tag1, typename units_tag2,
             typename = typename std::enable_if<
                 std::is_floating_point<rep>::value ||
                 (std::ratio_multiply<fraction1, fraction2>::den == 1 &&
                  !std::is_floating_point<rep2>::value)>::type>
    constexpr units_pair(const units<rep2, fraction1, units_tag1> &u1,
                         const units<rep2, fraction2, units_tag2> &u2,
                         units_operator op = std::multiplies<rep>())
        : detail::units_base<rep>(op(units_cast<units1>(u1).amount(),
                                     units_cast<units2>(u2).amount())),
          op_(op) {}

    units_operator pair_operator() const { return op_; }

    // template<typename fraction1, typename fraction2>
    // constexpr units_pair(
    //     const units_pair<rep, fraction1, fraction2, units_tag1_, units_tag2_> &up)
    //     : detail::units_base<rep>(
    //         units_operator()(
    //             units_cast<units1, rep, fraction1, units_tag1_>(up.amount()).amount(),
    //             units_cast<units2, rep, fraction2, units_tag2_>(1).amount())) {}

    // template<typename fraction1, typename fraction2>
    // constexpr units_pair(
    //     const units_pair<rep, fraction1, fraction2, units_tag2_, units_tag1_> &up)
    //     : detail::units_base<rep>(
    //         units_cast<units1, rep, fraction1, units_tag1_>(up.amount()).amount(),
    //         units_cast<units2, rep, fraction2, units_tag2_>(1).amount())) {}

private:
    units_operator op_;
};

template <typename rep,
          typename fraction1, typename units_tag1,
          typename fraction2, typename units_tag2>
constexpr units_pair<rep, fraction1, fraction2, units_tag1, units_tag2>
operator*(const units<rep, fraction1, units_tag1> &u1,
          const units<rep, fraction2, units_tag2> &u2) {
    return units_pair<rep, fraction1, fraction2, units_tag1, units_tag2>(
        u1, u2, std::multiplies<rep>());
}

template <typename rep,
          typename fraction1, typename units_tag1,
          typename fraction2, typename units_tag2>
constexpr units_pair<rep, fraction1, fraction2, units_tag1, units_tag2>
operator/(const units<rep, fraction1, units_tag1> &u1,
          const units<rep, fraction2, units_tag2> &u2) {
    return units_pair<rep, fraction1, fraction2, units_tag1, units_tag2>(
        u1, u2, std::divides<rep>());
}

} // namespace units

#endif//UNITS_PAIR_H
