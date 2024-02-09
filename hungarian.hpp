#pragma once
#include "santolib/template.hpp"

tint hungarian(const vector<vi> &A, vector<pii> *matching) {
  const tint INF = 1e18;
  tint n = sz(A) - 1, m = sz(A[0]) - 1;
  vi u(n + 1), v(m + 1), p(m + 1), way(m + 1);
 
  forsn(i, 1, n + 1) {
    p[0] = i;
 
    tint j0 = 0;
    vi minv(m + 1, INF);
    vector<bool> used(m + 1, false);
 
    do {
      used[j0] = true;
      tint i0 = p[j0], delta = INF, j1;
 
      forsn(j, 1, m + 1) {
        if (not used[j]) {
          tint curr = A[i0][j] - u[i0] - v[j];
          if (curr < minv[j]) minv[j] = curr, way[j] = j0;
          if (minv[j] < delta) delta = minv[j], j1 = j;
        }
      }
 
      forn(j, m + 1) {
        if (used[j])
          u[p[j]] += delta, v[j] -= delta;
        else
          minv[j] -= delta;
      }
      j0 = j1;
    } while (p[j0] != 0);
 
    do {
      tint j1 = way[j0];
      p[j0] = p[j1];
      j0 = j1;
    } while (j0);
  }
 
  if (matching != nullptr) forsn(i, 1, m + 1) {
      matching->emplace_back(p[i], i);
    }
 
  return -v[0];
}
