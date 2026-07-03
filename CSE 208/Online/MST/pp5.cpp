/* 
Second best MST

Second best MST =
    A spanning tree whose total weight is:
            strictly greater than MST
            but minimum among all such trees

*/

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




// finding the max edge
int dfs(int u, int parent, int target, int newwt, vector<vector<pair<int, int>>>&adj){

    if(u == target) return newwt;

    for(auto p : adj[u]){
        if(p.first == parent){
            continue;
        }

        int res = dfs(p.first, u, target, max(newwt, p.second), adj);
        if(res != -1) {
            if(p.second < newwt){
                return max(res, p.second);
            }
            return res;
        }
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
                mstCost += e.wt;

                mstedges.push_back(e);

                adj[e.u].push_back({e.v, e.wt});
                adj[e.v].push_back({e.u, e.wt});
                
            }

        }
        return mstCost;
    }


};


int main(){
    
    int m, n;
    cin>>m>>n;
    Graph graph(m);
    

    ll total_cost = 0;
    for(int i = 0; i < n; i++){
        int p, q;
        ll r;
        cin>>p>>q>>r;
        // 1 based
        p--; q--;
        total_cost += r;
        graph.addEdge(p, q, r);
    }

    ll mstCost = graph.krsukal();

    ll secondBest = LLONG_MAX;

    for(auto &e : graph.edges){

        bool InMST = false;

        for(auto & x : graph.mstedges){
            if(x.u == e.u && x. v == e.v && x.wt == e.wt){
                InMST = true;
            }
        }

        if(InMST) continue;

        int mx = dfs(e.u, -1, e.v, e.wt, graph.adj);

        if(mx == -1) continue;

        ll newCost = mstCost + e.wt - mx;

        if (newCost > mstCost)
        secondBest = min(secondBest, newCost);

    }


    if (secondBest == LLONG_MAX) cout << -1 << "\n";
    else cout << secondBest << "\n";

    return 0;
}
