//
// Created by ky3he4ik on 15/11/19.
//

#include "EllipticCurve.h"

EllipticCurve::EllipticCurve(const LongInt &_a, const LongInt &_b, const LongInt &_c, const LongInt &_p,
                             const LongInt &_exp) : a(_a), b(_b), c(_c), p(_p), exp(_exp), random() {}

UINT EllipticCurve::set_curve_order(const Point &point) {
    if (p == 0) {
        curve_order = 0;
        return curve_order;
    }
    Point q(point);
    curve_order = 1;
    //Add P to Q repeatedly until obtaining the identity (point at infinity).
    while (!q.get_inf()) {
        q = point + q;
        ++curve_order;
//        std::cerr << curve_order << ' ' << q.to_string() << '\n';
    }
    return curve_order;
}

std::string EllipticCurve::to_string() const {
    std::string res("y^2 = x^3");
    if (a != 0) {
        if (a > 0)
            res += std::string(" + ");
        res += a.to_string() + "*x^2";
    }
    if (b != 0) {
        if (b > 0)
            res += std::string(" + ");
        res += b.to_string() + "*x";
    }
    if (c != 0) {
        if (c > 0)
            res += std::string(" + ");
        res += c.to_string();
    }
    if (p != 0) {
        res += " over field " + p.to_string();
        if (exp != 1) {
            res += "^" + exp.to_string();
        }
    }
    return res;
}

// Compute the discriminant.
LongInt EllipticCurve::discriminant() const {
    return a * a * (b * b - (a * c) << 2) + (b << 2) * (9 * a * c - 2 * b * b) - 27 * c * c;
}

// Compute the order of a point on the curve.
// Порядок точки
UINT EllipticCurve::get_order() const {
    return curve_order;
}

bool EllipticCurve::contains(const Point &point) const {
    return point.get_inf() || (
            point.get_y().fast_pow_mod(LongInt(LONG_INT_LEN, 2), p).abs() ==
            (point.get_x().fast_pow_mod(LongInt(LONG_INT_LEN, 3), p).abs() +
             (point.get_x().fast_pow_mod(LongInt(LONG_INT_LEN, 2), p) * a).abs() + (point.get_x() * b).abs() + c
            ) % p

    );
}

EllipticCurve EllipticCurve::getSECP256k1() {
    return EllipticCurve(LongInt(LONG_INT_LEN), LongInt(LONG_INT_LEN), LongInt(LONG_INT_LEN, 7),
                         LongInt(LONG_INT_LEN, "0xfffffffffffffffffffffffffffffffffffffffffffffffffffffffefffffc2f",
                                 16),
                         LongInt(LONG_INT_LEN, 1));
}

/*
 * Generate a keypair using the point P of order n on the given curve. The private key is a
 * positive integer d smaller than n, and the public key is Q = dP.
 */
std::pair<UINT, Point> EllipticCurve::generate_keypair(const Point &point) {
    UINT d = LongInt::get_random(LONG_INT_LEN, random) % curve_order;
    Point Q = point * LongInt(LONG_INT_LEN, d);
    return {d, Q};
}