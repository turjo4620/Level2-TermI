#include <bits/stdc++.h>

using namespace std;

class Graph{
public:
    int V;
    vector<vector<int>>adj;

    Graph(int V){
        this->V = V;
        adj.resize(V);
    }

    void addEdge(int u, int v){
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
};



int main(){
    Graph graph(10);

    graph.addEdge(0, 1);
    
    return 0;
}