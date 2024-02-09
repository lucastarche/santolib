#pragma once
#include "santolib/template.hpp"

template <long long MOD>
struct modint {
  long long v;
  explicit operator long long() const { return v; }

  modint() { v = 0; }
  modint(long long _v) : v((_v % MOD + MOD) % MOD) {}

  modint &operator+=(modint b) {
    v += b.v;
    if (v >= MOD) v -= MOD;
    return *this;
  }

  modint &operator-=(modint b) {
    v -= b.v;
    if (v < 0) v += MOD;
    return *this;
  }

  modint &operator*=(modint b) {
    v *= b.v;
    v %= MOD;
    return *this;
  }

  friend modint pow(modint a, long long p) {
    assert(p >= 0);
    if (p == 0) return 1;
    modint ans = pow(a, p / 2);
    ans *= ans;

    if (p % 2 == 1) ans *= a;
    return ans;
  }

  friend modint inv(modint a) { return pow(a, MOD - 2); }

  friend modint operator+(modint a, modint b) { return a += b; }
  friend modint operator-(modint a, modint b) { return a -= b; }
  friend modint operator*(modint a, modint b) { return a *= b; }
  friend modint operator/(modint a, modint b) { return a *= inv(b); }

  friend bool operator==(modint a, modint b) { return a.v == b.v; }
  friend bool operator!=(modint a, modint b) { return a.v != b.v; }
  friend bool operator<(modint a, modint b) { return a.v < b.v; }

  friend ostream &operator<<(ostream &out, modint x) { return out << x.v; }
};
