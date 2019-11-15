//
// Created by ky3he4ik on 08/11/19.
//

#include "LongInt.h"
#include <iostream>

template<UINT bits_num>
LongInt<bits_num>::LongInt() {
    FOR_IND(i)value[i] = 0;
}

template<UINT bits_num>
LongInt<bits_num>::LongInt(UINT init) {
    value[LAST] = init;
}


template<UINT bits_num>
LongInt<bits_num>::LongInt(const LongInt<bits_num> &other) {
    FOR_IND(i)value[i] = other[i];
}

template<UINT bits_num>
LongInt<bits_num>::LongInt(const std::string &str, int radix) {
    for (auto &c: str) {
        int i = ctoi(c);
        if (i != -1 && i < radix)
            *this = *this * radix + i;
    }
}

template<UINT bits_num>
template<UINT old_bits_num>
LongInt<bits_num> LongInt<bits_num>::changeLen(const LongInt<old_bits_num> &other) {
    LongInt<bits_num> res;
    UINT len = res.get_len();
    UINT o_len = other.get_len();
    if (len < o_len)
        for (UINT i = 0; i < len; i++)
            res[i] = other[i + o_len - len];
    else
        for (UINT i = 0; i < o_len; i++)
            res[i + len - o_len] = other[i];
    return res;
}


template<UINT bits_num>
bool
LongInt<bits_num>::mult_cmp(const LongInt<bits_num> &a, const LongInt<bits_num> &b, const LongInt<bits_num> &third) {
    LongInt o_r((UINT) 0);
    std::pair<LongInt<bits_num>, bool> res;
    bool overflow = false;
    UINT len = a.get_len();
    for (UINT i = 0; i < len && !overflow; i++) {
        res = shift_overflow(a * b[i], (len - i - 1) * BITS_BASE);
        overflow |= res.first + o_r < o_r || ((res.first == 0) != (b[i] == 0)) || res.second;
        o_r += res.first;
        if (o_r > third)
            return true;
    }
    return overflow;
}


template<UINT bits_num>
std::pair<LongInt<bits_num>, bool> LongInt<bits_num>::shift_overflow(const LongInt<bits_num> &a, UINT other) {
    UINT shift_b = other / BITS_BASE, shift_s = other % BITS_BASE;
    LongInt res;
    bool of = false;
    UINT len = a.get_len();
    FOR_IND_REVERSE(i) {
        UINT ni = i + shift_b;
        if (ni < len) {
            res[i] = a[ni] << shift_s;
            if (ni != len - 1 && shift_s != 0)
                res[i] |= res[ni + 1] >> (BITS_BASE - shift_s);
        } else
            res[i] = 0;
    }
    if (shift_s)
        for (UINT i = 0; i < shift_b + 1; i++)
            of |= a[i] >> (BITS_BASE - shift_s);
    else
        for (UINT i = 0; i < shift_b; i++)
            of |= a[i];

    return std::make_pair(res, of);
}


template<UINT bits_num>
int LongInt<bits_num>::ctoi(char c) {
    if (c <= '9' && c >= '0')
        return c - '0';
    if (c <= 'Z' && c >= 'A')
        return c - 'A' + 10;
    if (c <= 'z' && c >= 'a')
        return c - 'a' + 10;
    return -1;
}

template<UINT bits_num>
char LongInt<bits_num>::itoc(int i) {
    return (i < 10) ? i + '0' : i + 'A' - 10;
}


template<UINT bits_num>
std::string LongInt<bits_num>::to_string(UINT radix, bool group) const {
    std::string res;
    LongInt tmp(*this);
    UINT l = 1;
    while (tmp != 0) {
        res.push_back(itoc(tmp % radix));
        tmp /= (radix);
        if (group && l % 4 == 0 && l != get_bits_count())
            res.push_back('_');
        l++;
    }
    if (!res.empty() && res.back() == '_')
        res.pop_back();
    reverse(res.begin(), res.end());
    if (res.empty())
        res = "0";
    return res;
}

template<UINT bits_num>
UINT LongInt<bits_num>::last_item() const {
    return value[LAST];
}

