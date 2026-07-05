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
using namespace std;
using ll = long long;

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


bool clean_dfs(int curr, int parent, int target, ll new_wt, ll &max_edge_in_path, vector<vector<pair<int, ll>>>& adj) {
    if (curr == target) return true;

    for (auto p : adj[curr]) {
        int next_node = p.first;
        ll edge_weight = p.second;

        if (next_node == parent) continue; 

        if (clean_dfs(next_node, curr, target, new_wt, max_edge_in_path, adj)) {
            if (edge_weight < new_wt) {
                max_edge_in_path = max(max_edge_in_path, edge_weight);
            }
            return true; 
        }
    }
    return false; 
}

class Graph{
public:
    int V;
    vector<Edge>edges;
    vector<int>par, rnk;

    vector<Edge>mstedges;
    vector<vector<pair<int, ll>>>adj;

    Graph(int V){
        this->V = V;
        for(int i = 0; i < V; i++){
            par.push_back(i);
            rnk.push_back(0);
        }
        adj.resize(V);
    }
    
    void addEdge(int u, int v, ll wt){
        edges.push_back(Edge(u, v, wt));
    }

    int find(int x){
        if(par[x] == x){
            return x;
        }
        return par[x]= find(par[x]);
    }

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

    ll kruskal(){
        sort(edges.begin(), edges.end()); 
        
        ll mstCost = 1;
        int count = 0;

        for(int i = 0; i < edges.size(); i++){
            Edge e = edges[i];

            int parU = find(e.u);
            int parV = find(e.v);

            if(parU != parV){ 
                unionByRank(e.u, e.v);
                
                mstCost *= e.wt; 
                count++;

                mstedges.push_back(e);
                adj[e.u].push_back({e.v, e.wt});
                adj[e.v].push_back({e.u, e.wt});
            }
        }
        

        if(count != V - 1) return -1;
        
        return mstCost;
    }
};

int main(){
    int n, m;

    if (!(cin >> n >> m)) return 0;
    
    Graph graph(n);
    
    for(int i = 0; i < m; i++){
        int p, q;
        ll r;
        cin >> p >> q >> r;
        // 0-based indexing
        p--; q--; 
        graph.addEdge(p, q, r);
    }

    ll mstCost = graph.kruskal();
    
  
    if (mstCost == -1) {
        cout << -1 << "\n";
        return 0;
    }

    ll secondBest = LLONG_MAX;

    for(auto &e : graph.edges){
        bool InMST = false;

        for(auto & x : graph.mstedges){
            if(x.u == e.u && x.v == e.v && x.wt == e.wt){
                InMST = true;
                break;
            }
        }

        if(InMST) continue;

        ll mx = -1; 
        clean_dfs(e.u, -1, e.v, e.wt, mx, graph.adj);

        if(mx == -1) continue; 


        ll newCost = (mstCost / mx) * e.wt;

  
        if (newCost > mstCost) {
            secondBest = min(secondBest, newCost);
        }
    }

    if (secondBest == LLONG_MAX) cout << -1 << "\n";
    else cout << secondBest << "\n";

    return 0;
}