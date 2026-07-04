/*
Graphland Transportation System
Problem Statement
In the country of Graphland, there are n cities but no roads or railroads yet. The government wants to connect all cities through an optimal transportation network that minimizes construction cost.

Graphland uses the following rule:

If the distance between two cities is at most r, they belong to the same state, and can be connected by a road.
If the distance is greater than r, the cities are in different states, and must be connected by a railroad.
The cost of building a road or a railroad is equal to the Euclidean distance between the two cities. Your task is to help Graphland build a minimum-cost network such that:

All cities are connected directly or indirectly.
The total length of roads and railroads used is minimized.
You must report how many states are formed.
Input Format
n r
x1 y1
x2 y2
...
xn yn
n — number of cities
r — distance threshold to determine if two cities are in the same state
Each of the next n lines contains two integers xᵢ and yᵢ, representing coordinates of the cities
Output Format
Print a single line containing three space-separated values:

<states> <total_road_length> <total_railroad_length>
<states> — the number of connected states
<total_road_length> — total length of roads used (rounded to nearest integer)
<total_railroad_length> — total length of railroads used (rounded to nearest integer)
Notes
Distance between cities is calculated using the Euclidean formula:
sqrt((x₂ - x₁)² + (y₂ - y₁)²)
Two cities are considered to be in the same state if they can be connected by a series of roads where all edges are ≤ r.
Each railroad edge connects two separate states, so:
number_of_states = number_of_railroad_edges + 1
Sample Input 1
4 10
0 0
0 10
10 0
10 10
Sample Output 1
1 30 0
Sample Input 2
5 5
0 0
0 5
5 0
10 10
20 20
Sample Output 2
3 10 25
Sample Input 3
3 100
0 0
1 0
2 0
Sample Output 3
1 2 0
Sample Input 4
3 1
0 0
100 0
200 0
Sample Output 4
3 0 200
Sample Input 5
4 20
0 0
40 30
30 30
10 10
Sample Output 5
2 24 28
*/


#include <bits/stdc++.h>
using ll = long long;

using namespace std;

class Edge{
public:
    int u, v;
    double wt;
    
    Edge(int u , int v, double wt){
        this->u = u;
        this->v = v;
        this->wt = wt;
    }

    bool operator <(const Edge &other) const{
        return this->wt < other.wt;
    }
};


class City{
public:
    int x, y; 
    int id;

    City(int id, int x, int y){
        this->id = id;
        this->x = x;
        this->y = y;
    }
};


class Graph{
public:
    int V;
    vector<Edge>edges;
    vector<int>par, rnk;

   
    vector<Edge>mstedges;


    Graph(int V){
        this->V = V;

        for(int i = 0; i < V; i++){
            par.push_back(i);
            rnk.push_back(0);
        }
    }
    void addEdge(int u, int v, double wt){
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

    double krsukal(){
        sort(edges.begin(), edges.end()); // O(ElogE)
        double mstCost = 0;

        int count = 0;

        for(int i = 0; i < edges.size(); i++){
            Edge e = edges[i];

            int parU = find(e.u);
            int parV = find(e.v);

            if(parU != parV){ // no cycle
                unionByRank(e.u, e.v);
                mstCost += e.wt;

                mstedges.push_back(e); // direct edge gulakei push kore dibo done

                
                
            }

        }
        return mstCost;
    }


};

double cost_calculator(int x1, int x2, int y1, int y2){
    return sqrt((1.0 * (x1 - x2) * (x1 - x2)) + ((y1 - y2) * (y1 - y2)  * 1.0));
}


int main(){
    
    int m, td;
    cin>>m>>td;
    Graph graph(m);
    

    vector<City>cities;
    
    for(int i = 0; i < m; i++){
        int p, q;
        cin>>p>>q;
        cities.push_back(City(i, p, q));
    }

    for(int i = 0; i < m; i++){
        for(int j = i + 1; j < m; j++){
            graph.addEdge(cities[i].id, cities[j].id, cost_calculator(cities[i].x, cities[j].x, cities[i].y, cities[j].y));
        }
    }

    double mstCost = graph.krsukal();

    int count = 0;
    double road_length = 0, rail_length = 0;
    for(Edge e : graph.mstedges){
        if(e.wt > td) {
            count++;
            rail_length += e.wt;
        }
        else{
            road_length += e.wt;
        }
    }



    cout<<count+1<<" "<<(ll)road_length<<" "<<(ll)rail_length<<endl;







    return 0;
}