template<UINT bits_num>
bool LongInt<bits_num>::get_bit(UINT pos) const {
    return (value[pos / BITS_BASE] >> ((BITS_BASE - (pos % BITS_BASE) - 1) % BITS_BASE)) & 1;
}

template<UINT bits_num>
UINT LongInt<bits_num>::get_len() const {
    return len;
}


template<UINT bits_num>
void LongInt<bits_num>::set_bit(UINT pos, bool val) {
    UINT mask = ((UINT) -1) - ((UINT) 1 << (pos % BITS_BASE));
    value[pos / BITS_BASE] &= mask;
    value[pos / BITS_BASE] |= (val << (pos % BITS_BASE));
}

template<UINT bits_num>
UINT LongInt<bits_num>::get_bits_count() const {
    return len * BITS_BASE;
}


template<UINT bits_num>
LongInt<bits_num> LongInt<bits_num>::operator+(const LongInt<bits_num> &other) const {
    LongInt res(*this);
    res += other;
    return res;
}

template<UINT bits_num>
LongInt<bits_num> LongInt<bits_num>::operator+(UINT other) const {
    LongInt res(*this);
    res += other;
    return res;
}

template<UINT bits_num>
LongInt<bits_num> &LongInt<bits_num>::operator+=(const LongInt<bits_num> &other) {
    uint64_t buf = 0;
    FOR_IND_REVERSE(i) {
        buf = buf + other[i] + (uint64_t) value[i];
        value[i] = buf;
        buf >>= BITS_BASE;
    }
    return *this;
}

template<UINT bits_num>
LongInt<bits_num> &LongInt<bits_num>::operator+=(UINT other) {
    uint64_t buf = 0;
    FOR_IND_REVERSE(i) {
        buf = buf + (uint64_t) value[i];
        if (i == len - 1)
            buf += other;
        value[i] = buf;
        buf >>= BITS_BASE;
    }
    return *this;
}


template<UINT bits_num>
LongInt<bits_num> LongInt<bits_num>::operator-(const LongInt<bits_num> &other) const {
    LongInt res(*this);
    res -= other;
    return res;
}

template<UINT bits_num>
LongInt<bits_num> LongInt<bits_num>::operator-(UINT other) const {
    LongInt res(*this);
    res -= other;
    return res;
}

template<UINT bits_num>
LongInt<bits_num> &LongInt<bits_num>::operator-=(const LongInt<bits_num> &other) {
    uint64_t buf = 0;
    FOR_IND_REVERSE(i) {
        buf = (uint64_t) value[i] - other[i] - buf;
        value[i] = buf;
        buf = (buf >> BITS_BASE) & (UINT)1;
    }
    return *this;
}

template<UINT bits_num>
LongInt<bits_num> &LongInt<bits_num>::operator-=(UINT other) {
    uint64_t buf = 0;
    FOR_IND_REVERSE(i) {
        buf = (uint64_t) value[i] - buf;
        if (i == len - 1)
            buf -= other;
        value[i] = buf;
        buf = (buf >> BITS_BASE) & (UINT)1;
    }
    return *this;
}


template<UINT bits_num>
LongInt<bits_num> LongInt<bits_num>::operator/(const LongInt<bits_num> &other) {
    if (other == 0)
        throw std::invalid_argument("Cannot divide by zero");
    LongInt ub((UINT) 0);
    --ub;
    LongInt lb((UINT) 0);
    // algorithm from wiki doesn't work correctly. Using binary search instead
    while (ub - lb > 1 && ub > lb) {
        LongInt mid = ((ub - lb) >> 1) + lb;
        bool dbg = bits_num == 256 && other[LAST] == 1 && mid[1] == 0 && mid[0] == 1;
//        if (bits_num == 256 && other[LAST] == 1 && mid[0] <= 1)
//            std::cerr << ub.to_string() << ' ' << lb.to_string() << ' ' << mid.to_string() << '\n';
        if (mult_cmp(other, mid, *this))
            ub = mid;
        else
            lb = mid;
    }
    return lb;
}

