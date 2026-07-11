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

    int n, m, k;
    cin >> n >> m>>k;

    vector<int>emergency(k);
    
    for(int i = 0; i < k; i++){
        cin>>emergency[i];
    }

    vector<vector<Edge>> g(n + 1);

    // input edges
    for (int i = 0; i < m; i++) {
        int a, b;
        ll c;
        cin >> a >> b >> c;

        g[b].push_back({a, c});
        // if undirected graph, also uncomment:
        // g[b].push_back({a, c});
    }


    for(int i = 0; i < emergency.size(); i++){
        g[0].push_back({emergency[i], 0});
    }

    vector<ll> dist = dijkstra(0, g);

    // output distances
    for (int i = 0; i < n + 1; i++) {
        if (dist[i] == INF) cout << -1;
        else cout << dist[i] << endl;

        // if (i != n - 1) cout << " ";
    }
    cout << "\n";

    return 0;
}





// #include <bits/stdc++.h>
// using ll = long long;

// using namespace std;

// class Edge{
// public:
//     int u, v;
//     ll wt;
    
//     Edge(int u , int v, ll wt){
//         this->u = u;
//         this->v = v;
//         this->wt = wt;
//     }

//     bool operator <(const Edge &other) const{
//         return this->wt < other.wt;
//     }
// };




// class Graph{
// public:
//     int V;
//     vector<Edge>edges;


//     Graph(int V){
//         this->V = V;
//     }
//     void addEdge(int u, int v, ll wt){
//         edges.push_back(Edge(u, v, wt));
//     }
 

//     void bellmanFord(int src){
//         vector<ll>dist(V + 1, LLONG_MAX);

//         dist[src] = 0;

//         for(int i = 0; i < V; i++){
//             for(Edge e : edges){
//                 if(dist[e.u] != LLONG_MAX && dist[e.v] > dist[e.u] + e.wt){
//                     dist[e.v] = dist[e.u] + e.wt;
//                 }
//             }   
//         }

//         for(Edge e: edges){
//             if(dist[e.u] != LLONG_MAX && dist[e.v] > dist[e.u] + e.wt){
//                 return;
//             }
//         }

//         for(int i = 0; i <= V; i++){
//             if(dist[i] == LLONG_MAX)
//                 cout << "INF ";
//             else
//                 cout << dist[i] << " ";
//         }

//     }

// };



// int main(){
    
//     int m, n, k;
//     cin>>m>>n>>k;
//     Graph graph(m + 1);

//     vector<int>emergency(k);

//     for(int i = 0; i < k; i++){
//         cin>>emergency[i];
//     }
    
    
//     for(int i = 0; i < n; i++){
//         int p, q;
//         ll r;
//         cin>>p>>q>>r;
//         graph.addEdge(p, q, r);
//     }

//     for(int i = 0; i < emergency.size(); i++){
//         graph.addEdge(0, emergency[i], 0);
//     }
    


//     graph.bellmanFord(0);

//     return 0;
// }
