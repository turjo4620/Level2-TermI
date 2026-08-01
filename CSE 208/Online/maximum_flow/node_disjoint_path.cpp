/*
b secion er ekta variation 
    b section e bolse path share kora jabe nah
    ekhnae bolbe node share kora jabe nah
    Path A: 0 -> 1 -> 3 -> 4 -> 6

    Path B: 0 -> 2 -> 3 -> 5 -> 6

    duita valid path jara same link share kore nah right?
    accha ekhoner problem er jonno dekho 3 share korte parbe nah, duita path samee node share korlei jhamela!

    amra ekhon pura jinishtake duita bhage bhag kore felbo

    u -> u + N
    out -> in 

    tailei mile jabe

    source ar sink er jonno capacity hobe INF ar normally hobe 1
*/

#include <bits/stdc++.h>

using namespace std;
using ll = long long;

const ll INF = 1e8;


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

    int N, M, src, sink;
    cin>>N>>M>>src>>sink;

    int s = 2* N;
    vector<vector<int>>adj(s);
    vector<vector<ll>>res_cap(s, vector<ll>(s, 0));


    // node split kortechi

    for(int i = 0; i < N; i++){
        int in = i;
        int out = i + N;

        adj[in].push_back(out);
        adj[out].push_back(in);

        if(i == src || i == sink){
            res_cap[in][out] = INF;
        }
        else{
            res_cap[in][out] = 1;
        }
    }

    for(int i = 0; i < M; i++){
        int a, b;
        cin>>a>>b;

        int a_out = a + N;
        int b_in = b;
        if(res_cap[a_out][b_in] == 0 && res_cap[b_in][a_out]){
        adj[a_out].push_back(b_in);
        adj[b_in].push_back(a_out);
        }
        res_cap[a_out][b_in] += 1;
    }


    ll max_flow = edmonds_karp_algorithm(N, 0, N - 1, res_cap, adj);
    cout<<max_flow<<endl;
    
    return 0;
}