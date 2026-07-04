// mst 22 -- Building Resilient Networks Amid Risky Connections
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
    vector<Edge>safe_edges, risky_edges;
    vector<int>par, rnk, safe_count;
    int total_safe;
    vector<bool> is_risky;

    Graph(int V, const vector<bool>& is_risky_node){
        this->V = V;
        this->is_risky = is_risky_node;
        total_safe = 0;

        for(int i = 0; i < V; i++){
            par.push_back(i);
            rnk.push_back(0);

            if(!is_risky[i]){
                safe_count.push_back(1);
                total_safe++;
            }
            else{
                safe_count.push_back(0);
            }
        }
    }
    void addSafeEdge(int u, int v, ll wt){
        safe_edges.push_back(Edge(u, v, wt));
    }

    void addRiskyedges(int u, int v, ll wt){
        risky_edges.push_back(Edge(u, v, wt));
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
        safe_count[parA]++;
        }
    else if (rnk[parA] > rnk[parB]) {
        par[parB] = parA;
        safe_count[parA]++;
        }
    else {
        par[parA] = parB;
        safe_count[parB] += safe_count[parA];

        }

        return true;
    }
    void krsukal(){
        sort(safe_edges.begin(), safe_edges.end()); // O(ElogE)
        sort(risky_edges.begin(), risky_edges.end());

        vector<Edge>initial_mst;


        for (const Edge &e : safe_edges) {
            if (unionByRank(e.u, e.v)) {
                initial_mst.push_back(e);
        }
        }
        int first_safe = -1;
        for(int i = 0; i < V; i++){
            if(!is_risky[i]) {
                first_safe = i;
                break;
            }
        }

        if(total_safe > 0){
            for(const Edge &e : risky_edges){
                if(safe_count[find(first_safe)] == total_safe) break;

                if(unionByRank(e.u, e.v)){
                    initial_mst.push_back(e);
                }
            }

            if (safe_count[find(first_safe)] != total_safe){
                cout<<-1<<endl;
                return;
            }
        }

    

    vector<int>deg(V, 0);
    vector<vector<pair<int, int>>> adj(V);

    for(int i = 0; i < initial_mst.size(); i++){
        int u = initial_mst[i].u;
        int v = initial_mst[i].v;

        adj[u].push_back({v, i});
        adj[v].push_back({u, i});

        deg[u]++;
        deg[v]++;
    }

    queue<int> q;
    for(int i = 0; i < V; i++){
        if(deg[i] == 1 && is_risky[i]){
            q.push(i);
        }
    }

    vector<bool>keep_edge(initial_mst.size(), true);

    while(!q.empty()){
        int u = q.front();
        q.pop();

        for(auto &edge_info : adj[u]){
            int v = edge_info.first;
            int edge_idx = edge_info.second;

            if(keep_edge[edge_idx]){
                keep_edge[edge_idx] = false;
                deg[v]--;

                if(deg[v] == 1 && is_risky[v]){
                    q.push(v);
                }
            }
        }
    }

    vector<Edge>final_edges;
    ll mstCost = 0;

    for(int i = 0; i < initial_mst.size(); i++){
        if(keep_edge[i]){
            final_edges.push_back(initial_mst[i]);
            mstCost += initial_mst[i].wt;
        }
    }

    cout << final_edges.size() << "\n";
        for (const Edge &e : final_edges) {
            cout << e.u << " " << e.v << "\n";
        }
        cout << mstCost << "\n";  


}
};


int main(){
    
    int n;
    cin>>n;

    int m;
    cin>>m;

    int penalty;
    cin>>penalty;

    int pn;
    cin>>pn;

    vector<bool>is_risky(n, false);

    for(int i = 0; i < pn; i++){
        int a ;
        cin>>a;
        is_risky[a] = true;
    }

    Graph graph(n, is_risky);

    for(int i = 0; i < m; i++){
        int p, q;
        ll r;
        cin>>p>>q>>r;
        

        int risky_count = is_risky[p] + is_risky[q];

        ll eff_wt = r + (risky_count * penalty);

        if(risky_count == 0){
            graph.addSafeEdge(p, q, eff_wt);
        }
        else{
            graph.addRiskyedges(p, q, eff_wt);


        }

        }


        graph.krsukal();



        return 0;
        
    }

    
