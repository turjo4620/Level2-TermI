#include <bits/stdc++.h>
using namespace std;

using ll = long long;
const ll INF = 1e18;


/*
Online 1 (C1/C2): Mandatory flight

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

Now, you really want to travel in a certain flight and want to keep it in your travel route
mandatorily (mandatory edge). Find the minimum possible travel cost from Dhaka to London in
this scenario. Of course, you can use the coupon as before.

Required complexity
Same complexity as Dijkstra.

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

After that, one line contains three integers x, y, and w, describing the mandatory flight, which
is one of the m international flight connections given. If it is not possible to reach London using
the mandatory flight, print “Not possible”.

Output
Print a single integer:
the minimum possible travel cost from Dhaka to London that includes the mandatory flight.

Example

Input 
5 6
1 2 3
2 3 7
3 5 4
1 4 10
4 3 2
2 5 8
2 3 7
Output
10
*/


struct Edge {
    int to;
    ll w;
};

// Dijkstra function
vector<vector<ll>> dijkstra(int src, const vector<vector<Edge>>& g) {
    int n = g.size();
    // vector<ll> dist(n, INF);
    vector<vector<ll>>dist(n, vector<ll>(2, INF));

    // priority_queue<pair<ll,int>, vector<pair<ll,int>>, greater<pair<ll,int>>> pq;

    //{cost, {node, state}}
    priority_queue<pair<ll, pair<int, int>>,
                    vector<pair<ll, pair<int, int>>>,
                    greater<pair<ll, pair<int, int>>>>pq;


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

            if (dist[u][state] + w < dist[v][state]) {
                dist[v][state] = dist[u][state] + w;
                pq.push({dist[v][state], {v, state}});
            }

            // state 0 -> coupon unused
            //       1 -> used
            if(state == 0){
                ll discounted_price = w / 2;
                if (dist[u][0] + discounted_price < dist[v][1]) {
                dist[v][1] = dist[u][0] + discounted_price;
                pq.push({dist[v][1], {v, 1}});
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
    //reverse graph
    vector<vector<Edge>> rev_g(n);

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
        rev_g[b].push_back({a, c});
    }

    int x, y;
    ll w; 
    cin>>x>>y>>w;
    x--;
    y--;

    int src = 0;
    vector<vector<ll>> dist1 = dijkstra(src, g);
    vector<vector<ll>> distR = dijkstra(n - 1, rev_g);

    ll opt1 = INF, opt2 = INF, opt3 = INF;

    if (dist1[x][1] != INF && distR[y][0] != INF)
        opt1 = dist1[x][1] + w + distR[y][0];
        
    if (dist1[x][0] != INF && distR[y][0] != INF)
        opt2 = dist1[x][0] + (w / 2) + distR[y][0];
        
    if (dist1[x][0] != INF && distR[y][1] != INF)
        opt3 = dist1[x][0] + w + distR[y][1];

    ll min_cost = min({opt1, opt2, opt3});


    if (min_cost >= INF) {
        cout << "Not possible\n";
    } else {
        cout << min_cost << "\n";
    }
    cout << "\n";

    return 0;
}