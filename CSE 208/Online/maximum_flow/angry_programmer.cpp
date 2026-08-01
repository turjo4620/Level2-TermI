/*
eita onekta oi special problem er moto

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

    // গ্রাফের সাইজ 2*N এর একটু বেশি রাখলেই হবে
    int s = 2 * N + 2; 
    int src = 1;         // Source হলো 1 (1_in)
    int sink = N + N;    // Sink হলো N (N_out)
    
    vector<vector<int>>adj(s);
    vector<vector<ll>>res_cap(s, vector<ll>(s, 0));

    // Source এবং Sink এর নিজেদের ভাঙার কস্ট অসীম
    adj[1].push_back(1 + N);
    adj[1 + N].push_back(1);
    res_cap[1][1 + N] = 1e16; 

    adj[N].push_back(N + N);
    adj[N + N].push_back(N);
    res_cap[N][N + N] = 1e16;

    for(int i = 1; i <= N - 2; i++){
        int a, c;
        cin >>a>>c;
        int a_in = a;
        int a_out = a + N;

        if(res_cap[a_in][a_out] == 0){
            adj[a_in].push_back(a_out);
            adj[a_out].push_back(a_in);
        }
        // Compilation error fix: shudhu cost ta add kore diba
        res_cap[a_in][a_out] += c;
    }

    for(int i = 1; i <= M; i++){
        int a, b, c;
        cin>>a>>b>>c;
        
        int a_in = a;
        int a_out = a + N;
        int b_in = b;
        int b_out = b + N;

        if(res_cap[a_out][b_in] == 0){
            adj[a_out].push_back(b_in);
            adj[b_in].push_back(a_out);
        }
        res_cap[a_out][b_in] += c;

        // b theke a te connection (karon wires bidirectional)
        if(res_cap[b_out][a_in] == 0){
            adj[b_out].push_back(a_in);
            adj[a_in].push_back(b_out);
        }
        res_cap[b_out][a_in] += c;
    }

    // Function call e 'N' er bodole 's' dite hobe
    ll max_flow = edmonds_karp_algorithm(s, src, sink, res_cap, adj);
    cout<<max_flow<<endl;
    
    return 0;
}