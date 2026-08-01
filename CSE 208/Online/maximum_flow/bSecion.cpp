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

    int src;
    int sink;
    cin>>src>>sink;

    vector<vector<int>>adj(N);
    vector<vector<ll>>res_cap(N, vector<ll>(N, 0));

    for(int i = 0; i < M; i++){
        int a, b;
        cin>>a>>b;
        if(res_cap[a][b] == 0){
            adj[a].push_back(b);
            adj[b].push_back(a);
        }
        res_cap[a][b] += 1;
    }


    ll max_flow = edmonds_karp_algorithm(N, src, sink, res_cap, adj);
    if(max_flow <= 0){
        cout<<-1<<endl;
    }
    else{
        cout<<max_flow<<endl;
    }
    
    return 0;
}