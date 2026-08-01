/*
Problem: Router Capacity Constrained Maximum Bandwidth

You are given a computer network consisting of N routers and M directed communication links.

Each communication link has a maximum bandwidth. A link from router u to router v with bandwidth c means that at most c Mbps of data can be transferred through that link.

However, every router also has a processing capacity. A router can forward only a limited amount of data through itself, regardless of how much bandwidth its incoming and outgoing links have.

Your task is to find the maximum amount of data that can be transferred from the source router S to the destination router T while satisfying both:

No communication link can carry data exceeding its bandwidth limit.
No router can process more data than its processing capacity.
Input Format

The first line contains:

N M
N = number of routers
M = number of directed communication links

The second line contains:

S T
S = source router
T = destination router

The next N lines contain:

router_id processing_capacity

where:

router_id is the router number (0 to N-1)
processing_capacity is the maximum amount of data the router can process.

The next M lines contain:

u v bandwidth

where:

u = starting router
v = ending router
bandwidth = maximum bandwidth of the link u -> v
Output Format

Print a single integer:

maximum possible data transfer rate from S to T

5 6
0 4

0 100
1 50
2 80
3 40
4 100

0 1 1000
0 2 500
1 3 600
2 3 700
3 4 1000
2 4 200

*/


#include <bits/stdc++.h>

using namespace std;
using ll = long long;


// struct edge{
//     int u, v;
//     ll wt;
// };


bool bfs_helper(vector<vector<int>>&adj, int src, int sink, vector<vector<ll>>&res_capacity, vector<int>&parent){
    fill(parent.begin(), parent.end(), -1);
    queue<int>q;
    q.push(src);

    vector<bool>visited(adj.size());

    visited[src] = true;

    while(!q.empty()){
        int u = q.front();
        q.pop();

        // storing u

        for(int v : adj[u]){
            if(!visited[v] && res_capacity[u][v] > 0){
                visited[v] = true;
                parent[v] = u;
                q.push(v);
            }
        }
    }
    return visited[sink];
}


ll edmonds_karp_algorithm(int V, int src, int sink, vector<vector<ll>>&res_capacity, vector<vector<int>>&adj){

    if(src == sink) return 0;
    
    ll total_flow = 0;

    vector<int>parent(V, -1);

    while(bfs_helper(adj, src, sink, res_capacity, parent)){
        ll min_res_capacity = LLONG_MAX;
        int curr = sink;

        while(curr != src){
            int par = parent[curr];
            min_res_capacity = min(min_res_capacity, res_capacity[par][curr]);
            curr = par;
        }

        curr = sink;
        while(curr != src){
            int p = parent[curr];
            res_capacity[p][curr] -= min_res_capacity;
            res_capacity[curr][p] += min_res_capacity;
            curr = p;
        }

        total_flow += min_res_capacity;
    }

    return total_flow;
    

}


int main(){

    int N, M;
    cin>>N>>M;

    int src, dest;
    cin>>src>>dest;

    int s = 2 * N + 2;

    vector<vector<int>>adj(s);
    vector<vector<ll>>res_cap(s, vector<ll>(s, 0));

    for(int i = 0; i < N; i++){
        int a, b;
        cin>>a>>b;
        int a_in = a;
        int a_out = a + N;
        if(res_cap[a_in][a_out] == 0){
            adj[a_in].push_back(a_out);
            adj[a_out].push_back(a_in);
        }
        res_cap[a_in][a_out] = b;
        // source keo split nah korle!
        /*
        if(a==src || a==dest)
            res_cap[a_out][a_in]=1e9;
        else
            res_cap[a_out][a_in]=b; */
        }


    for(int i = 0; i < M; i++){
        int a, b, c;
        cin>>a>>b>>c;
        int a_in = a;
        int a_out = N + a;
        int b_in = b;
        int b_out = b + N;
        if(res_cap[a_out][b_in] == 0){
            adj[a_out].push_back(b_in);
            adj[b_in].push_back(a_out);
        }
        res_cap[a_out][b_in] += c;
    }

    ll max_flow = edmonds_karp_algorithm(s, src, dest + N, res_cap, adj); // dest o kintu in out hoise!
    cout<<max_flow<<endl;
    
    return 0;
}