template<UINT bits_num>
LongInt<bits_num> LongInt<bits_num>::operator/(UINT other) const {
    LongInt res(*this);
    res /= other;
    return res;
}

template<UINT bits_num>
LongInt<bits_num> &LongInt<bits_num>::operator/=(const LongInt<bits_num> &other) {
    if (other == 0)
        throw std::invalid_argument("Cannot divide by zero");

    LongInt<bits_num> res = *this / other;
    FOR_IND(i)value[i] = res[i];
    return *this;
}


template<UINT bits_num>
LongInt<bits_num> &LongInt<bits_num>::operator/=(UINT other) {
    uint64_t buf = 0;
    FOR_IND(i) {
        buf = (buf << BITS_BASE) + value[i];
        value[i] = buf / other;
        buf = buf % other;
    }
    // not the best solution but it's easy to write and it works!
    return *this;
}


template<UINT bits_num>
LongInt<bits_num> LongInt<bits_num>::operator*(const LongInt<bits_num> &other) const {
    LongInt<bits_num> res;
    FOR_IND(i) {
        res += (other * value[i]) << (bits_num - (i + 1) * BITS_BASE);
    }
    return res;
}

template<UINT bits_num>
LongInt<bits_num> LongInt<bits_num>::operator*(UINT other) const {
    LongInt res;
    uint64_t buf = 0;
    FOR_IND_REVERSE(i) {
        buf += value[i] * (uint64_t) other;
        res[i] = buf;
        buf >>= BITS_BASE;
    }
    return res;
}


template<UINT bits_num>
LongInt<bits_num> &LongInt<bits_num>::operator*=(const LongInt<bits_num> &other) {
    LongInt<bits_num> res = *this * other;
    FOR_IND(i)value[i] = res[i];
    return *this;
}

template<UINT bits_num>
LongInt<bits_num> &LongInt<bits_num>::operator*=(UINT other) {
    uint64_t buf = 0;
    FOR_IND_REVERSE(i) {
        buf = value[i] * (uint64_t) other + buf;
        value[i] = buf;
        buf >>= BITS_BASE;
    }
    return *this;
}


template<UINT bits_num>
LongInt<bits_num> LongInt<bits_num>::operator%(const LongInt<bits_num> &other) const {
    if (other == 0)
        throw std::invalid_argument("Cannot divide by zero");

    LongInt remainder;
    // algorithm from wiki
    for (UINT i = get_bits_count() - 1; i < get_bits_count(); i--) {
        remainder <<= 1;
        remainder |= get_bit(get_bits_count() - i - 1);
//        res.set_bit(i, remainder >= other);
        if (remainder >= other) {
            remainder -= other;
        }
    }
    return remainder;
//    LongInt a(*this);
//    LongInt d = a / other;
//    return *this - other * d;
}

template<UINT bits_num>
UINT LongInt<bits_num>::operator%(UINT other) const {
    uint64_t buf = 0;
    FOR_IND(i) {
        buf = ((buf << BITS_BASE) + value[i]) % other;
    }
    return (UINT) buf;
}

template<UINT bits_num>
LongInt<bits_num> LongInt<bits_num>::operator%=(const LongInt<bits_num> &other) {
    if (other == 0)
        throw std::invalid_argument("Cannot divide by zero");
    LongInt<bits_num> res = *this % other;
    FOR_IND(i)value[i] = res[i];
    return *this;
}

template<UINT bits_num>
LongInt<bits_num> LongInt<bits_num>::operator%=(UINT other) {
    uint64_t buf = 0;
    FOR_IND(i) {
        buf = ((buf << BITS_BASE) + value[i]) % other;
        value[i] = 0;
    }
    value[len - 1] = buf;
    return this;
}

template<UINT bits_num>
bool LongInt<bits_num>::operator==(const LongInt<bits_num> &other) const {
    FOR_IND(i)
        if (value[i] != other[i])
            return false;
    return true;
}

template<UINT bits_num>
bool LongInt<bits_num>::operator==(UINT other) const {
    for (UINT i = 0; i < len - 1; i++)
        if (value[i] != 0)
            return false;
    return value[len - 1] == other;
}


