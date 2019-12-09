//
// Created by ky3he4ik on 15/11/19.
//

#include "EllipticCurve.h"

EllipticCurve::EllipticCurve(const LongInt &_a, const LongInt &_b, const LongInt &_c, const LongInt &_p,
                             const LongInt &_exp) : a(_a), b(_b), c(_c), p(_p), exp(_exp), random() {
    if (!is_valid())
        std::cerr << "Warning: this curve is invalid!\n";
}

// Compute the order of a point on the curve. Slow version. Todo: Schoof's algorithm
// Порядок точки
LongInt EllipticCurve::set_curve_order(const Point &point) {
    base_point = new Point(point);
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

bool EllipticCurve::contains(const Point &point) const {
    return point.get_inf() || (
            point.get_y().fast_pow_mod(LongInt(LONG_INT_LEN, 2), p).abs() ==
            (point.get_x().fast_pow_mod(LongInt(LONG_INT_LEN, 3), p).abs() +
             (point.get_x().fast_pow_mod(LongInt(LONG_INT_LEN, 2), p) * a).abs() + (point.get_x() * b).abs() + c
            ) % p

    );
}

EllipticCurve EllipticCurve::getSECP256k1() {
    return EllipticCurve(LongInt(256), LongInt(256), LongInt(256, 7),
                         LongInt(256,
                                 "0xFFFFFFFF_FFFFFFFF_FFFFFFFF_FFFFFFFF_FFFFFFFF_FFFFFFFF_FFFFFFFE_FFFFFC2F", 16),
                         LongInt(256, 1));
}

/*
 * Generate a keypair using the point P of order n on the given curve. The private key is a
 * positive integer d smaller than n, and the public key is Q = dP.
 */
std::pair<LongInt, Point> EllipticCurve::generate_keypair(const Point &point) {
    LongInt d = LongInt::get_random(LONG_INT_LEN, random) % curve_order;
    Point Q = point * d;
    return {d, Q};
}


// for each X there is two Y on curve, odd and even.
LongInt EllipticCurve::get_y(const LongInt &x, bool is_odd) const {
    LongInt _a = (x.fast_pow_mod(3, p) + 7) % p; // a = ( ( ( (x^3) mod n ) ax^2 + bx + c ) mod n )
    LongInt y(_a.fast_pow_mod((p + 1) / 4, p)); // y = a^{(n+1)//4} mod n
    if ((y.last_item() & 1) == is_odd)
        return y;
    return -y % p;  // invert Y
}


// for each X there is two Y on curve, odd and even.
Point EllipticCurve::get_point_by_x(const LongInt &x, bool is_odd) const {
    return Point(this, x % p, get_y(x, is_odd));
}

LongInt EllipticCurve::get_curve_order(const Point &_base_point) const {
    if (p == 0)
        return LongInt();
    Point q(*base_point);
    LongInt order(LONG_INT_LEN, 1);
    //Add P to Q repeatedly until obtaining the identity (point at infinity).
    while (!q.get_inf()) {
        q = *base_point + q;
        ++order;
//        std::cerr << curve_order << ' ' << q.to_string() << '\n';
    }
    return order;
}

LongInt EllipticCurve::fast_curve_order(const Point &_base_point) const {
    if (*base_point == _base_point)
        return curve_order;
    return get_curve_order(_base_point);
}

void EllipticCurve::set_curve_order(const Point &_base_point, const LongInt &_curve_order) {
    base_point = new Point(_base_point);
    curve_order = _curve_order;
}

bool EllipticCurve::is_valid() const {
    return (4 * a.fast_pow_mod(3, p) + 27 * b * b) % p != 0;
}

Point EllipticCurve::get_base_point() const {
    return *base_point;
}
