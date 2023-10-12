#include "rational.h"

Rational::Rational() {
    SetNumerator(0);
    SetDenominator(1);
}

Rational::Rational(int value) {  // NOLINT
    SetNumerator(value);
    SetDenominator(1);
}

Rational::Rational(int numer, int denom) {
    if (denom == 0) {
        throw RationalDivisionByZero();
    }
    bool is_negative = false;
    if (numer < 0) {
        numer *= -1;
        is_negative ^= true;
    }
    if (denom < 0) {
        denom *= -1;
        is_negative ^= true;
    }
    int gcd = std::gcd(numer, denom);
    if (is_negative) {
        numer *= -1;
    }
    numer_ = numer / gcd;
    denom_ = denom / gcd;
}

int Rational::GetNumerator() const {
    return numer_;
}

int Rational::GetDenominator() const {
    return denom_;
}

void Rational::SetNumerator(int value) {
    *this = Rational(value, denom_);
}

void Rational::SetDenominator(int value) {
    *this = Rational(numer_, value);
}

Rational& operator+=(Rational& lhs, const Rational& rhs) {
    int64_t lcm = std::lcm(lhs.denom_, rhs.denom_);
    lhs.Set(
        static_cast<int64_t>(lhs.numer_) * (lcm / lhs.denom_) + static_cast<int64_t>(rhs.numer_) * (lcm / rhs.denom_),
        lcm);
    return lhs;
}

Rational& operator*=(Rational& lhs, const Rational& rhs) {
    lhs.Set(static_cast<int64_t>(lhs.numer_) * static_cast<int64_t>(rhs.numer_),
            static_cast<int64_t>(lhs.denom_) * static_cast<int64_t>(rhs.denom_));
    return lhs;
}

Rational& operator++(Rational& ratio) {  // faster than += 1
    ratio.numer_ += ratio.denom_;
    return ratio;
}

Rational& operator--(Rational& ratio) {
    ratio.numer_ -= ratio.denom_;
    return ratio;
}

std::istream& operator>>(std::istream& is, Rational& ratio) {
    int numer = 0;
    int denom = 0;
    is >> numer >> denom;
    ratio = Rational(numer, denom);
    return is;
}

void Rational::Set(int64_t numer, int64_t denom) {
    if (denom == 0) {
        throw RationalDivisionByZero();
    }
    int64_t gcd = std::gcd(numer, denom);
    SetNumerator(static_cast<int>(numer / gcd));
    SetDenominator(static_cast<int>(denom / gcd));
}

Rational operator+(const Rational& ratio) {
    return ratio;
}

Rational operator-(const Rational& ratio) {
    return *new Rational(-ratio.GetNumerator(), ratio.GetDenominator());
}

Rational& operator-=(Rational& lhs, const Rational& rhs) {
    return lhs += -rhs;
}

Rational& operator/=(Rational& lhs, const Rational& rhs) {
    return lhs *= *new Rational(rhs.GetDenominator(), rhs.GetNumerator());
}

Rational operator+(const Rational& lhs, const Rational& rhs) {
    Rational ratio = lhs;
    ratio += rhs;
    return ratio;
}

Rational operator-(const Rational& lhs, const Rational& rhs) {
    Rational ratio = lhs;
    ratio -= rhs;
    return ratio;
}

Rational operator*(const Rational& lhs, const Rational& rhs) {
    Rational ratio = lhs;
    ratio *= rhs;
    return ratio;
}

Rational operator/(const Rational& lhs, const Rational& rhs) {
    Rational ratio = lhs;
    ratio /= rhs;
    return ratio;
}

Rational operator++(Rational& ratio, int) {
    Rational ration_copy = ratio;
    ++ratio;
    return ration_copy;
}

Rational operator--(Rational& ratio, int) {
    Rational ration_copy = ratio;
    --ratio;
    return ration_copy;
}

bool operator<(const Rational& lhs, const Rational& rhs) {
    int64_t lcm = std::lcm(lhs.GetDenominator(), rhs.GetDenominator());
    return (static_cast<int64_t>(lhs.GetNumerator()) * (lcm / lhs.GetDenominator()) <
            static_cast<int64_t>(rhs.GetNumerator()) * (lcm / rhs.GetDenominator()));
}

bool operator>(const Rational& lhs, const Rational& rhs) {
    int64_t lcm = std::lcm(lhs.GetDenominator(), rhs.GetDenominator());
    return (static_cast<int64_t>(lhs.GetNumerator()) * (lcm / lhs.GetDenominator()) >
            static_cast<int64_t>(rhs.GetNumerator()) * (lcm / rhs.GetDenominator()));
}

bool operator<=(const Rational& lhs, const Rational& rhs) {
    int64_t lcm = std::lcm(lhs.GetDenominator(), rhs.GetDenominator());
    return (static_cast<int64_t>(lhs.GetNumerator()) * (lcm / lhs.GetDenominator()) <=
            static_cast<int64_t>(rhs.GetNumerator()) * (lcm / rhs.GetDenominator()));
}

bool operator>=(const Rational& lhs, const Rational& rhs) {
    int64_t lcm = std::lcm(lhs.GetDenominator(), rhs.GetDenominator());
    return (static_cast<int64_t>(lhs.GetNumerator()) * (lcm / lhs.GetDenominator()) >=
            static_cast<int64_t>(rhs.GetNumerator()) * (lcm / rhs.GetDenominator()));
}

bool operator==(const Rational& lhs, const Rational& rhs) {
    return lhs.GetNumerator() == rhs.GetNumerator() && lhs.GetDenominator() == rhs.GetDenominator();
}

bool operator!=(const Rational& lhs, const Rational& rhs) {
    return lhs.GetNumerator() != rhs.GetNumerator() || lhs.GetDenominator() != rhs.GetDenominator();
}

std::ostream& operator<<(std::ostream& os, const Rational& ratio) {
    os << ratio.GetNumerator();
    if (ratio.GetDenominator() != 1) {
        os << '/' << ratio.GetDenominator();
    }
    return os;
}