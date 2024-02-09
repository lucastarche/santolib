#include "santolib/template.hpp"

const tint root = 0, sigma = 26;
struct AhoCorasick {
  using NextTable = array<tint, sigma>;

  struct Node {
    tint fail = root, match = root;
    vi patterns;
    NextTable next;
    Node() { forn(i, sigma) next[i] = -1; }
  };
  using Trie = vector<Node>;

  tint patternAmount = 0;
  Trie trie;
  vi reverseBFS;
  AhoCorasick() { trie = {Node()}; }

  void insert(const string s) {
    tint idx = root;
    for (auto c : s) {
      if (trie[idx].next[c - 'a'] == -1) {
        trie[idx].next[c - 'a'] = sz(trie);
        trie.push_back(Node());
      }

      idx = trie[idx].next[c - 'a'];
    }

    trie[idx].patterns.push_back(patternAmount++);
  }

  void compile() {
    reverseBFS.push_back(root);

    forn(i, sz(reverseBFS)) {
      tint u = reverseBFS[i];
      forn(c, sigma) {
        tint v = trie[u].next[c];
        if (v == -1) continue;
        reverseBFS.push_back(v);

        if (u == root) continue;
        tint fail = u;
        do {
          fail = trie[fail].fail;
        } while (fail != root && trie[fail].next[c] == -1);

        if (trie[fail].next[c] != -1) {
          fail = trie[v].fail = trie[fail].next[c];
          trie[v].match = trie[fail].patterns.empty() ? trie[fail].match : fail;
        }
      }
    }

    reverse(all(reverseBFS));
  }

  tint next(tint idx, char c) {
    while (idx != root and trie[idx].next[c - 'a'] == -1) idx = trie[idx].fail;
    if (trie[idx].next[c - 'a'] != -1) return trie[idx].next[c - 'a'];
    return idx;
  }

  struct Matches {
    vi first, count;
  };

  Matches matchSuffix(const string &s) {
    Matches matches = {vi(patternAmount, -1), vi(patternAmount, 0)};

    tint idx = root;
    forn(i, sz(s)) {
      idx = next(idx, s[i]);
      tint match = trie[idx].match;
      tint patternID = -1;

      if (trie[idx].patterns.size() > 0)
        patternID = trie[idx].patterns.front();
      else if (trie[match].patterns.size() > 0)
        patternID = trie[match].patterns.front();

      if (patternID != -1) {
        if (matches.first[patternID] < 0) matches.first[patternID] = i;
        matches.count[patternID]++;
      }
    }

    return matches;
  }

  Matches getMatches(const string &s) {
    Matches matches = matchSuffix(s);
    for (auto u : reverseBFS) {
      if (trie[u].patterns.empty()) continue;
      tint x = trie[u].patterns.front();
      if (matches.first[x] == -1) continue;

      for (auto pattern : trie[u].patterns) {
        matches.first[pattern] = matches.first[x];
        matches.count[pattern] = matches.count[x];
      }

      tint match = trie[u].match;
      if (trie[match].patterns.empty()) continue;

      tint y = trie[match].patterns.front();
      if (matches.first[y] == -1 or matches.first[y] > matches.first[x]) {
        matches.first[y] = matches.first[x];
      }
      matches.count[y] += matches.count[x];
    }

    return matches;
  }
};
