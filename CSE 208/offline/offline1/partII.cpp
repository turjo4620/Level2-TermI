#include <bits/stdc++.h>

using namespace std;
using ll = long long;

struct flight{
    string flight_id;
    string departure_city;
    string arrival_city;
    int departure_time;
    int arrival_time;
};


bool bfs_helper(vector<vector<int>>&adj, int src, int sink, vector<vector<ll>>&res_capacity, vector<int>&parent){
    queue<int>q;
    q.push(src);

    parent[src] = -1;

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

    vector<flight>flights;

    int n;
    cin>>n;

    for(int i = 0; i < n; i++){
        string flight_id, departure_city, arrival_city, departure_time, arrival_time;

        cin>>flight_id>>departure_city>>arrival_city>>departure_time>>arrival_time;

        int d_t = stoi(departure_time.substr(0, 2)) * 60 + stoi(departure_time.substr(3, 2));
        int a_t = stoi(arrival_time.substr(0, 2)) * 60 + stoi(arrival_time.substr(3, 2));

        flights.push_back(flight{flight_id, departure_city, arrival_city, d_t, a_t});

        }

            // step-2 feasible connection

        vector<vector<int>>adj_for_dag(n);

        for(int i = 0;  i < n; i++){
            for(int j = 0; j < n; j++){
                if(i == j) continue; 

                if((flights[i].arrival_city == flights[j].departure_city) && flights[i].arrival_time + 180 <= flights[j].departure_time){
                    adj_for_dag[i].push_back(j);
                }
            }
        }


        // step-3 

        int total_node = 2 * n + 2;
        int src  = 0;
        int sink = 2*n + 1;

        vector<vector<int>>adj(total_node);
        vector<vector<ll>>res_cap(total_node, vector<ll>(total_node, 0));

        for(int i = 1 ; i <= n; i++){
            adj[0].push_back(i);
            adj[i].push_back(0);
            res_cap[0][i] = 1;

            adj[2*n + 1].push_back(i + n);
            adj[i + n].push_back(2*n + 1);
            res_cap[i + n][2*n + 1] = 1;
        }


        for(int i = 1; i <= n; i++){
            for(int j = 1; j <= n; j++){
                if(i==j)continue;
                if((flights[i - 1].arrival_city == flights[j - 1].departure_city) && flights[i - 1].arrival_time + 180 <= flights[j - 1].departure_time){
                    adj[i].push_back(j + n);
                    adj[j + n].push_back(i);

                    res_cap[i][j+n] = 1;
                }
            }
        }

        ll maximum_matching = edmonds_karp_algorithm(total_node, src, sink, res_cap, adj);
        ll min_path_cover = n - maximum_matching;

        cout<<"Number of Aircraft: "<<min_path_cover<<endl;
        

        vector<int>next(n + 1, -1);
        vector<int>prev(n + 1, false);

        for(int i = 1; i <= n; i++){
            for(int v : adj[i]){
                if(v > n && v <= 2*n + 1 && res_cap[i][v] == 0){
                    next[i] = v - n;
                    prev[v - n] = true;
                }
            }
        }

        int count = 1;
        for(int i = 1; i <= n; i++){
            if(!prev[i]){
                cout<<"Aircraft "<<count<<": ";
                int curr = i;
                while(curr != -1){
                    cout<<flights[curr - 1].flight_id;
                    if(next[curr] != -1) cout<<"->";
                    curr = next[curr];
                }
                count++;
                cout<<"\n";
            }
        }

    
    return 0;
}