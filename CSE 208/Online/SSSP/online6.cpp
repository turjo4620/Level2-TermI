/*
🧪 Problem: Too Many DSA Labs!
🏫 Scenario:
In the future, the ECE department has N DSA labs, connected by M bidrectional roads. Each road has a walking cost (effort to move between labs). Every lab charges an entry fee F to maintain all that fancy lab equipment 😅.

All students enter the lab network at Lab 1.

Each lab can hold only a certain number of students (it has a capacity).

Now, K students are entering one by one (student 1 enters first, then student 2, and so on…).

🎯 Goal for Each Student:
Find the best lab to go to so that the total cost is minimized.

💰 Total Cost =
Walking cost from Lab 1 to the chosen lab + Entry fee F

But: - A student can’t go to a lab if it’s already full (capacity reached). - If no lab is available, the student gets a cost of -1.

📥 Input Format:
N M F
c1 c2 c3 ... cN
u1 v1 w1
u2 v2 w2
...
uM vM wM
K
Where:
N = number of labs
M = number of roads
F = fixed entry fee for any lab
ci = capacity of lab i (space-separated, N values)
Each road is between labs ui and vi with walking cost wi
K = number of students
📤 Output Format:
Print K space-separated integers — the minimum total cost for each student.

If a student can’t find a lab with remaining space, print -1 for that student.

📦 Sample Input:
5 4 20
1 2 1 1 2
1 2 2
4 5 1
3 4 1
1 3 1
5
✅ Sample Output:
20 21 22 22 22
🧠 Notes:
All students start at Lab 1.
The graph is connected, so every lab is reachable.
Solve using Dijkstra’s/BF Algorithm from Lab 1 to get shortest distances.
Always assign a student to the cheapest available lab.

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
vector<ll> dijkstra(int src, const vector<vector<Edge>>& g, ll F) {
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
    ll F;
    cin >> n >> m >> F;

    vector<vector<Edge>> g(n);

    vector<int>capacity(n);
    for(int i = 0; i < n; i++){
        cin>>capacity[i];
    }

    // input edges
    for (int i = 0; i < m; i++) {
        int a, b;
        ll c;
        cin >> a >> b >> c;

        a--; 
        b--; // remove if input is 0-based

        g[a].push_back({b, c});
        // if undirected graph, also uncomment:
        g[b].push_back({a, c});
    }

    int num_stu;
    cin>>num_stu;

    int src = 0;
    vector<ll> dist = dijkstra(src, g, F);


    vector<pair<ll, int>>available_labs;
    for(int i = 0; i < n; i++){
        if(dist[i] != INF){
            available_labs.push_back({dist[i] + F, i});
        }
    }

    sort(available_labs.begin(), available_labs.end());

    for(int i  = 0; i < num_stu; i++){
        bool found = false;

        for(auto & lab : available_labs){
            ll cost = lab.first;
            int lab_idx = lab.second;

            if(capacity[lab_idx] > 0){
                cout<<cost<<" ";
                capacity[lab_idx] --;
                found = true;
                break;
            }

        }
        if (!found) {
            cout << -1 << " ";
        }
    }

    // output distances
    // for (int i = 0; i < n; i++) {
    //     if (dist[i] == INF) cout << -1;
    //     else cout << dist[i];

    //     if (i != n - 1) cout << " ";
    // }
    // cout << "\n";

    return 0;
}