/*
Online 1 (A1/A2): Tired of flights

Time: 30 mins

Problem description
Let me remind you the story of offline 1.

You are analyzing a global flight network centered around Dhaka (DAC).
A traveler wants to fly from Syrjälä International Terminal Hazrat Shahjalal International Airport to London
Heathrow Airport. Each flight between countries has a fixed ticket price. The traveler has one international
discount coupon that can be used to halve the cost of any one flight in the journey. (If the cost is x, it
becomes ⌊x/2⌋)
You are given a list of international flights with their prices. Find the minimum possible travel cost from
Dhaka to London.

Now, you are tired of flights and want to find the minimum possible travel cost from Dhaka to
London within k flights. Of course, you have the coupon.

Required complexity
Time complexity: O(VE)

Input
The first input line contains three integers n, m, and k:
● n is the number of airports
● m is the number of international flight connections
● k is the maximum number of flights the traveler can take

The airports are numbered 1, 2, ..., n.
Airport 1 is Hazrat Shahjalal International Airport (Dhaka), and airport n is London
Heathrow Airport.
After this, there are m lines describing the flights.
Each line contains three integers a, b, and c, meaning that there is a unidirectional flight from
airport a to airport b with ticket price c.

Output
Print a single integer:
the minimum possible travel cost from Dhaka to London using at most k flights, after
optimally applying the discount coupon. If it is not possible to reach London within k flights, print
“Not possible”.

Example

Input Output
4 5 1
1 2 4
2 4 4
1 3 10
3 4 1
2 3 2

Not possible

4 5 2
1 2 4
2 4 4
1 3 10
3 4 1
2 3 2

6

4 5 3
1 2 4
2 4 4
1 3 10
3 4 1
2 3 2

5
*/


#include <bits/stdc++.h>
using ll = long long;
const ll INF = 1e18;
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
 

    ll bellmanFord(int src, int K, int target){
        // vector<ll>dist(V, LLONG_MAX);

        vector<vector<ll>>prev_dist(V + 1, vector<ll>(2, INF));

        prev_dist[src][0] = 0;

        // dist[src] = 0;

        // for(int i = 0; i < K; i++){

        //     vector<vector<long long>> curr_dist = prev_dist;

        //     for(Edge e : edges){
        //         int u = e.u;
        //         int v = e.v;
        //         ll w = e.wt;

        //         if(prev_dist[u][0] != INF){
        //             curr_dist[v][0] = min(prev_dist[u][0] + w, curr_dist[v][0]);
        //         }

        //         if(prev_dist[u][1] != INF){
        //             curr_dist[v][1] = min(prev_dist[u][1] + w, curr_dist[v][1]);
        //         }
        //         if(prev_dist[u][0] != INF){
        //             curr_dist[v][1] = min(prev_dist[u][0] + (w / 2), curr_dist[v][1]);
        //         }

        //     }   
        //     prev_dist = curr_dist;
        // }

        for(int i = 0; i < K; i++){
            vector<vector<ll>>curr = prev_dist;
            for(Edge &e : edges){
                int u = e.u;
                int v = e.v;
                ll wt = e.wt;

                if(curr[v][0] != INF){
                    curr[v][0] = min(prev_dist[u][0] + wt, curr[v][0]);
                }

                if(curr[v][0] != INF){
                    curr[v][1] = min(prev_dist[u][0] + (wt / 2), curr[v][1]);
                }

                if(curr[v][1] != INF){
                    curr[v][1] = min(prev_dist[u][1] + wt, curr[v][1]);
                }
            }
            prev_dist = curr;
        }

        // for(Edge e: edges){
        //     if(dist[e.u] != LLONG_MAX && dist[e.v] > dist[e.u] + e.wt){
        //         return;
        //     }
        // }

        // for(int i = 0; i < V; i++){
        //     if(dist[i] == LLONG_MAX)
        //         cout << "INF ";
        //     else
        //         cout << dist[i] << " ";
        // }

        ll ans = min(prev_dist[target][0], prev_dist[target][1]);
        if(ans == INF) return -1;
        else return ans;
    }

};



int main(){
    
    int m, n, k;
    cin>>m>>n>>k;
    Graph graph(m);
    
    
    for(int i = 0; i < n; i++){
        int p, q;
        ll r;
        cin>>p>>q>>r;
        graph.addEdge(p, q, r);
    }

    ll ans = graph.bellmanFord(1, k, m);
    if(ans == -1) cout<<"Impossible"<<endl;
    else cout<< ans <<endl;

    return 0;
}


