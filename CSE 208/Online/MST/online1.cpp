/*
Bangladesh University of Engineering and Technology
CSE208: Data Structure and Algorithms Sessional II

Online 3

Section: A1 + A2 Time: 35 Minutes

Electrifying the Kingdom
The Kingdom of Valoria has N cities. To bring the kingdom into the modern age,
the King wants to provide electricity to every city. There are two ways to provide
electricity to a city:
1. Build a Power Station: You can build a power station directly inside city i
for a cost of Pi
.

2. Connect to the Grid: You can run a transmission cable between city i and
city j with cost Ci,j
.
Rules of Electricity:
● If city i has a power station, it has electricity.
● If city i is connected via cable to city j, and city j has electricity (either from
a station or another cable connection), then city i also has electricity.
You need to minimize the total cost so that every city has access to electricity.
Input Format
● First line: Integer N (number of cities).
● Second line: N space-separated integers representing P1
, P2
, ..., PN (cost to

build a station in each city).
● Third line: Integer M (number of potential cables).
● Next M lines: Three integers u, v, w (cable between city u and v with cost w).

Output Format
Print a single integer: the minimum total cost to electrify all cities.
Constraints
● 1 ≤ N ≤ 2,000
● 1 ≤ M ≤ N × (N-1) / 2
● 1 ≤ Pi
, w ≤ 10
9

Samples
Sample 1
Input Output
3
10 20 30
2
1 2 5
2 3 5

20

Explanation: Build station at City 1 (Cost 10). Connect City 1 to 2 (Cost 5).
Connect City 2 to 3 (Cost 5). Total: 10 + 5 + 5 = 20.

*/



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
    
    int n;
    cin>>n;
    Graph graph(n + 1);
    
    for(int i = 1 ; i <= n; i++){
        int a; 
        cin>>a;
        graph.addEdge(0, i, a);
    }

    int m;
    cin>>m;
    for(int i = 0; i < m; i++){
        int p, q, r;
        cin>>p>>q>>r;
        graph.addEdge(p, q, r);
    }



    


    graph.krsukal();

    return 0;
}