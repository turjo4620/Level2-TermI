#include <bits/stdc++.h>

using namespace std;

class Edge{
public:
    int u, v, wt;
    Edge(int u, int v, int wt){
        this->u = u;
        this->v = v;
        this->wt = wt;
    }

    // bool operator <(const Edge &other) const{
    //     return this->wt < other.wt;
    // }
};

class Graph{
public:
    int V;
    vector<Edge>edges;
    vector<vector<pair<int, int>>>adj;

    Graph(int V){
        this->V = V;
        adj.resize(V);
    }

    void addEdge(int u, int v, int wt){
        edges.push_back(Edge(u, v, wt));
        adj[u].push_back({v, wt});
        adj[v].push_back({u, wt});
    }

    //prims algorithm

    int primMST(){
        vector<bool>inMST(V, false);
        // priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>>pq;

        // max heap
        // min heap -> greater pass
        // priority_queue<int, vector<int>, greater<int>>pq;

        priority_queue<pair<int ,int>, vector<pair<int, int>>, greater<pair<int, int>>>pq;

        
        vector<int>key(V, INT_MAX);
        vector<int>parent(V, -1);

        int mstCost = 0, cnt = 0;

        pq.push({0, 0}); // source node -> {wt, vertex}
        key[0] = 0;

        while(pq.size() > 0){
            auto p = pq.top();
            int wt = p.first;
            int u = p.second;
            pq.pop();
            
            if(!inMST[u]){
                inMST[u] = true;
                mstCost += wt;
                cnt++;

                for(int i = 0; i < adj[u].size(); i++){
                    int v = adj[u][i].first;
                    int w = adj[u][i].second;

                    if(!inMST[v] && w < key[v]){
                        key[v] = w;
                        parent[v] = u;
                        pq.push({w, v});
                    }
                }

            }
        }
        if(cnt != V){
            return -1;
        }
        return mstCost;
    }
};



int main(){
    
    Graph graph(4);

    graph.addEdge(0, 1, 10);
    graph.addEdge(0, 2, 6);
    graph.addEdge(0, 2, 5);
    graph.addEdge(1, 3, 15);
    graph.addEdge(2, 3, 4);

    cout << graph.primMST() << endl;

    return 0;
}


/*
prims algorithm

class Edge{
public:
    int u, v, wt;
    
    Edge(int u, int v, int wt){
        this->u = u;
        this->v = v;
        this->wt = wt;
    }
};

class Graph{
public:
    int V;
    vector<Edge>edges;
    vector<vector<pair<int, int>>>adj;

    Graph(int V){
        this->V = V;
        adj.resize(V);
    }

    void addEdge(int u, int v, int wt){
        edges.push_back(Edge(u, v, wt));
        adj[u].push_back(v, wt);
        adj[v].push_back(u, wt);
    }

    int prims(){
        vector<bool>inMST(V, false);
        // min heap
        priority_queue<pair<int, int>, vector<pair<int, int>>>pq;
        int mstCost = 0, cnt = 0;

        pq.push({0, 0}) // src -> {wt, vertex}

        while(pq.size() > 0){
            auto p = pq.top();
            int wt = p.first;
            int u = p.second;

            if(!inMST[u]){
                inMST[u] = true;
                mstCost += wt;
                cnt++;

                for(int i = 0; i < edges.size(); i++){
                    int v = adj[u][i].second;
                    int w = adj[u][i].first;

                    if(!inMST[v]){
                        pq.push({w, v});    
                    }
                }
            }
        }
            return mstCost;
    }
}
*/


