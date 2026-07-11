// shattered kingdom
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
 

    void bellmanFord(int src, vector<bool>&blocked,vector<int>&emergency ){
        // super source technique
        vector<ll>dist(V + 1, LLONG_MAX);

        dist[src] = 0;


        for(int i = 0; i < V; i++){
            for(Edge e : edges){
                // block hoile just skip
                if(blocked[e.u] || blocked[e.v]) continue;
                    if(dist[e.u] != LLONG_MAX && dist[e.v] > dist[e.u] + e.wt){
                        dist[e.v] = dist[e.u] + e.wt;
                    }    
            }  
        }

        for(Edge e: edges){
            // block hoile ekhaneo skip
            if(blocked[e.u] || blocked[e.v]) continue;
            if(dist[e.u] != LLONG_MAX && dist[e.v] > dist[e.u] + e.wt){
               cout<<"Abyss Detected"<<endl;
               return;
            }
        }

        for(int i = 0; i < emergency.size(); i++){
            if(dist[emergency[i]] == LLONG_MAX)
                cout << "INF ";
            else
                cout << dist[emergency[i]] << " ";
        }

    }

};



int main(){
    
    int n, m, k, b, e;
    cin>>n>>m>>k>>b>>e;
    Graph graph(n);
    vector<int>capital(k);

    for(int i = 0; i < k; i++){
        cin>>capital[i];
    }

    vector<bool>blocked(n + 1, false);
    
    for(int i = 0; i < b; i++){
        int a;
        cin>>a;
        blocked[a] = true;
    }

    vector<int>emergency(e);

    for(int i = 0; i < e; i++){
        cin>>emergency[i];
    }
    
    
    for(int i = 0; i < m; i++){
        int p, q;
        ll r;
        cin>>p>>q>>r;
        graph.addEdge(p, q, r);
    }

    for(int i = 0; i < k; i++){
        graph.addEdge(0, capital[i], 0);
    }
    graph.bellmanFord(0 ,blocked, emergency);

    return 0;
}
