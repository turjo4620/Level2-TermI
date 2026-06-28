/*
🚌 Online on Single Source Shortest Path
Problem Statement
You are a National Tuition Lord, constantly traveling between cities to tutor students. Since your time is valuable, every hour spent traveling or waiting costs you money. You want to find the most cost-effective route from one city to another.

Here’s how the cost works:

You earn K units of money per hour, so every hour spent traveling or waiting is a lost opportunity—we call this opportunity cost.
Every time you wait for a connecting bus in a city (excluding your starting city), you spend 1 extra hour waiting.
Your goal
Given: - Travel times and costs between cities, - Your hourly earning rate K, - A starting city S and a destination city D,

Find the path that minimizes your total cost, where:

Total cost = Travel Cost + Opportunity Cost

Opportunity Cost = K × (Total Time Spent)
Total Time Spent = Sum of travel times + 1 hour for each transfer between buses

Input Format
K                  # hourly opportunity cost
N                  # number of cities
X                  # number of roads
u1 v1 t1 c1        # X lines, each with: source, destination, travel time, travel cost
...
S                  # starting city
D                  # destination city
ui, vi: cities connected by a road
ti: time in hours to travel that road
ci: travel cost
All roads are bidirectional
Output Format
Print the most optimal path and its associated total time and total cost:

S->[C1->C2->...->Cy]->D T C
Where: - T: total time taken (including 1-hour waits between buses) - C: total cost (travel + time cost)

If there is no path from S to D, print:

Error
Sample Input
1000
4
5
1 2 1 2500
1 3 1 3000
1 4 2 7000
2 4 1 3000
3 4 1 2000
1
4
Sample Output
1->3->4 3 8000
Explanation:
- Path taken: 1 → 3 → 4
- Time: 1 (to 3) + 1 (wait) + 1 (to 4) = 3 hours
- Travel cost = 3000 + 2000 = 5000
- Opportunity cost = 3 × 1000 = 3000
- Total cost = 8000

*/ 

#include <bits/stdc++.h>
using namespace std;

using ll = long long;
const ll INF = 1e18;

ll K;

struct Edge {
    int to;
    ll tt;
    ll w;
};

// Dijkstra function
vector<ll> dijkstra(int src, const vector<vector<Edge>>& g, vector<ll> &parent, vector<ll> &total_time) {
    int n = g.size();
    vector<ll> dist(n, INF);
    vector<ll> min_cost(n, INF);
    

    priority_queue<pair<ll,int>, vector<pair<ll,int>>, greater<pair<ll,int>>> pq;

    dist[src] = 0;
    total_time[src] = 0;
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
            int wait_time = (u == src) ? 0 : 1;
            ll cost = e.w + (K * (e.tt + wait_time));



            if (dist[u] + cost < dist[v]) {
                dist[v] = dist[u] + cost;
                total_time[v] = total_time[u] + e.tt + (u == src ? 0 : 1);
                parent[v] = u;
                pq.push({dist[v], v});
            }
        }
    }

    return dist;
}


int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, X;
    cin>>K>>N>>X;

    vector<vector<Edge>> g(N);

    // input edges
    for (int i = 0; i < X; i++) {
        int u, v, tt;
        ll tc;
        cin>>u>>v>>tt>>tc;

        u--; 
        v--; // remove if input is 0-based

        g[u].push_back({v, tt, tc});
        // if undirected graph, also uncomment:
        // g[b].push_back({a, c});
        g[v].push_back({u, tt, tc});
    }

    int S, D;
    cin>>S>>D;
    S--;
    D--;
    vector<ll> parent(N, -1);
    vector<ll> total_time(N, 0);

    vector<ll> min_cost = dijkstra(S, g, parent, total_time);

    if(min_cost[D]==INF){
    cout<<"Error";
    return 0;
}
    vector<int>path;
    int curr = D;

    while(curr != -1){
        path.push_back(curr);
        if(curr == S) break;
        curr = parent[curr];
    }
    reverse(path.begin(), path.end());

    for(int i : path){
        cout<<i + 1;
        if(i == path[path.size() - 1])cout<<" ";
        else cout<<"->";
    }

    cout<<total_time[D]<<" " << min_cost[D]<<endl;

    // // output distances
    // for (int i = 0; i < n; i++) {
    //     if (dist[i] == INF) cout << -1;
    //     else cout << dist[i];

    //     if (i != n - 1) cout << " ";
    // }

    
    cout << "\n";

    return 0;
}
