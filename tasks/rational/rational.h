#pragma once

#include <stdexcept>
#include <iostream>
#include <numeric>

class RationalDivisionByZero : public std::runtime_error {
public:
    RationalDivisionByZero() : std::runtime_error("RationalDivisionByZero") {
    }
};

class Rational {
public:
    Rational() {
        SetNumerator(0);
        SetDenominator(1);
    }

    Rational(int value) {  // NOLINT
        SetNumerator(value);
        SetDenominator(1);
    }

    Rational(int numer, int denom) {
        if (denom == 0) {
            RationalDivisionByZero();
        }
        int gcd = std::gcd(numer, denom);
        SetNumerator(numer / gcd);
        SetDenominator(denom / gcd);
    }

    int GetNumerator() const {
        return numer_;
    }

    int GetDenominator() const {
        return denom_;
    }

    void SetNumerator(int value) {
        numer_ = value;
    }

    void SetDenominator(int value) {
        if (value == 0) {
            RationalDivisionByZero();
        }
        denom_ = value;
    }

    friend Rational& operator+=(Rational& lhs, const Rational& rhs) {
        int64_t lcm = std::lcm(lhs.denom_, rhs.denom_);
        lhs.Set(static_cast<int64_t>(lhs.numer_) * (lcm / lhs.denom_) +
                    static_cast<int64_t>(rhs.numer_) * (lcm / rhs.denom_),
                lcm);
        return lhs;
    }

    friend Rational& operator*=(Rational& lhs, const Rational& rhs) {
        lhs.Set(static_cast<int64_t>(lhs.numer_) * static_cast<int64_t>(rhs.numer_),
                static_cast<int64_t>(lhs.denom_) * static_cast<int64_t>(rhs.denom_));
        return lhs;
    }

    friend Rational& operator++(Rational& ratio) {  // faster than += 1
        ratio.numer_ += ratio.denom_;
        return ratio;
    }

    friend Rational& operator--(Rational& ratio) {
        ratio.numer_ -= ratio.denom_;
        return ratio;
    }

    friend std::istream& operator>>(std::istream& is, Rational& ratio) {
        is >> ratio.numer_ >> ratio.denom_;
        return is;
    }

private:
    void Set(int64_t numer, int64_t denom) {
        if (denom == 0) {
            RationalDivisionByZero();
        }
        int64_t gcd = std::gcd(numer, denom);
        SetNumerator(static_cast<int>(numer / gcd));
        SetDenominator(static_cast<int>(denom / gcd));
    }

    int numer_;
    int denom_;
};

inline Rational operator+(const Rational& ratio) {
    return ratio;
}

inline Rational operator-(const Rational& ratio) {
    return *new Rational(-ratio.GetNumerator(), ratio.GetDenominator());
}

inline Rational& operator-=(Rational& lhs, const Rational& rhs) {
    return lhs += -rhs;
}

inline Rational& operator/=(Rational& lhs, const Rational& rhs) {
    return lhs *= *new Rational(rhs.GetDenominator(), rhs.GetNumerator());
}

inline Rational operator+(const Rational& lhs, const Rational& rhs) {
    Rational ratio = lhs;
    ratio += rhs;
    return ratio;
}

inline Rational operator-(const Rational& lhs, const Rational& rhs) {
    Rational ratio = lhs;
    ratio -= rhs;
    return ratio;
}

inline Rational operator*(const Rational& lhs, const Rational& rhs) {
    Rational ratio = lhs;
    ratio *= rhs;
    return ratio;
}

inline Rational operator/(const Rational& lhs, const Rational& rhs) {
    Rational ratio = lhs;
    ratio /= rhs;
    return ratio;
}

inline Rational operator++(Rational& ratio, int) {
    Rational ration_copy = ratio;
    ++ratio;
    return ration_copy;
}

inline Rational operator--(Rational& ratio, int) {
    Rational ration_copy = ratio;
    --ratio;
    return ration_copy;
}

inline bool operator<(const Rational& lhs, const Rational& rhs) {
    int64_t lcm = std::lcm(lhs.GetDenominator(), rhs.GetDenominator());
    return (static_cast<int64_t>(lhs.GetNumerator()) * (lcm / lhs.GetDenominator()) <
            static_cast<int64_t>(rhs.GetNumerator()) * (lcm / rhs.GetDenominator()));
}

inline bool operator>(const Rational& lhs, const Rational& rhs) {
    int64_t lcm = std::lcm(lhs.GetDenominator(), rhs.GetDenominator());
    return (static_cast<int64_t>(lhs.GetNumerator()) * (lcm / lhs.GetDenominator()) >
            static_cast<int64_t>(rhs.GetNumerator()) * (lcm / rhs.GetDenominator()));
}

inline bool operator<=(const Rational& lhs, const Rational& rhs) {
    int64_t lcm = std::lcm(lhs.GetDenominator(), rhs.GetDenominator());
    return (static_cast<int64_t>(lhs.GetNumerator()) * (lcm / lhs.GetDenominator()) <=
            static_cast<int64_t>(rhs.GetNumerator()) * (lcm / rhs.GetDenominator()));
}

inline bool operator>=(const Rational& lhs, const Rational& rhs) {
    int64_t lcm = std::lcm(lhs.GetDenominator(), rhs.GetDenominator());
    return (static_cast<int64_t>(lhs.GetNumerator()) * (lcm / lhs.GetDenominator()) >=
            static_cast<int64_t>(rhs.GetNumerator()) * (lcm / rhs.GetDenominator()));
}

inline bool operator==(const Rational& lhs, const Rational& rhs) {
    int64_t lcm = std::lcm(lhs.GetDenominator(), rhs.GetDenominator());
    return (static_cast<int64_t>(lhs.GetNumerator()) * (lcm / lhs.GetDenominator()) ==
            static_cast<int64_t>(rhs.GetNumerator()) * (lcm / rhs.GetDenominator()));
}

inline bool operator!=(const Rational& lhs, const Rational& rhs) {
    int64_t lcm = std::lcm(lhs.GetDenominator(), rhs.GetDenominator());
    return (static_cast<int64_t>(lhs.GetNumerator()) * (lcm / lhs.GetDenominator()) !=
            static_cast<int64_t>(rhs.GetNumerator()) * (lcm / rhs.GetDenominator()));
}

inline std::ostream& operator<<(std::ostream& os, const Rational& ratio) {
    os << ratio.GetNumerator() << ratio.GetDenominator();
    return os;
}