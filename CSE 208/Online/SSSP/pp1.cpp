#include <bits/stdc++.h>
using namespace std;

using ll = long long;
const ll INF = LLONG_MAX;

struct Edge {
    int to;
    ll tt;
    ll w;
};

// Dijkstra function
void dijkstra(int src, const vector<vector<Edge>>& g, int K, vector<int>&par,vector<ll>&total_time, vector<ll>&cost) {
    int n = g.size();

    priority_queue<pair<ll,int>, vector<pair<ll,int>>, greater<pair<ll,int>>> pq;

    cost[src] = 0;
    total_time[src] = 0;
    pq.push({0, src});

    while (!pq.empty()) {
        auto p = pq.top();
        ll d = p.first;
        int u = p.second;
        pq.pop();

        

        // skip outdated state
        if (d != cost[u]) continue;

        for (const auto &e : g[u]) {
            int v = e.to;
            ll w = e.w;

            int wait_time = (u == src ? 0 : 1);
            ll t_cost = w + (e.tt + wait_time) * K;
            
            if (cost[u] + t_cost < cost[v]) {
                cost[v] = cost[u] + t_cost;
                total_time[v] = total_time[u] + e.tt + wait_time;
                par[v]= u;
                pq.push({cost[v], v});
            }
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int K;
    cin>>K;

    int n, m;
    cin >> n >> m;

    vector<vector<Edge>> g(n);

    // input edges
    for (int i = 0; i < m; i++) {
        int a, b;
        ll c;
        ll w;
        cin >> a >> b >> c>>w;

        a--; 
        b--; // remove if input is 0-based

        g[a].push_back({b, c, w});
        // if undirected graph, also uncomment:
        // g[b].push_back({a, c});
    }


    int s, tar;
    cin>>s>>tar;
    s--;
    tar--;
    int src = s;
    vector<int>par(n, -1);
    vector<ll>total_time(n, 0);
    vector<ll>cost(n, INF);

    dijkstra(src, g, K, par, total_time, cost);

    vector<int>path;
    int curr = tar;
    while(curr != -1){
        path.push_back(curr);
        if(curr == src) break;
        curr = par[curr];
    }

    reverse(path.begin(), path.end());

    for(int i : path){
        cout<<i + 1<<" ";
    }

    // output distances
    // for (int i = 0; i < n; i++) {
    //     if (dist[i] == INF) cout << -1;
    //     else cout << dist[i];

    //     if (i != n - 1) cout << " ";
    // }
    // cout << "\n";


    cout<<total_time[tar] <<" "<<cost[tar];
    return 0;
}