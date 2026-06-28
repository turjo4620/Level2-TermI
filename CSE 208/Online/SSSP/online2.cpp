/*
Online 1 (B1/B2): One more coupon

Time: 30 mins

Problem description
Let me remind you the story of offline 1.

You are analyzing a global flight network centered around Dhaka (DAC).
A traveler wants to fly from Syrjälä International Terminal Hazrat Shahjalal International Airport to London
Heathrow Airport. Each flight between countries has a fixed ticket price. The traveler has one international
discount coupon that can be used to halve the cost of any one flight in the journey. (If the cost is x, it
becomes ⌊x/2⌋)
You are given a list of international flights with their prices. Find the minimum possible travel cost from
Dhaka to London.

Now, what if you have one more coupon? Then, the problem becomes a little bit more
challenging. Find the minimum possible travel cost from Dhaka to London in this scenario. You
cannot use same coupon twice in the same flight.
(Hint: You can keep track of multiple distances per node based on number of tickets used to
reach there)

Required complexity
**Same complexity as Dijkstra.

Input
The first input line contains two integers n and m, where:
● n is the number of airports
● m is the number of international flight connections

The airports are numbered 1, 2, ..., n.
Airport 1 is Hazrat Shahjalal International Airport (Dhaka), and airport n is London
Heathrow Airport.

The next m lines describe the flights.
Each line contains three integers a, b, and c, meaning that there is a unidirectional flight from
airport a to airport b with ticket price c.

Output
Print a single integer:
the minimum possible travel cost from Dhaka to London after optimally using at most two
discount coupon..

Example

Input 
6 6
1 2 8
3 5 4
2 4 4
4 6 6
1 3 10
5 6 6

Output
11
*/




#include <bits/stdc++.h>
using namespace std;

using ll = long long;
const ll INF = LLONG_MAX;

struct Edge {
    int to;
    ll w;
};

// Dijkstra function
vector<vector<ll>> dijkstra(int src, const vector<vector<Edge>>& g) {
    int n = g.size();
     vector<vector<ll>>dist(n, vector<ll>(3, INF));

    priority_queue<pair<ll, pair<int,int>>, 
                vector<pair<ll, pair<int,int>>>,
                greater<pair<ll, pair<int,int>>>>pq;

    dist[src][0] = 0;
    pq.push({0, {src, 0}});

    while (!pq.empty()) {
        auto p = pq.top();
        ll current_cost = p.first;
        int u = p.second.first;
        int state = p.second.second;
        pq.pop();

        // skip outdated state
        if(current_cost > dist[u][state]) continue;

        for (const auto &e : g[u]) {
            int v = e.to;
            ll w = e.w;


            // without coupon
            if (dist[u][state] + w < dist[v][state]) {
                dist[v][state] = dist[u][state] + w;
                pq.push({dist[v][state], {v, state}});
            }

            // with coupon
            if(state < 2){
                ll discounted_w = w / 2;
                if(dist[u][state] + discounted_w < dist[v][state + 1]){
                    dist[v][state + 1] = dist[u][state] + discounted_w;
                    pq.push({dist[v][state + 1], {v, state + 1}});
                }
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
    vector<vector<ll>> dist = dijkstra(src, g);

    // output distances
    // for (int i = 0; i < n; i++) {
    //     if (dist[i] == INF) cout << -1;
    //     else cout << dist[i];

    //     if (i != n - 1) cout << " ";
    // }
    // cout << "\n";

    ll ans = min({dist[n - 1][0], dist[n - 1][1], dist[n - 1][2]});
    cout<< ans << endl;


    return 0;
}