#include <bits/stdc++.h>
using namespace std;

using ll = long long;
const ll INF = LLONG_MAX;

struct Edge {
    int to;
    ll w;
};

// Dijkstra function
vector<ll> dijkstra(int src, const vector<vector<Edge>>& g) {
    int n = g.size();
    vector<ll> dist(n, INF);

    priority_queue<pair<ll,int>, vector<pair<ll,int>>, greater<pair<ll,int>>> pq;

    dist[src] = 0;
    pq.push({0, src});

    while (!pq.empty()) {
        auto p = pq.top();
        ll d = p.first;
        int u = p.second;
        pq.pop();

        // skip outdated state
        if (d != dist[u]) continue;

        for (const auto &e : g[u]) {
            int v = e.to;
            ll w = e.w;

            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }

    return dist;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<Edge>> g(n);

    // input edges
    for (int i = 0; i < m; i++) {
        int a, b;
        ll c;
        cin >> a >> b >> c;

        a--; 
        b--; // remove if input is 0-based

        g[a].push_back({b, c});
        // if undirected graph, also uncomment:
        // g[b].push_back({a, c});
    }

    int src = 0;
    vector<ll> dist = dijkstra(src, g);

    // output distances
    for (int i = 0; i < n; i++) {
        if (dist[i] == INF) cout << -1;
        else cout << dist[i];

        if (i != n - 1) cout << " ";
    }
    cout << "\n";

    return 0;
}