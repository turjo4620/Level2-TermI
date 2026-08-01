/*
A game consists of n rooms and m teleporters. At the beginning of each day, you start in room 1 and you have to reach room n.
You can use each teleporter at most once during the game. How many days can you play if you choose your routes optimally?
Input
The first input line has two integers n and m: the number of rooms and teleporters. The rooms are numbered 1,2,\dots,n.
After this, there are m lines describing the teleporters. Each line has two integers a and b: there is a teleporter from room a to room b.
There are no two teleporters whose starting and ending room are the same.
Output
First print an integer k: the maximum number of days you can play the game. Then, print k route descriptions according to the example. You can print any valid solution.
Constraints

2 \le n \le 500
1 \le m \le 1000
1 \le a,b \le n

Example
Input:
6 7
1 2
1 3
2 6
3 4
3 5
4 6
5 6

Output:
2
3
1 2 6
4
1 3 4 6
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


bool dfs_path_extraction(int u, int sink, vector<vector<int>>& adj, vector<vector<ll>>& orig_cap, vector<vector<ll>>& res_cap, vector<bool>& visited, vector<int>& path){

    path.push_back(u);
    if(u == sink) return true;
    visited[u] = true;

    for(int v : adj[u]){
        if(!visited[v] && orig_cap[u][v]- res_cap[u][v] > 0){
                    if(dfs_path_extraction(v, sink, adj, orig_cap, res_cap, visited, path)){
                        return true;
                    }
        }
    }
    path.pop_back();
    return false;
}


int main(){

    int N, M;
    cin>>N>>M;

    vector<vector<int>>adj(N);
    vector<vector<ll>>res_cap(N, vector<ll>(N, 0));
    vector<vector<ll>>orgcap(N, vector<ll>(N, 0));

    for(int i = 0; i < M; i++){
        int a, b;
        cin>>a>>b;
        a--; b--;
        if(res_cap[a][b] == 0){
            adj[a].push_back(b);
            adj[b].push_back(a);
        }
        ll cap;
        cap = 1;
        res_cap[a][b] += cap;
        orgcap[a][b] += cap;
    }


    ll max_flow = edmonds_karp_algorithm(N, 0, N - 1, res_cap, adj);
    cout<<max_flow<<endl;

    int src = 0;
    int sink = N - 1;
    if(max_flow != 0){
        for(int i = 0; i < max_flow; i++){
            vector<bool>visited(N, false);
            vector<int>path;

            dfs_path_extraction(src, sink, adj, orgcap, res_cap, visited, path);

            cout<<path.size()<<endl;

            for(int u : path){
                cout<<u + 1<<" ";
            }
            cout<<endl;

            for(int j = 0; j < path.size() - 1; j++){
                int u = path[j];
                int v = path[j + 1];

                res_cap[u][v]++;
            }
        }
    }
    
    return 0;
}