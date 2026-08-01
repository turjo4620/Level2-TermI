// min cut er concept

/*
Kaaleppi has just robbed a bank and is now heading to the harbor. However, the police wants to stop him by closing some streets of the city.
What is the minimum number of streets that should be closed so that there is no route between the bank and the harbor?
Input
The first input line has two integers n and m: the number of crossings and streets. The crossings are numbered 1,2,\dots,n. The bank is located at crossing 1, and the harbor is located at crossing n.
After this, there are m lines that describing the streets. Each line has two integers a and b: there is a street between crossings a and b. All streets are two-way streets, and there is at most one street between two crossings.
Output
First print an integer k: the minimum number of streets that should be closed. After this, print k lines describing the streets. You can print any valid solution.
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

    vector<vector<int>>adj(N);
    vector<vector<ll>>res_cap(N, vector<ll>(N, 0));
    vector<vector<ll>>org_cap(N, vector<ll>(N, 0));

    for(int i = 0; i < M; i++){
        int a, b;
        cin>>a>>b;
        a--; b--;
        if(res_cap[a][b] == 0 && res_cap[b][a] == 0){
            adj[a].push_back(b);
            adj[b].push_back(a);
        }
        ll cap;
        cap = 1;
        res_cap[a][b] += cap;
        org_cap[a][b] += cap;
        res_cap[b][a] += cap;
        org_cap[b][a] += cap;
    }


    ll max_flow = edmonds_karp_algorithm(N, 0, N - 1, res_cap, adj);
    
    vector<bool>visited(N);
    queue<int> q;

    visited[0] = true;
    q.push(0);

    while(!q.empty()){
        int u = q.front();
        q.pop();

        for(int v : adj[u]){
            if(!visited[v] && res_cap[u][v] > 0){
                visited[v] = true;
                q.push(v);
            }
        }
    }

    cout<<max_flow<<endl;
    for(int i = 0; i < N; i++){
        if(visited[i]){
            for(int u : adj[i]){
                if(!visited[u] && org_cap[i][u] > 0){
                    cout<<i +1 <<" "<<u + 1<<endl;
                }
            }
        }
    }
    
    return 0;
}