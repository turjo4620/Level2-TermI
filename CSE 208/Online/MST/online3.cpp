/*
Bangladesh University of Engineering and Technology
CSE208: Data Structure and Algorithms Sessional II

Online 3

Section: C1 + C2 Time: 35 Minutes

The Interstellar Gates
Humanity has colonized N planets in a 3D sector. The coordinates of the i-th planet
are (xi
, yi
, zi).
To create a trade network, we need to connect all planets. The cost to build a warp
gate between planet i and planet j is the minimum of the coordinate differences:

Cost(i, j) = min( |xi - xj
|, |yi - yj
|, |zi - zj
| )

Calculate the minimum cost to connect all planets so that every planet is reachable
from every other planet.
Note: Because N can be large (100,000), O(N
2
) solution is NOT acceptable.

Input Format
● First line: Integer N.
● Next N lines: Three integers x, y, z describing a planet's location.
Output Format
Single integer: The minimum cost to connect all planets.
Constraints
● 1 ≤ N ≤ 100,000
● 0 ≤ x, y, z ≤ 10
9

Samples
Sample 1
Input Output
2
0 0 0
1 10 20

1

Explanation: Cost = min(|0-1|, |0-10|, |0-20|) = 1.
Sample 2
Input Output
3
0 0 0
10 5 5
20 10 10

10

Sample 3
Input Output
3
1 5 10
2 8 15
7 12 11 */


// #include <bits/stdc++.h>

// using namespace std;
// using ll = long long;

// class Planet {
// public:
//     int id;
//     ll x, y, z;
    
//     Planet(int id, ll x, ll y, ll z) {
//         this->id = id;
//         this->x = x;
//         this->y = y;
//         this->z = z;
//     }
// };

//     bool compX(const Planet &a, const Planet &b){ return a.x < b.x;}
//     bool compY(const Planet &a, const Planet &b){ return a.y < b.y;}
//     bool compZ(const Planet &a, const Planet &b){ return a.z < b.z;}




// class Edge{
// public:
//     int u, v;
//     ll wt;
    
//     Edge(int u , int v, ll wt){
//         this->u = u;
//         this->v = v;
//         this->wt = wt;
//     }

//     bool operator <(const Edge &other) const{
//         return this->wt < other.wt;
//     }
// };

// class Graph{
// public:
//     int V;
//     vector<Edge>edges;
//     vector<int>par, rank;

//     Graph(int V){
//         this->V = V;

//         // for 1 based indexing
//         for(int i = 0; i <= V; i++){
//             par.push_back(i);
//             rank.push_back(0);
//         }
//     }
//     void addEdge(int u, int v, ll wt){
//         edges.push_back(Edge(u, v, wt));
//     }


//     // find function

//     int find(int x){
//         if(par[x] == x){
//             return x;
//         }
//         // path compression
//         return par[x]= find(par[x]);
//     }

//     // union by rank

//     bool unionByRank(int u, int v){
//         int parA = find(u);
//         int parB = find(v);

//         if(parA == parB) return false;
//         if(rank[parA] == rank[parB]){
//             par[parB] = parA;
//             rank[parA]++;    
//         }
//         else if(rank[parA] > rank[parB]){
//             par[parB] = parA;
//         }
//         else{
//             par[parA] = parB;
//         }
//         return true;
//     }

//     ll krsukal(){
//         sort(edges.begin(), edges.end()); // O(ElogE)
//         ll mstCost = 0;

//         int count = 0;

//         for(auto &e : edges){
//             if(unionByRank(e.u, e.v)){
//                 mstCost+=e.wt;
//                 count++;
//                 if(count == V - 1) break;
//             }
//         }
//         return mstCost;
//     }


// };


// int main(){
    
//     int n;
//     cin>>n;
//     Graph graph(n);
    
//     vector<Planet>planets;

//     for(int i = 0; i < n; i++){
//         ll x, y, z; 
//         cin>>x>>y>>z;
//         planets.push_back(Planet(i + 1, x, y, z));
//     }

//     sort(planets.begin(), planets.end(), compX);
//     for(int i = 0; i < n - 1; i++){
//         ll cost = abs(planets[i].x - planets[i + 1].x);
//         graph.addEdge(planets[i].id, planets[i + 1].id, cost);
//     }
//     sort(planets.begin(), planets.end(), compY);
//     for(int i = 0; i < n - 1; i++){
//         ll cost = abs(planets[i].y - planets[i + 1].y);
//         graph.addEdge(planets[i].id, planets[i + 1].id, cost);
//     }
//     sort(planets.begin(), planets.end(), compZ);
//     for(int i = 0; i < n - 1; i++){
//         ll cost = abs(planets[i].z - planets[i + 1].z);
//         graph.addEdge(planets[i].id, planets[i + 1].id, cost);
//     }

//     cout<<graph.krsukal()<<"\n";





    


//     graph.krsukal();

//     return 0;
// }


#include <bits/stdc++.h>

using namespace std;
using ll = long long;


class Planet{
public:
    ll x, y, z;
    int id;

    Planet(int id, ll x, ll y, ll z){
        this->id = id;
        this->x = x;
        this->y = y;
        this->z = z;
    }
};

    bool compX(Planet &a, Planet &b){ return a.x < b.x;}
    bool compY(Planet &a, Planet &b){ return a.y < b.y;}
    bool compZ(Planet &a, Planet &b){ return a.z < b.z;}
    


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
    void addEdge(ll u, ll v, ll wt){
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


int main(){
    
    int n;
    cin>>n;
    Graph graph(n);

    vector<Planet>planets;

    for(int i = 0; i < n; i++){
        ll p, q, r;
        cin>>p>>q>>r;
        planets.push_back(Planet(i, p, q, r));
    }

    sort(planets.begin(), planets.end(), compX);
    for(int i = 0; i < n - 1; i++){
        ll cost = abs(planets[i].x - planets[i + 1].x);
        graph.addEdge(planets[i].id, planets[i + 1].id, cost);
    }
    sort(planets.begin(), planets.end(), compY);
    for(int i = 0; i < n - 1; i++){
        ll cost = abs(planets[i].y - planets[i + 1].y);
        graph.addEdge(planets[i].id, planets[i + 1].id, cost);
    }
    sort(planets.begin(), planets.end(), compZ);
    for(int i = 0; i < n - 1; i++){
        ll cost = abs(planets[i].z - planets[i + 1].z);
        graph.addEdge(planets[i].id, planets[i + 1].id, cost);
    }

    graph.krsukal();

    return 0;
}