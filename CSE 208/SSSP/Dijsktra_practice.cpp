#include <bits/stdc++.h>
using namespace std;


/*
Shortest path from src to all vertices for weighted graph

Dijsktra algo -> greedy algorithm, for positive weighted
locally choosing the optimal answer to reach the final optimum answer

calculating the shortest path distance

*/


// Graph representation

class Edge{
public:
    int v;
    int wt;

    Edge(int v, int wt){
        this->v = v;
        this->wt = wt;
    }
};

/*
Edge relaxation:
   - comparing direct and indirect paths  
   src -> u -> v
   if dist[v] > dist[u] + wt(u, v) 
        dist[v] = dist[u] + wt(u, v)

    basically, here the shortest path is being choosen!
*/

/*
Dijksta's algorithm:
    -> performing BFS but using priority queue
    
    what to store in priority queue?
        - dist[u], u (node and shortest distance from src to node)
        - we are storing dist[u] first because in priority queue, the sorting will occur according to the first element of the pair

        Steps:
        distance vector -> dist (initialized with infinity)
        priority queue -> PQ
        
        dist[src] = 0

        while PQ is not empty
            u = pq.top().second
            for all neighbours of u (g[u])
            for (Edge e : neighs)
                --Edge relaxation
                if dist[e.v] > dist[u] + e.wt(u, v) 
                    dist[e.v] = dist[u] + e.wt(u, v)
                push dist[e.v] and v in PQ.

    */

void dijsktra(int src, vector<vector<Edge>>g, int V){
    vector<int>dist(V, INT_MAX);
    dist[src] = 0;

    /*
    // normal priority queue -> max heap
    priority_queue<int>pq;
    // min heap
    priority_queue<int, vector<int>, greater<int>> pq;
    */
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>>pq;
    // <dist[u], u>

    vector<int>parent(V, -1);
    pq.push({0, src});


    while(pq.size() > 0){
        int u = pq.top().second;
        pq.pop();

        for(Edge e : g[u]){
            // edge relaxation step
            if (dist[e.v] > dist[u] + e.wt){
                dist[e.v] = dist[u] + e.wt;
                parent[e.v] = u;
                pq.push({dist[e.v], e.v});
            }    
        }
    }

    for(int i = 0; i < V; i++){
        cout<<dist[i]<<" ";
    }
    cout<<endl;
    for(int i : parent){
        cout<<i<<" ";
    }
    cout<<endl;

}

int main(){
    int n, m;
    cin>>n>>m;
    vector<vector<Edge>>g(n);

    for(int i = 0; i < m; i++){
        int a, b, c;
        cin>>a>>b>>c;
        g[a - 1].push_back(Edge(b - 1, c));
    }
    dijsktra(0, g, n);
    return 0;
}



