// The present -> A section


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

class Graph{
public:
    int V;
    vector<Edge>edges;
    vector<int>par, rnk;

    Graph(int V){
        this->V = V;

        for(int i = 0; i < V; i++){
            par.push_back(i);
            rnk.push_back(0);
        }
    }
    void addEdge(int u, int v, ll wt){
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

    bool unionByRank(int u, int v) {
    int parA = find(u);
    int parB = find(v);

    if (parA == parB)
        return false;

    if (rnk[parA] == rnk[parB]) {
        par[parB] = parA;
        rnk[parA]++;
        }
    else if (rnk[parA] > rnk[parB]) {
        par[parB] = parA;
        }
    else {
        par[parA] = parB;
        }

        return true;
    }
    void krsukal(){
        sort(edges.begin(), edges.end()); // O(ElogE)
        ll mstCost = 0;

        int count = 0;

        for (const Edge &e : edges) {
            if (unionByRank(e.u, e.v)) {
                mstCost += e.wt;
                count++;

            if (count == V - 1)
                break;
            }
        }
        cout<<"MST cost: "<<mstCost<<endl;
    }


};


int main(){
    
    int n; 
    cin>>n;
    
    Graph graph(n + 1);

    int m;
    cin>>m;

    for(int i = 0; i < n; i++){
        int a;
        cin>>a;
        graph.addEdge(0, i + 1, a);
    }

    for(int i = 0; i < m; i++){
        int p,q,r;
        cin>>p>>q>>r;
        graph.addEdge(p, q, r);
    }

    graph.krsukal();

    return 0;
}