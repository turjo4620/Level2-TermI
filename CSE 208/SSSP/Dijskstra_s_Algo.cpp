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

    pq.push({0, src});

    while(pq.size() > 0){
        int u = pq.top().second;
        pq.pop();

        for(Edge e : g[u]){
            // edge relaxation step
            if (dist[e.v] > dist[u] + e.wt){
                dist[e.v] = dist[u] + e.wt;
                pq.push({dist[e.v], e.v});
            }    
        }
    }

    for(int i = 0; i < V; i++){
        cout<<dist[i]<<" ";
    }
    cout<<endl;
}

int main(){
    int V = 6;
    vector<vector<Edge>>g(V);

    g[0].push_back(Edge(1, 2));
    g[0].push_back(Edge(2, 4));

    g[1].push_back(Edge(2, 1));
    g[1].push_back(Edge(3, 7));

    g[2].push_back(Edge(4, 3));

    g[3].push_back(Edge(5, 1));

    g[4].push_back(Edge(3, 2));
    g[4].push_back(Edge(5, 5));
    
    dijsktra(0, g, V);
    return 0;
}