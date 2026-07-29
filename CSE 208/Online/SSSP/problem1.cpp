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
 

    void bellmanFord(int src, int K, int start,int target){
        vector<ll>prev_dist(V, LLONG_MAX);

        src = start;
        prev_dist[src] = 0;

        for(int i = 0; i < K; i++){
            vector<ll>curr_dist = prev_dist;
            for(Edge e : edges){
                if(prev_dist[e.u] != LLONG_MAX && curr_dist[e.v] > prev_dist[e.u] + e.wt){
                    curr_dist[e.v] = prev_dist[e.u] + e.wt;
                }
            }   
            prev_dist = curr_dist;
        }

        // for(Edge e: edges){
        //     if(dist[e.u] != LLONG_MAX && dist[e.v] > dist[e.u] + e.wt){
        //         return;
        //     }
        // }

            if(prev_dist[target] == LLONG_MAX)
                cout << "INF ";
            else
                cout << prev_dist[target] << " ";
        

    }

};



int main(){
    
    int n, m;
    cin>>n>>m;
    Graph graph(n);

    int K;
    cin>>K;

    int src, target;
    cin>>src>>target;
    src--; target--;
    
    
    for(int i = 0; i < m; i++){
        int p, q;
        ll r;
        cin>>p>>q>>r;
        p--; q--;
        graph.addEdge(p, q, r);
    }

    graph.bellmanFord(0, K, src, target);

    return 0;
}
