// Source: https://cp-algorithms.com/algebra/fft.html#implementation
#pragma once
#include "santolib/template.hpp"

namespace fft {

using tdouble = long double;
using cd = complex<tdouble>;
const tdouble PI = acos(-1);

void fft(vector<cd>& a, bool invert) {
  tint n = sz(a);
  if (n == 1) return;

  vector<cd> a0(n / 2), a1(n / 2);
  forn(i, n / 2) a0[i] = a[2 * i], a1[i] = a[2 * i + 1];
  fft(a0, invert);
  fft(a1, invert);

  tdouble ang = 2 * PI / n * (invert ? -1 : 1);
  cd w(1), wn(cos(ang), sin(ang));
  forn(i, n / 2) {
    a[i] = a0[i] + w * a1[i];
    a[i + n / 2] = a0[i] - w * a1[i];
    if (invert) {
      a[i] /= 2;
      a[i + n / 2] /= 2;
    }
    w *= wn;
  }
}

vi multiply(const vi& a, const vi& b) {
  vector<cd> fa(all(a)), fb(all(b));
  tint n = 1;
  while (n < sz(a) + sz(b)) n *= 2;
  fa.resize(n), fb.resize(n);

  fft(fa, false), fft(fb, false);
  forn(i, n) fa[i] *= fb[i];
  fft(fa, true);

  vi ans(n);
  forn(i, n) ans[i] = round(fa[i].real());
  return ans;
}

};  // namespace fft
