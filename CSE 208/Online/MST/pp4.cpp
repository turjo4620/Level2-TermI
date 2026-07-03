/*
🧩 Problem 2: Arctic Network
Problem Statement

A group of outposts (stations) are located in a remote region. The government wants to ensure communication between all outposts.

They can use:

Satellite links (free, unlimited range, but only S satellites available)
Radio links (cable connections between outposts, each with a cost equal to Euclidean distance)
Goal

You must connect all outposts such that:

The network is fully connected
You are allowed to use at most S satellite connections
Remaining connections must be made using radio links
Key Idea of Cost

If two outposts are at coordinates:

(x1, y1) and (x2, y2)

Then radio cost is:

(x1−x2)
2
+(y1−y2)
2
	​

Task

Find the minimum possible maximum distance used for radio connections after optimally placing satellites.

Input Format

Each test case:

S P
S = number of satellites
P = number of outposts

Then P lines:

x y

Coordinates of each outpost.

Output

For each test case:

Print:

minimum maximum spacing

(precision up to 2 decimal places)

Constraints
1 ≤ P ≤ 1000
1 ≤ S ≤ P
Multiple test cases until EOF
*/


#include <bits/stdc++.h>
using d = double;

using namespace std;

class Outpost{
public:
    int id;
    d x, y;

    Outpost(int id, d x, d y){
        this->id = id;
        this->x = x;
        this->y = y;
    }
};


class Edge{
public:
    int u, v;
    d wt;
    
    Edge(int u , int v, d wt){
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
    void addEdge(int u, int v, d wt){
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
        int pu = find(u);
        int pv = find(v);

        if(pu == pv) return;

        if(rnk[pu] < rnk[pv]){
            par[pu] = pv;
        }
        else if(rnk[pu] > rnk[pv]){
            par[pv] = pu;
        }
        else{
            par[pv] = pu;
            rnk[pu]++;
        }
    }   

    vector<d> krsukal(){
        sort(edges.begin(), edges.end()); // O(ElogE)
        // ll mstCost = 0;
        vector<d>mstcost;

        int count = 0;

        for(int i = 0; i < edges.size() && count < V - 1; i++){
            Edge e = edges[i];

            int parU = find(e.u);
            int parV = find(e.v);

            if(parU != parV){ // no cycle
                unionByRank(e.u, e.v);
                // mstCost += e.wt;
                mstcost.push_back(e.wt);
                count++;
            }

        }
        return mstcost;
    }


};

d cost_calculator(int x1, int x2, int y1, int y2){
    return sqrt(((x1 - x2) * (x1 - x2)) + (y1 - y2) * (y1 - y2));
}


int main(){
    
    int S, P;
    cin>>S>>P;
    Graph graph(P);
    

    vector<Outpost>points;

    for(int i = 0; i < P; i++){
        int x, y;
        cin>>x>>y;
        points.push_back(Outpost(i, x, y));
    }

    for(int i = 0; i < P; i++){
        for(int j = i + 1; j < P; j++){
            graph.addEdge(points[i].id, points[j].id, cost_calculator(points[i].x, points[j].x, points[i].y, points[j].y));
        }
    }

   vector<d>cost = graph.krsukal();
   sort(cost.begin(), cost.end());

   int remove = S - 1;
   if(remove >= cost.size()){
    cout<<0.00<<endl;
    return 0;
   }
   
    double ans = cost[cost.size() - 1 - remove];
    
    cout << fixed << setprecision(2) << ans << "\n";


    // cout << (total_cost - graph.krsukal()) << endl;

    return 0;
}