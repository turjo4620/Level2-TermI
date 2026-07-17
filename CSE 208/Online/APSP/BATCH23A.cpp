#include <bits/stdc++.h>

using namespace std;
using ll = long long;
const ll INF = 1e18;



// can't use map, so custom index function needed

int getIdx(vector<string>& names, string query){
    for(int i = 0; i < names.size(); i++){
        if(names[i] == query){
            return i;
        }
    }
    return -1;
}


class Edge{
public:
    int u, v;
    double wt;

    Edge(int u, int v, double wt){
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
    vector<string>names;

    Graph(int V, vector<string>&names){
        this->V = V;
        this->names = names;
    }

    void addEdge(int u, int v, double wt){
        edges.push_back(Edge(u, v,wt));
    }

    void floydWarshall(){
        vector<vector<double>>dist(V, vector<double>(V, 0)); // initalized with 0
        
        for(int i = 0; i < V; i++){
            dist[i][i] = 1; // dollar to dollar -> 1
        }

        for(Edge e : edges){
            dist[e.u][e.v] = max(dist[e.u][e.v] , e.wt);
        }

        for(int k = 0; k < V; k++){
            for(int i = 0; i < V; i++){
                for(int j = 0; j < V; j++){
                    if(dist[i][k] != INF && dist[k][j] != INF){
                        dist[i][j] = max(dist[i][j], dist[i][k] * dist[k][j]);
                    }
                }
            }
        }

        bool found = false;
        for(int i = 0; i < V; i++){
            if(dist[i][i] > 1.0){
                cout<<names[i]<<endl;
                found = true;
            }
        }

        if(!found){
            cout<<"No Arbitrage\n";
        }
    }
};




int main() {
    int n;
    cin>>n;

    vector<string>names(n);

    for(int i = 0; i < n; i++){
        cin>>names[i];
    }

    Graph graph(n, names);

    int m;
    cin>>m;
    
    for (int i = 0; i < m; i++) {
        // int p, q;
        // double r;
        // cin >> p >> q >> r;
        // graph.addEdge(p, q, r);

        string u_name, v_name;
        double rate;
        cin>>u_name>>rate>>v_name;

        int u = getIdx(names, u_name);
        int v = getIdx(names, v_name);

        graph.addEdge(u, v, rate);    
    }

    graph.floydWarshall();

    return 0;
}