template<UINT bits_num>
bool LongInt<bits_num>::operator!=(const LongInt<bits_num> &other) const {
    FOR_IND(i)
        if (value[i] != other[i])
            return true;
    return false;
}

template<UINT bits_num>
bool LongInt<bits_num>::operator!=(UINT other) const {
    for (UINT i = 0; i < len - 1; i++)
        if (value[i] != 0)
            return true;
    return value[len - 1] != other;
}


template<UINT bits_num>
bool LongInt<bits_num>::operator>(const LongInt<bits_num> &other) const {
    FOR_IND(i)
        if (value[i] > other[i])
            return true;
        else if (value[i] < other[i])
            return false;
    return false;
}

template<UINT bits_num>
bool LongInt<bits_num>::operator>(UINT other) const {
    for (UINT i = 0; i < len - 1; i++)
        if (value[i])
            return true;
    return value[len - 1] > other;
}


template<UINT bits_num>
bool LongInt<bits_num>::operator>=(const LongInt<bits_num> &other) const {
    FOR_IND(i)
        if (value[i] > other[i])
            return true;
        else if (value[i] < other[i])
            return false;
    return value[len - 1] == other[len - 1];
}

template<UINT bits_num>
bool LongInt<bits_num>::operator>=(UINT other) const {
    for (UINT i = 0; i < len - 1; i++)
        if (value[i])
            return true;
    return value[len - 1] >= other;
}


template<UINT bits_num>
bool LongInt<bits_num>::operator<(const LongInt<bits_num> &other) const {
    FOR_IND(i)
        if (value[i] < other[i])
            return true;
        else if (value[i] > other[i])
            return false;
    return false;
}

template<UINT bits_num>
bool LongInt<bits_num>::operator<(UINT other) const {
    for (UINT i = 0; i < len - 1; i++)
        if (value[i])
            return false;
    return value[len - 1] < other;
}


template<UINT bits_num>
bool LongInt<bits_num>::operator<=(const LongInt<bits_num> &other) const {
    FOR_IND(i)
        if (value[i] < other[i])
            return true;
        else if (value[i] > other[i])
            return false;
    return last_item() <= other.last_item();
}

template<UINT bits_num>
bool LongInt<bits_num>::operator<=(UINT other) const {
    for (UINT i = 0; i < len - 1; i++)
        if (value[i])
            return false;
    return value[len - 1] <= other;
}


template<UINT bits_num>
LongInt<bits_num> LongInt<bits_num>::operator|(const LongInt<bits_num> &other) const {
    LongInt<bits_num> res;
    FOR_IND(i)res[i] = value[i] | other[i];
    return res;
}

template<UINT bits_num>
LongInt<bits_num> LongInt<bits_num>::operator|(UINT other) const {
    LongInt<bits_num> res(*this);
    res[len - 1] |= other;
    return res;
}

template<UINT bits_num>
LongInt<bits_num> &LongInt<bits_num>::operator|=(const LongInt<bits_num> &other) {
    FOR_IND(i) value[i] |= other[i];
    return *this;
}

template<UINT bits_num>
LongInt<bits_num> &LongInt<bits_num>::operator|=(UINT other) {
    value[LAST] |= other;
    return *this;
}


template<UINT bits_num>
LongInt<bits_num> LongInt<bits_num>::operator&(const LongInt<bits_num> &other) const {
    LongInt<bits_num> res;
    FOR_IND(i)res[i] = value[i] & other[i];
    return res;
}

template<UINT bits_num>
LongInt<bits_num> LongInt<bits_num>::operator&(UINT other) const {
    LongInt<bits_num> res;
    res[len - 1] = value[len - 1] & other;
    return res;
}

template<UINT bits_num>
LongInt<bits_num> &LongInt<bits_num>::operator&=(const LongInt<bits_num> &other) {
    FOR_IND(i)value[i] &= other[i];
    return *this;
}

template<UINT bits_num>
LongInt<bits_num> &LongInt<bits_num>::operator&=(UINT other) {
    FOR_IND(i)value[i] = 0;
    value[LAST] &= other;
    return *this;
}


