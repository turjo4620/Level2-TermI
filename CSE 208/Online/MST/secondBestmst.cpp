#include <bits/stdc++.h>

using namespace std;
using ll = long long;

class Edge{
public:
    int u, v, id; 
    ll wt;
    
    Edge(int id, int u , int v, ll wt){
        this->u = u;
        this->v = v;
        this->wt = wt;
        this->id = id;
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
    void addEdge(int id,int u, int v, ll wt){
        edges.push_back(Edge(id, u, v, wt));
    }


    void resetDSU() {
        for(int i = 0; i < V; i++){
            par[i] = i;
            rnk[i] = 0;
        }
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
    ll krsukal(int ignored_edge_id, vector<int>& mst_edges){
        resetDSU();
        ll mstCost = 0;

        int count = 0;

        for (const Edge &e : edges) {
            if(e.id == ignored_edge_id) continue;

            if (unionByRank(e.u, e.v)) {
                mstCost += e.wt;
                count++;

            if(ignored_edge_id == -1){
                mst_edges.push_back(e.id);
            }

            if (count == V - 1)
                break;
            }
        }
        if(count != V - 1)return LLONG_MAX;

        return mstCost;
    }


};


int main(){
    
    int n;
    cin>>n;
    Graph graph(n);

    int m;
    cin>>m;
    for(int i = 0; i < m; i++){
        int p, q;
        ll r;
        cin>>p>>q>>r;
        graph.addEdge(i, p, q, r);
    }
    sort(graph.edges.begin(), graph.edges.end());
    vector<int>mst_edges;

    ll best_mst = graph.krsukal(-1, mst_edges);
    
    if(best_mst == LLONG_MAX) {cout<<-1<<endl; return 0;}


    ll second_best_mst = LLONG_MAX;
    vector<int> dummy;

    for(int ignored_id : mst_edges){
        ll current_cost = graph.krsukal(ignored_id, dummy);
        second_best_mst = min(second_best_mst, current_cost);
    }

    cout<<second_best_mst<<endl;
    return 0;
}