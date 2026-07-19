
/*
July 2025 CSE 208
Online on APSP
Time: 30 minutes

A logistics company manages a delivery network represented as a weighted directed graph.
Each node is a city, and each edge is a direct road with a specific cost (fuel/tolls). The
government recently set up a quality control center located at city V. To promote quality control,
the government is giving a discount campaign. Those vehicles that pass through city V will get a
discount of 1 unit to each road cost (edge weights will reduce by 1 if the path passes through
vertex V).
Calculate the shortest possible cost to travel between all pairs of cities (i,j).

Input
The first input line has two integers n and m: the number of cities and roads (1 <= n <= 500, 1
<= m <= n
2
).

Then, there are m lines describing the roads. Each line has three integers a, b and c: there is a
road between cities a and b whose length is c (0 <= a,b < n, 1 <= c <= 10
9
).

Next line will take the city V
Next line contains the number of queries q
Finally, there are q lines describing the queries. Each line has two integers a and b: determine
the length of the shortest route between cities a and b.
Output
Print the length of the shortest route for each query. If there is no route, print -1 instead.
Example


*/

#include <bits/stdc++.h>

using namespace std;
using ll = long long;
const ll INF = 1e18;

class Edge{
public:
    int u, v;
    ll wt;

    Edge(int u, int v, ll wt){
        this->u = u;
        this->v = v;
        this->wt = wt;
    }
    bool operator <(const Edge &other) const{
        return this->wt < other.wt;
    }
};

class Graph{
public:
    int V;
    vector<Edge>edges;

    Graph(int V){
        this->V = V;
    }

    void addEdge(int u, int v, ll wt){
        edges.push_back(Edge(u, v,wt));
    }

    void floydWarshall(int discount, vector<vector<int>>&query){
        vector<vector<ll>>dist1(V, vector<ll>(V, INF));
        vector<vector<ll>>dist2(V, vector<ll>(V, INF));
        
        for(int i = 0; i < V; i++){
            dist1[i][i] = 0;
            dist2[i][i] = 0;
        }

        for(Edge e : edges){
            dist1[e.u][e.v] = min(dist1[e.u][e.v] , e.wt);
            dist2[e.u][e.v] = min(dist2[e.u][e.v] , e.wt - 1);
        }

        for(int k = 0; k < V; k++){
            for(int i = 0; i < V; i++){
                for(int j = 0; j < V; j++){
                    if (dist1[i][k] != INF && dist1[k][j] != INF) {
                        dist1[i][j] = min(dist1[i][j], dist1[i][k] + dist1[k][j]);
                    }
                    if (dist2[i][k] != INF && dist2[k][j] != INF) {
                        dist2[i][j] = min(dist2[i][j], dist2[i][k] + dist2[k][j]);
                    }                        
                    
                }
            }
        }

        for(int i = 0; i < query.size(); i++){
            int start = query[i][0];
            int end = query[i][1];

            ll ans = dist1[start][end];

                if(dist2[start][discount] != INF && dist2[discount][end] != INF){
                    ans = min(ans, dist2[start][discount] + dist2[discount][end]);
                }
                if(ans >= INF){
                    cout << -1 << endl;
                }
                else{
                    cout << ans << endl;
                }
            
        }

    }
};




int main() {
    int n, m;
    cin >> n >> m;
    
    Graph graph(n);
    
    for (int i = 0; i < m; i++) {
        int p, q;
        ll r;
        cin >> p >> q >> r;
        graph.addEdge(p, q, r);
    }

    int discount;
    cin>>discount;
    

    int Q;
    cin>>Q;

    vector<vector<int>>query(Q, vector<int>(2));
    for(int i = 0; i < Q; i++){
        cin>>query[i][0];
        cin>>query[i][1];
    }

    graph.floydWarshall(discount, query);


    return 0;
}