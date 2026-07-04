/*The present -> B and C section */

#include <bits/stdc++.h>

using namespace std;
using ll = long long;

class Water{
public:
    int x, y;
    int id;

    Water(int id, int x, int y){
        this->id = id;
        this->x = x;
        this->y = y;
    }
};


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

int cost_calculator(int x1, int x2, int y1, int y2){
    return (abs(x1 - x2) + abs(y1 - y2));
}


int main(){
    
    int n;
    cin>>n;
    Graph graph(n);

    vector<Water>supplies;

    for(int i = 0; i < n; i++){
        int p, q;
        cin>>p>>q;
        supplies.push_back(Water(i, p, q));
    }

    for(int i = 0; i < n; i++){
        for(int j = i + 1; j < n; j++){
            graph.addEdge(supplies[i].id, supplies[j].id, cost_calculator(supplies[i].x, supplies[j].x, supplies[i].y, supplies[j].y));
        }
    }

    graph.krsukal();

    return 0;
}