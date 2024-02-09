#pragma once
#include "santolib/template.hpp"

class MaxFlow {
 public:
  struct Edge {
    tint to, capacity, flow;
  };

  tint n;
  vector<Edge> edges;
  vector<vi> adj;
  vi dist, last;

  static const tint INF = 1e18;

  bool BFS(tint s, tint t) {
    dist.assign(n, -1);
    dist[s] = 0;

    queue<tint> q;
    q.push(s);
    while (!q.empty()) {
      tint u = q.front();
      q.pop();
      if (u == t) break;

      for (auto next : adj[u]) {
        auto &[v, cap, flow] = edges[next];
        if (cap - flow > 0 && dist[v] == -1) dist[v] = dist[u] + 1, q.push(v);
      }
    }

    return dist[t] != -1;
  }

  tint DFS(tint u, tint t, tint f = INF) {
    if ((u == t) || (f == 0)) return f;

    for (tint &i = last[u]; i < sz(adj[u]); i++) {
      auto &[v, cap, flow] = edges[adj[u][last[u]]];
      if (dist[v] != dist[u] + 1) continue;
      if (tint pushed = DFS(v, t, min(f, cap - flow))) {
        auto &reverseFlow = edges[adj[u][last[u]] ^ 1].flow;
        flow += pushed, reverseFlow -= pushed;
        return pushed;
      }
    }

    return 0;
  }

  MaxFlow(tint _n) : n(_n) {
    edges.clear();
    adj.assign(n, {});
  }

  void addEdge(tint from, tint to, tint capacity, bool directed = true) {
    if (from == to) return;
    edges.push_back({to, capacity, 0});
    adj[from].push_back(sz(edges) - 1);
    edges.push_back({from, directed ? 0LL : capacity, 0});
    adj[to].push_back(sz(edges) - 1);
  }

  tint getMaxFlow(tint s, tint t) {
    tint maxFlow = 0;
    while (BFS(s, t)) {
      last.assign(n, 0);
      while (tint f = DFS(s, t)) maxFlow += f;
    }
    return maxFlow;
  }
};
