
/*
Problem Statement
The Professor has successfully orchestrated the heist at the Royal Mint of Utopia. Now comes the most
critical part - the escape. The Professor has mapped out various locations in Amaurot, capital of Utopia,
as checkpoints, connected by secret routes known only to the resistance.
Your task is to help the Professor determine the maximum number of completely independent escape
routes from the Royal Mint to the designated safe house. Each route must be independent - no two routes
can share the same passage between checkpoints, ensuring that if the police block one passage, the other
routes remain viable.
The city is mapped as a network where:
• Each location is numbered from 1 to N
• The Royal Mint is always location 1
• The safe house is always location N
• Each passage between locations can only be used by one escape route
• Some passages are one-way due to police patrol patterns
The Professor needs not just the number of independent routes, but also the exact paths for each
route to brief the team.
Input Format
Input starts with an integer T (≤ 100), denoting the number of escape scenarios to analyze.
Each scenario begins with two integers N and M (2 ≤ N ≤ 100, 1 ≤ M ≤ 5000), where N is the
number of locations and M is the number of passages.
Each of the next M lines contains two integers u and v (1 ≤ u, v ≤ N, u 6= v), indicating a one-way
passage from location u to location v.
Output Format
For each scenario, first print “Case X: Y” where X is the case number and Y is the maximum number of
independent escape routes.
If Y > 0, print the next Y lines, each containing one escape route. Each route should be printed as
a sequence of location numbers from 1 to N, separated by ->.
If Y = 0, print “No escape route possible! The Professor needs a new plan.”
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

    int T;
    cin>>T;
    int count = 0;
    while(T--){
    int N, M;
    cin>>N>>M;

    vector<vector<int>>adj(N);
    vector<vector<ll>>res_cap(N, vector<ll>(N, 0));

    for(int i = 0; i < M; i++){
        int a, b;
        cin>>a>>b;
        a--; b--;
        if(res_cap[a][b] == 0){
            adj[a].push_back(b);
            adj[b].push_back(a);
        }
        res_cap[a][b] += 1;
    }


    ll max_flow = edmonds_karp_algorithm(N, 0, N - 1, res_cap, adj);


    cout<<"Case: "<<count+1;
    if(max_flow > 0) cout<<max_flow<<endl;
    else cout<<"No"<<endl;

    count++;

    vector<int>next(N,-1);
    vector<bool>prev(N, false);

    for(int i = 0; i < N; i++){
        for(int v : adj[i]){
            if(res_cap[i][v] != 1){
                next[i] = v;
                prev[v] = true;
            }
        }
    }

    for(int i = 0; i < N; i++){
        if(!prev[i]){
            int curr = i;
            while(curr != -1){
                cout<< curr+1;
                if(next[curr] != -1)cout<<" ->";
                curr = next[curr];
            }
        }
        cout<<'\n';
    }

}
    
    return 0;
}