#pragma once
#include "santolib/template.hpp"

struct SuffixAutomaton {
  static constexpr tint root = 0;
  static constexpr tint null = -1;
  static constexpr tint SIGMA = 26;
  using Next = array<tint, 26>;

  tint lastState = root;
  vector<Next> next;
  vi link, cloneOf, length;
  vector<vi> byLen;

  tint addState(tint len, tint clone = null) {
    tint s = sz(next);

    next.emplace_back();
    link.push_back(null);
    cloneOf.push_back(clone);
    length.push_back(len);

    if (len > length[lastState]) lastState = s;
    if (len >= sz(byLen)) byLen.resize(len + 1);
    byLen[len].push_back(s);

    if (clone != null) {
      next[s] = next[clone];
      link[s] = link[clone];
    } else {
      fill(all(next[s]), null);
    }

    return s;
  }

  void extend(char c) {
    tint i = c - 'a';
    tint s = lastState;
    tint newState = addState(length[s] + 1);

    for (; s != null && next[s][i] == null; s = link[s]) next[s][i] = newState;
    if (s == null) return void(link[newState] = root);

    tint t = next[s][i];
    if (length[t] == length[s] + 1) return void(link[newState] = t);

    tint clone = addState(length[s] + 1, t);
    for (; s != null && next[s][i] == t; s = link[s]) next[s][i] = clone;
    link[t] = link[newState] = clone;
  }

  SuffixAutomaton() { addState(0); }
};
