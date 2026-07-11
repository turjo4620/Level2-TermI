#include <bits/stdc++.h>
using ll = long long;

using namespace std;

class Edge{
public:
    int u, v;
    // ll wt;
    
    Edge(int u , int v){
        this->u = u;
        this->v = v;
        // this->wt = wt;
    }

    // bool operator <(const Edge &other) const{
    //     return this->wt < other.wt;
    // }
};




class Graph{
public:
    int V;
    vector<Edge>edges;


    Graph(int V){
        this->V = V;
    }
    void addEdge(int u, int v){
        edges.push_back(Edge(u, v));
    }
 

    void bellmanFord(int src, vector<ll>&tax){
        vector<ll>dist(V, LLONG_MAX);

        dist[src] = 0;
        tax[0] = 0;

        for(int i = 0; i < V - 1; i++){
            for(Edge &e : edges){
                if(dist[e.u] != LLONG_MAX && dist[e.v] > dist[e.u] + tax[e.v]){
                    dist[e.v] = dist[e.u] + tax[e.v];
                }
            }   
        }

        for(Edge &e: edges){
            if(dist[e.u] != LLONG_MAX && dist[e.v] > dist[e.u] + tax[e.v]){
                cout<<"Negative cycle detected!"<<endl;
                return;
            }
        }

        // for(int i = 0; i < V; i++){
        //     if(dist[i] == LLONG_MAX)
        //         cout << "INF ";
        //     else
        //         cout << dist[i] << " ";
        // }

        cout<<dist[V - 1]<<endl;

    }

};



int main(){
    
    int m, n;
    cin>>m>>n;
    Graph graph(m);
    
    vector<ll>tax(m);

    for(int i = 0; i < m; i++){
        cin>>tax[i];
    }
    
    for(int i = 0; i < n; i++){
        int p, q;
        // ll r;
        cin>>p>>q;
        p--; q--;
        graph.addEdge(p, q);
    }

    graph.bellmanFord(0, tax);

    return 0;
}
