
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


    // path extraction
    bool extract_path_dfs(int u, int sink, vector<vector<int>>& adj, vector<vector<ll>>& orig_cap, 
                      vector<vector<ll>>& res_cap, vector<bool>& visited, vector<int>& path){

            path.push_back(u);
            if(u == sink){
                return true;
            }

            visited[u] = true;

            for(int v : adj[u]){
                if(!visited[v] && (orig_cap[u][v] - res_cap[u][v] > 0)){
                    if(extract_path_dfs(v, sink, adj, orig_cap, res_cap, visited, path)){
                        return true;
                    }
                }
            }

            path.pop_back();
            return false;
        }



int main(){

    int T;
    cin>>T;

    for(int tc = 1; tc <= T; tc++){
        int N, M;
        cin>>N>>M;

        int s = N;
        int src = 0;
        int sink = N - 1;
        vector<vector<int>> adj(s);
        vector<vector<ll>> orig_cap(s, vector<ll>(s, 0)); // We need to remember original capacities
        vector<vector<ll>> res_cap(s, vector<ll>(s, 0));


    for (int i = 0; i < M; i++) {
            int u, v;
            cin >> u >> v;
            u--; v--;
            if (res_cap[u][v] == 0 && res_cap[v][u] == 0) {
                adj[u].push_back(v);
                adj[v].push_back(u);
            }
            // Capacity is 1 per passage
            orig_cap[u][v] += 1; 
              res_cap[u][v] += 1;  
        }   
        
        int max_flow = edmonds_karp_algorithm(s, src, sink, res_cap, adj);

        cout << "Case " << tc << ": " << max_flow << "\n";

        if(max_flow == 0){
            cout<<"NONE"<<endl;
        }
        else{
            for(int i = 0; i < max_flow; i++){
                vector<int>path;
                vector<bool> visited(s, false);
                
                extract_path_dfs(src, sink, adj, orig_cap, res_cap, visited, path);

                for(int j = 0; j < path.size(); j++){
                    cout<<path[j] + 1;
                    if(j < path.size() - 1) cout<<" -> ";
                }
                cout<<endl;

                // abar ager moto baniye fellam jate abar eita choose nah kore
                for(int j = 0; j < path.size() - 1; j++){
                    int u = path[j];
                    int v = path[j + 1];
                    res_cap[u][v]++;
                }
            }
        }
    }

    
    return 0;
}