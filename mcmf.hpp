#pragma once
#include "santolib/template.hpp"

// Min Cost - Max Flow
class MCMF {
 public:
  struct Edge {
    tint to, cap, flow, cost;
  };

  tint n, totalCost;
  vector<bool> enqueued;
  vector<Edge> edges;
  vector<vi> adj;
  vi dist, last;

  static constexpr tint INF = 1e18;
  bool SPFA(tint s, tint t) {
    queue<tint> q({s});
    dist.assign(n, INF), enqueued[s] = true, dist[s] = 0;

    while (!q.empty()) {
      tint u = q.front();
      q.pop(), enqueued[u] = false;

      for (auto next : adj[u]) {
        auto &[v, cap, flow, cost] = edges[next];
        if (cap - flow > 0 && dist[v] > dist[u] + cost) {
          dist[v] = dist[u] + cost;
          if (not enqueued[v]) enqueued[v] = true, q.push(v);
        }
      }
    }

    return dist[t] != INF;
  }

  tint DFS(tint u, tint t, tint f = INF) {
    if ((u == t) || (f == 0)) return f;

    enqueued[u] = true;
    for (tint &i = last[u]; i < sz(adj[u]); i++) {
      auto &[v, cap, flow, cost] = edges[adj[u][last[u]]];
      if (enqueued[v] || dist[v] != dist[u] + cost) continue;

      if (tint pushed = DFS(v, t, min(f, cap - flow))) {
        auto &reverseFlow = edges[adj[u][last[u]] ^ 1].flow;
        flow += pushed, reverseFlow -= pushed;
        totalCost += cost * pushed;

        enqueued[u] = false;
        return pushed;
      }
    }
    enqueued[u] = false;

    return 0;
  }

  MCMF(tint _n) : n(_n) {
    totalCost = 0;
    edges.clear();
    enqueued.resize(n, false);
    adj.assign(n, {});
  }

  void addEdge(tint from, tint to, tint cap, tint cost, bool directed = true) {
    if (from == to) return;
    adj[from].push_back(sz(edges));
    edges.push_back({to, cap, 0, cost});
    adj[to].push_back(sz(edges));
    edges.push_back({from, 0, 0, -cost});
    if (not directed) addEdge(to, from, cap, cost);
  }

  pii getMaxFlow(tint s, tint t) {
    tint maxFlow = 0;
    while (SPFA(s, t)) {
      last.assign(n, 0);
      while (tint f = DFS(s, t)) maxFlow += f;
    }
    return {maxFlow, totalCost};
  }

  // Returns -1 if impossible
  tint getCostKFlow(tint s, tint t, tint k) {
    tint flowSent = 0;
    while (SPFA(s, t) && flowSent != k) {
      last.assign(n, 0);
      while (tint f = DFS(s, t, k - flowSent)) flowSent += f;
    }

    return (k == flowSent) ? totalCost : -1;
  }
};
