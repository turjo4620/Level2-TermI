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

class Edge {
public:
    int u, v, id; 
    ll wt;
    Edge(int u, int v, int id, ll wt) : u(u), v(v), id(id), wt(wt) {}
    bool operator<(const Edge &other) const { return this->wt < other.wt; }
};


class Graph {
public:
    int V;
    vector<Edge> edges;
    vector<int> par, rank;

    Graph(int V) : V(V) {
        for (int i = 0; i <= V; i++) { // V+1 পর্যন্ত যেন 1-based index সাপোর্ট করে
            par.push_back(i);
            rank.push_back(0);
        }
    }
    void addEdge(int u, int v, int id, ll wt) {
        edges.push_back(Edge(u, v, id, wt));
    }

    int find(int x) {
        return (par[x] == x) ? x : (par[x] = find(par[x]));
    }

    bool unionByRank(int u, int v) {
        int parA = find(u);
        int parB = find(v);
        if (parA == parB) return false;
        if (rank[parA] < rank[parB]) par[parA] = parB;
        else if (rank[parA] > rank[parB]) par[parB] = parA;
        else { par[parB] = parA; rank[parA]++; }
        return true;
    }



    pair<ll, vector<int>> getMst(int forbidden){
        sort(edges.begin(), edges.end());

        for(int i = 0; i <= V; i++){
            par[i] = i;
            rank[i] = 0;
        }

        ll prod = 1;
        int count = 0;
        vector<int>USED;

        for(auto & e : edges){
            if(e.id == forbidden) continue;
            if(unionByRank(e.u, e.v)){
                prod *= e.wt;
                USED.push_back(e.id);
                count++;
            }
        }
        if(count == V - 1) return{prod, USED};
        else return {-1, {}};
    }

};

int main(){
    ios_base::sync_with_stdio(false); cin.tie(NULL);
    int N, M;
    cin>>N>>M;
    Graph g(N);

    for(int i = 0; i < M; i++){
        int u, v; 
        ll wt;
        cin>>u>>v>>wt;
        g.addEdge(u, v, i, wt);
    }
    

    auto primary = g.getMst(-1);
    if(primary.first == -1) {cout<< -1 <<endl; return 0;}

    set<ll>backupplans;
 
    for(int i = 0; i < M; i++){
        auto backup = g.getMst(i);
        if(backup.first != -1 && backup.first > primary.first){
            backupplans.insert(backup.first);
        }
    }
    if(backupplans.empty()) cout << -1 <<endl;
    else cout<< *backupplans.begin()<<endl;

}