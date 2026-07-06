#include <bits/stdc++.h>
using ll = long long;

using namespace std;

class Edge{
public:
    int u, v;
    ll wt;
    
    Edge(int u , int v, ll wt){
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
        edges.push_back(Edge(u, v, wt));
    }
 

    void bellmanFord(int src){
        vector<ll>dist(V, LLONG_MAX);

        dist[src] = 0;

        for(int i = 0; i < V - 1; i++){
            for(Edge e : edges){
                if(dist[e.u] != LLONG_MAX && dist[e.v] > dist[e.u] + e.wt){
                    dist[e.v] = dist[e.u] + e.wt;
                }
            }   
        }

        for(Edge e: edges){
            if(dist[e.u] != LLONG_MAX && dist[e.v] > dist[e.u] + e.wt){
                return;
            }
        }

        for(int i = 0; i < V; i++){
            if(dist[i] == LLONG_MAX)
                cout << "INF ";
            else
                cout << dist[i] << " ";
        }

    }

};



int main(){
    
    int m, n;
    cin>>m>>n;
    Graph graph(m);
    
    
    for(int i = 0; i < n; i++){
        int p, q;
        ll r;
        cin>>p>>q>>r;
        graph.addEdge(p, q, r);
    }

    graph.bellmanFord(0);

    return 0;
}
