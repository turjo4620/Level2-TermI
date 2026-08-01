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

    struct person{
      int exp;
      int age;
      int record;
      int origin;
      int weapon;
      int trust;
      int lang;  
    };


    bool is_compatible(person a, person b) {
    // 1. Experience Gap <= 12
    if (abs(a.exp - b.exp) > 12) return false;
    
    // 2. Age Compatibility <= 5
    if (abs(a.age - b.age) > 5) return false;
    
    // 3. Criminal Record (Must match)
    if (a.record != b.record) return false;
    
    // 4. Origin (Must match)
    if (a.origin != b.origin) return false;
    
    // 5. Weapon (At least one must be proficient)
    if (a.weapon == 0 && b.weapon == 0) return false;
    
    // 6. Trust Level >= 10
    if (a.trust + b.trust < 10) return false;
    
    // 7. Language (Bitwise AND must be > 0)
    if ((a.lang & b.lang) == 0) return false;
    
    // If it survives all checks, they are a match!
    return true; 
}


int main(){
    int t;
    cin>>t;
    int tc = 1;
    while(t--){
        
    int N, M;
    cin>>M>>N;

    int s = M + N + 2;
    vector<vector<int>>adj(s);
    vector<vector<ll>>res_cap(s, vector<ll>(s, 0));


    vector<person>gang(M + 1);
    vector<person>partner(N + 1);

    for(int i = 1; i <= M; i++){
        int a, b, c, d, e, f, g;
        cin>>a>>b>>c>>d>>e>>f>>g;
        gang[i] = (person{a, b, c, d, e, f, g});
    }
    for(int i = 1; i <= N; i++){
        int a, b, c, d, e, f, g;
        cin>>a>>b>>c>>d>>e>>f>>g;
        partner[i] = (person{a, b, c, d, e, f, g});
    }
    int src = 0;
    int sink = M + N + 1;

    for(int i = 1; i <= M; i++){
        
        if(res_cap[src][i] == 0){
            adj[src].push_back(i);
            adj[i].push_back(src);
            res_cap[src][i] = 1;
            res_cap[i][src] = 0;
        }
    }
    for(int i = 1; i <= N; i++){
        if(res_cap[sink][i + M] == 0){
            adj[sink].push_back(i + M);
            adj[i + M].push_back(sink);
            res_cap[sink][i + M] = 0;
            res_cap[i + M][sink] = 1;
        }
    }

    for(int i = 1; i <=M; i++){
        for(int j = 1; j <= N; j++){
            if(is_compatible(gang[i], partner[j])){
                adj[i].push_back(j + M);
                adj[j + M].push_back(i);
                res_cap[i][j + M] = 1;
                res_cap[j + M][i] = 0;
            }
        }
    }

    ll max_flow = edmonds_karp_algorithm(s, src, sink, res_cap, adj);
    cout << "Case " << tc++ << ": " << max_flow << '\n';
    
    }
    return 0;
}