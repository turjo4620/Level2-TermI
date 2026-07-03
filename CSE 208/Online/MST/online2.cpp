// have to complete


/*
Designing a Robust Network with Multiplicative Costs
Problem Statement
A power grid planner is designing a network to connect N cities using M possible bidirectional transmission lines. Each line has a positive cost.

To ensure redundancy and reliability, the planner wants two different connection plans:

A primary plan that connects all cities with the minimum possible product of edge costs.
A backup plan that:
Also connects all cities (forms a spanning tree),
Has a product of edge costs strictly greater than the primary plan,
Is the smallest such product among all remaining valid spanning trees.
Your task is to compute the product of edge weights for this backup network configuration.

If no such plan exists, print -1.

Input Format
N M
u1 v1 w1
u2 v2 w2
...
uM vM wM
N — number of cities (1 ≤ N ≤ 15)
M — number of available edges (N-1 ≤ M ≤ N×(N-1)/2)
Each of the next M lines contains:
u_i and v_i — endpoints of the edge (1-based indices)
w_i — weight of the edge (1 ≤ w_i ≤ 1000)
Output Format
Print a single integer — the product of edge weights in the second-best network plan. If no such plan exists, print -1.

Sample Input 1
4 5
1 2 2
2 3 2
3 4 2
4 1 2
1 3 3
Sample Output 1
12
Sample Input 2
3 3
1 2 1
2 3 1
1 3 1
Sample Output 2
-1*/


#include <bits/stdc++.h>
using ll = long long;

using namespace std;

class Edge{
public:
    int u, v;
    ll wt;
    ll cost;
    
    Edge(int u , int v, ll wt){
        this->u = u;
        this->v = v;
        this->wt = wt;
        this->cost = log(wt);
    }

    // cost er basis e sorting hobe ->> product to oijonno

    bool operator <(const Edge &other) const{
        return this->cost < other.cost;
    }
};



// second best mst ber korte hoile mst path e max edge ber kore kore remove kora lage and check kroa lage

int dfs(int u, int parent, int target, int maxEdge, vector<vector<pair<int, int>>>&adj){
    if(u == target) return maxEdge;

    for(auto p : adj[u]){
        if(p.first == parent){
            continue;
        }
        int res = dfs(p.first, u, target, max(maxEdge, p.second), adj);
        if(res != -1) return res;
    }
    return -1;
}

class Graph{
public:
    int V;
    vector<Edge>edges;
    vector<int>par, rnk;

    // for second best mst
    vector<Edge>mstedges;
    vector<vector<pair<int, int>>>adj;


    Graph(int V){
        this->V = V;

        for(int i = 0; i < V; i++){
            par.push_back(i);
            rnk.push_back(0);
        }
        adj.resize(V);
    }
    void addEdge(int u, int v, int wt){
        edges.push_back(Edge(u, v, wt));
    }


    // find function

    int find(int x){
        if(par[x] == x){
            return x;
        }
        // path compression
        return par[x]= find(par[x]);
    }

    // union by rank

    void unionByRank(int u, int v){
        int pu = find(u);
        int pv = find(v);

        if(pu == pv) return;

        if(rnk[pu] < rnk[pv]){
            par[pu] = pv;
        }
        else if(rnk[pu] > rnk[pv]){
            par[pv] = pu;
        }
        else{
            par[pv] = pu;
            rnk[pu]++;
        }
    }   

    ll krsukal(){
        sort(edges.begin(), edges.end()); // O(ElogE)
        ll mstCost = 0;

        int count = 0;

        for(int i = 0; i < edges.size(); i++){
            Edge e = edges[i];

            int parU = find(e.u);
            int parV = find(e.v);

            if(parU != parV){ // no cycle
                unionByRank(e.u, e.v);
                
                mstCost += e.cost;

                mstedges.push_back(e);

                adj[e.u].push_back({e.v, e.cost});
                adj[e.v].push_back({e.u, e.cost});

                count++;
                if(count == V - 1) break;
                
            }

        }
        return mstCost;
    }


};


int main(){
    
    int m, n;
    cin>>m>>n;
    Graph graph(m);
    

    // ll total_cost = 0;
    for(int i = 0; i < n; i++){
        int p, q;
        ll r;
        cin>>p>>q>>r;
        // 1 based
        p--; q--;
        // total_cost += r;
        graph.addEdge(p, q, r);
    }

    ll mstCost = graph.krsukal();

    ll seondBest = LLONG_MAX;

    for(auto &e : graph.edges){
        bool inMst = false;

        for(auto &x : graph.mstedges){
            if(e.u == x.u && e.v == x.v && e.wt == x.wt){
                inMst = true;
            }
        }

        if(inMst) continue;

        // dfs(int u, int parent, int target, int maxEdge, vector<vector<pair<int, int>>>&adj)
        int mx = dfs(e.u, -1, e.v, 0, graph.adj);

        if(mx == -1) continue;

        ll newcost = mstCost - mx + e.cost;

        if(newcost > mstCost){
            seondBest = min(newcost, seondBest);
        }
    }

    if(seondBest == LLONG_MAX) cout << -1;
    else cout << (long long)exp(seondBest);

    return 0;
}