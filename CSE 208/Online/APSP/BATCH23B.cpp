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

    void floydWarshall(vector<vector<int>>&query, int m, int n){
        vector<vector<ll>>dist(V, vector<ll>(V, INF));
        
        for(int i = 0; i < V; i++){
            dist[i][i] = 0;
        }

        for(Edge e : edges){
            dist[e.u][e.v] = min(dist[e.u][e.v] , e.wt);
        }

        for(int k = 0; k < V; k++){
            for(int i = 0; i < V; i++){
                for(int j = 0; j < V; j++){
                    if(dist[i][k] != INF && dist[k][j] != INF){
                        dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                    }
                }
            }
        }

        // for(int i = 0; i < V; i++){
        //     for(int j =0; j < V; j++){
        //         if(dist[i][j] == INF){
        //             cout<<"INF ";
        //         }
        //         else{
        //             cout<<dist[i][j]<<" ";
        //         }
        //     }
        //     cout<<endl;
        // }

        for(int i = 0 ; i < query.size(); i++){
            int u = query[i][0];
            int v = query[i][1];

            ll ans = min(dist[u][m] + dist[m][v], dist[u][n] + dist[n][v]);
            if(ans >= INF)
            cout<<-1<<endl;
            
            else
            cout<<ans<<endl;

            
        }
    }
};




int main() {
    int m, n;
    cin >> m >> n;
    
    Graph graph(m);
    
    for (int i = 0; i < n; i++) {
        int p, q;
        ll r;
        cin >> p >> q >> r;
        graph.addEdge(p, q, r);
    }
    int w, x;
    cin>>w>>x;

    int a;
    cin>>a;
    vector<vector<int>>query(a, vector<int>(2));
    for(int i = 0; i < a; i++){
        for(int j = 0; j < 2; j++){
            cin>>query[i][j];
        }
    }

    graph.floydWarshall(query, w, x);

    return 0;
}