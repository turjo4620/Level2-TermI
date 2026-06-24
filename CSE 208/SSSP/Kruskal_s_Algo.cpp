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
        int mstCost = 0;

        int count = 0;

        for(int i = 0; i < edges.size() && count < V - 1; i++){
            Edge e = edges[i];

            int parU = find(e.u);
            int parV = find(e.v);

            if(parU != parV){ // no cycle
                unionByRank(e.u, e.v);
                mstCost += e.wt;
                count++;
            }

        }
        cout<<"MST cost: "<<mstCost<<endl;
    }


};


int main(){
    
    Graph graph(4);

    graph.addEdge(0, 1, 10);
    graph.addEdge(0, 2, 6);
    graph.addEdge(0, 2, 5);
    graph.addEdge(1, 3, 15);
    graph.addEdge(2, 3, 4);


    graph.krsukal();

    return 0;
}