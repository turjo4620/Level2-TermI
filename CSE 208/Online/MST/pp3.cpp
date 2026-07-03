/*
Problem 1: Dark Roads
Problem Statement

A city has a network of roads connecting houses. Every road has a maintenance cost. The mayor wants to turn off as many street lights as possible to save money, while ensuring that every house remains reachable from every other house.

Your task is to compute how much money can be saved.

More formally:

There are m houses (numbered from 0 to m-1).
There are n undirected roads.
Each road connects two houses and has a maintenance cost.
Initially, all roads are maintained.
You may remove roads, but the graph must remain connected.

Print the maximum savings.

Input

The input consists of multiple test cases.

Each test case begins with:

m n
m = number of houses
n = number of roads

Then follow n lines:

u v w

meaning there is a road between u and v with cost w.

Input ends with:

0 0
Output

For each test case, output a single integer:

maximum money saved
Constraints
1 ≤ m ≤ 200000
1 ≤ n ≤ 200000
Graph is connected.
Multiple test cases.
*/

#include <bits/stdc++.h>
using ll = long long;

using namespace std;

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

    ll krsukal(){
        sort(edges.begin(), edges.end()); // O(ElogE)
        ll mstCost = 0;

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
        return mstCost;
    }


};


int main(){
    
    int m, n;
    cin>>m>>n;
    Graph graph(m);
    

    ll total_cost = 0;
    for(int i = 0; i < n; i++){
        int p, q;
        ll r;
        cin>>p>>q>>r;
        // 1 based
        // p--; q--;
        total_cost += r;
        graph.addEdge(p, q, r);
    }



    


    cout << (total_cost - graph.krsukal()) << endl;

    return 0;
}