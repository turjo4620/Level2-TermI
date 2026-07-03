
/*
Problem 2: Rebuilding the Archipelago

**Problem Statement**
A massive storm has recently swept through an archipelago consisting of `N` islands. Before the storm, there were many bridges connecting the islands. After the storm, only `K` bridges survived and are still fully functional.

The government wants to ensure that all islands are fully connected again (i.e., you can travel from any island to any other island using a sequence of bridges). You are given a list of `M` possible new bridges that can be constructed, along with their construction costs.

Since the government is short on funds, they want to spend the minimum possible amount of money to rebuild the network. The surviving bridges cost nothing to keep and use.

Calculate the minimum construction cost required to make all islands connected.

**Input Format**

* The first line contains three integers: `N` (number of islands), `K` (number of surviving bridges), and `M` (number of potential new bridges).
* Each of the next `K` lines contains two integers `u` and `v`, indicating that a surviving bridge already connects island `u` and island `v`.
* Each of the next `M` lines contains three integers `u`, `v`, and `w`, indicating that a new bridge can be built between island `u` and island `v` at a cost of `w`.

**Constraints**

* 1 ≤ N ≤ 100,000
* 0 ≤ K ≤ 100,000
* 0 ≤ M ≤ 200,000
* 1 ≤ u, v ≤ N
* 1 ≤ w ≤ 10^9

**Output Format**

* Print a single integer representing the minimum total cost to connect all the islands. If it is impossible to connect all islands, print `-1`.

**Sample Input 1**

```text
4 1 4
1 2
2 3 5
3 4 2
1 4 4
1 3 10

```

**Sample Output 1**

```text
6

```

**Explanation for Sample 1**
Islands 1 and 2 are already connected (cost = 0).
We can build a bridge between 3 and 4 (cost = 2).
We can build a bridge between 1 and 4 (cost = 4).
Total cost = 0 + 2 + 4 = 6. All islands (1, 2, 3, 4) are now connected. */

#include <bits/stdc++.h>

using namespace std;
using ll = long long;


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
    vector<int>par, rank;

    Graph(int V){
        this->V = V;

        // for 1 based indexing
        for(int i = 0; i <= V; i++){
            par.push_back(i);
            rank.push_back(0);
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

    bool unionByRank(int u, int v){
        int parA = find(u);
        int parB = find(v);

        if(parA == parB) return false;
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
        return true;
    }

    ll krsukal(){
        sort(edges.begin(), edges.end()); // O(ElogE)
        ll mstCost = 0;

        int count = 0;

        for(auto &e : edges){
            if(unionByRank(e.u, e.v)){
                mstCost+=e.wt;
                count++;
                if(count == V - 1) break;
            }
        }
        if(count == V - 1) return mstCost ;
        return -1;
    }


};


int main(){
    
    int n;
    cin>>n;
    Graph graph(n);
    
    int k, m; 
    cin >>k>>m;

    for(int i = 0; i < k; i++){
        int a, b;
        cin>>a>>b;
        graph.addEdge(a, b, 0);
    }

    for(int i = 0; i < m; i++){
        int p, q;
        ll r;
        cin>>p>>q>>r;

        graph.addEdge(p, q, r);
    }

    

    cout<<graph.krsukal()<<"\n";





    



    return 0;
}