template<UINT bits_num>
LongInt<bits_num> LongInt<bits_num>::operator^(const LongInt<bits_num> &other) const {
    LongInt<bits_num> res;
    FOR_IND(i)res[i] = value[i] ^ other[i];
    return res;
}

template<UINT bits_num>
LongInt<bits_num> LongInt<bits_num>::operator^(UINT other) const {
    LongInt<bits_num> res(*this);
    res[len - 1] ^= other;
    return res;
}


template<UINT bits_num>
LongInt<bits_num> &LongInt<bits_num>::operator^=(const LongInt<bits_num> &other) {
    FOR_IND(i)value[i] ^= other[i];
    return *this;
}

template<UINT bits_num>
LongInt<bits_num> &LongInt<bits_num>::operator^=(UINT other) {
    value[LAST] ^= other;
    return *this;
}


template<UINT bits_num>
LongInt<bits_num> LongInt<bits_num>::operator<<(UINT other) const {
    LongInt<bits_num> res(*this);
    res <<= other;
    return res;
}

template<UINT bits_num>
LongInt<bits_num> LongInt<bits_num>::operator<<=(UINT other) {
    UINT shift_b = other / BITS_BASE, shift_s = other % BITS_BASE;
    FOR_IND(i) {
        UINT ni = i + shift_b;
        if (ni < len) {
            value[i] = value[ni] << shift_s;
            if (ni != len - 1 && shift_s != 0)
                value[i] |= value[ni + 1] >> (BITS_BASE - shift_s);
        } else
            value[i] = 0;
    }
    return *this;
}

template<UINT bits_num>
LongInt<bits_num> LongInt<bits_num>::operator>>(UINT other) const {
    LongInt<bits_num> res(*this);
    res >>= other;
    return res;
}

template<UINT bits_num>
LongInt<bits_num> LongInt<bits_num>::operator>>=(UINT other) {
    UINT shift_b = other / BITS_BASE, shift_s = other % BITS_BASE;
    FOR_IND_REVERSE(i) {
        UINT ni = i - shift_b;
        if (ni <= i) {
            value[i] = value[ni] >> shift_s;
            if (ni != 0 && shift_s != 0)
                value[i] |= value[ni - 1] << (BITS_BASE - shift_s);
        } else
            value[i] = 0;
    }
    return *this;
}

template<UINT bits_num>
LongInt<bits_num> LongInt<bits_num>::operator~() const {
    LongInt<bits_num> res;
    FOR_IND(i)res[i] = ~value[i];
    return res;
}

template<UINT bits_num>
LongInt<bits_num> &LongInt<bits_num>::operator++() {
    *this += 1;
    return *this;
}

template<UINT bits_num>
const LongInt<bits_num> LongInt<bits_num>::operator++(int) {
    LongInt<bits_num> res(*this);
    *this += 1;
    return res;
}

template<UINT bits_num>
LongInt<bits_num> &LongInt<bits_num>::operator--() {
    *this -= 1;
    return *this;
}

template<UINT bits_num>
const LongInt<bits_num> LongInt<bits_num>::operator--(int) {
    LongInt<bits_num> res(*this);
    *this -= 1;
    return res;
}

template<UINT bits_num>
LongInt<bits_num> &LongInt<bits_num>::operator=(UINT other) {
    FOR_IND(i)value[i] = 0;
    value[LAST] = other;
    return *this;
}

template<UINT bits_num>
LongInt<bits_num> &LongInt<bits_num>::operator=(int other) {
    FOR_IND(i)value[i] = 0;
    for (UINT ps = sizeof(other) / BITS_BASE; ps != 0; ps--) {
        value[ps] = ((unsigned) other >> (BITS_BASE * ps)) & ((UINT) -1);
    }
    value[LAST] = (unsigned) other & ((UINT) -1);
    return *this;
}

template<UINT bits_num>
UINT &LongInt<bits_num>::operator[](UINT index) {
    return this->value[index];
}

template<UINT bits_num>
UINT LongInt<bits_num>::operator[](UINT index) const {
    return this->value[index];
}