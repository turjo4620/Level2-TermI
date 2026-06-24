#include <bits/stdc++.h>

using namespace std;

class Edge{
public:
    int u, v, wt;
    
    Edge(int u , int v, int wt){
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
    vector<int>par, rank;

    Graph(int V){
        this->V = V;

        for(int i = 0; i < V; i++){
            par.push_back(i);
            rank.push_back(0);
        }
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
        int parA = find(u);
        int parB = find(v);

        if(parA == parB) return;
        if(rank[parA] == rank[parB]){
            par[parB] = parA;
            rank[parA]++;    
        }
        else if(rank[parA] > rank[parB]){
            par[parB] = parA;
        }
        else{
            par[parA] = parB;
        }
    }

    void krsukal(){
        sort(edges.begin(), edges.end()); // O(ElogE)
        long long mstCost = 0;

        int count = 0;

        for(size_t i = 0; i < edges.size() && count < V - 1; i++){
            Edge e = edges[i];

            int parU = find(e.u);
            int parV = find(e.v);

            if(parU != parV){ // no cycle
                unionByRank(e.u, e.v);
                mstCost += e.wt;
                count++;
            }

        }
        if(count != V-1){
            cout << "IMPOSSIBLE\n";
            return;
        }
        cout << mstCost << '\n';
    }


};


int main(){
    
    int n, m;
    cin>>n>>m;

    Graph graph(n);

    for(int i = 0; i < m; i++){
            int a, b, c;
            cin>>a>>b>>c;
            graph.addEdge(a - 1,b - 1,c);
    }



    graph.krsukal();

    return 0;
}