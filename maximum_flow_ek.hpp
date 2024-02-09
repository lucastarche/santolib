#pragma once
#include "santolib/template.hpp"

class MaxFlow {
 public:
  using Edge = struct {
    tint to;
    tint capacity;
    tint flow;
  };

  using Parent = struct {
    tint node;
    tint edge;
  };

  tint n;
  vector<Edge> edges;
  vector<vi> adj;
  vi dist;
  vector<Parent> parent;

  static const tint INF = 1e18;

  // Finds an augmenting path in the graph
  bool BFS(tint s, tint t) {
    dist.assign(n, -1);
    parent.assign(n, {-1, -1});
    dist[s] = 0;

    queue<tint> q;
    q.push(s);
    while (!q.empty()) {
      tint u = q.front();
      q.pop();

      if (u == t) break;  // Do not explore from the tree
      for (auto next : adj[u]) {
        Edge &edge = edges[next];
        tint v = edge.to;

        // If we can send one more unit of flow, and we didn't visit the edge
        if (edge.capacity - edge.flow > 0 && dist[v] == -1) {
          dist[v] = dist[u] + 1;
          q.push(v);
          parent[v] = {u, next};
        }
      }
    }

    return dist[t] != -1;
  }

  // Sends one unit of flow from source s to sink t
  // Returns the minimum weight in the augmenting path
  tint sendOneFlow(tint s, tint t, tint f = INF) {
    if (s == t) return f;
    Parent p = parent[t];
    tint &capacity = edges[p.edge].capacity, &flow = edges[p.edge].flow;
    tint &reverseFlow = edges[p.edge ^ 1].flow;

    // Use the edge p.node -> t, update bottleneck accordingly
    tint pushed = sendOneFlow(s, p.node, min(f, capacity - flow));

    flow += pushed;         // Update flow from the forward edge
    reverseFlow -= pushed;  // Update flow from the backward edge

    return pushed;
  }

  MaxFlow(tint _n) : n(_n) {
    edges.clear();
    adj.assign(n, {});
  }

  void addEdge(tint from, tint to, tint weight, bool directed = true) {
    if (from == to) return;

    // from -> to
    edges.push_back({to, weight, 0});
    adj[from].push_back(sz(edges) - 1);

    // to -> from
    edges.push_back({from, directed ? 0 : weight, 0});
    adj[to].push_back(sz(edges) - 1);
  }

  tint getMaxFlow(tint s, tint t) {
    tint maxFlow = 0;
    while (BFS(s, t)) {
      tint flow = sendOneFlow(s, t);
      if (flow == 0) break;  // We didn't find any augmenting path
      maxFlow += flow;       // We found more flow :D
    }

    return maxFlow;
  }
};
