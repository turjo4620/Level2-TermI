#include <bits/stdc++.h>

using namespace std;
using ll = long long;

// struct edge{
//     int u, v;
//     ll wt;
// };

bool bfs_helper(vector<vector<int>> &adj, int src, int sink, vector<vector<ll>> &res_capacity, vector<int> &parent)
{
    fill(parent.begin(), parent.end(), -1);
    queue<int> q;
    q.push(src);

    vector<bool> visited(adj.size());

    visited[src] = true;

    while (!q.empty())
    {
        int u = q.front();
        q.pop();

        // storing u

        for (int v : adj[u])
        {
            if (!visited[v] && res_capacity[u][v] > 0)
            {
                visited[v] = true;
                parent[v] = u;
                q.push(v);
            }
        }
    }
    return visited[sink];
}

ll edmonds_karp_algorithm(int V, int src, int sink, vector<vector<ll>> &res_capacity, vector<vector<int>> &adj)
{

    if (src == sink)
        return 0;

    ll total_flow = 0;

    vector<int> parent(V, -1);

    while (bfs_helper(adj, src, sink, res_capacity, parent))
    {
        ll min_res_capacity = LLONG_MAX;
        int curr = sink;

        while (curr != src)
        {
            int par = parent[curr];
            min_res_capacity = min(min_res_capacity, res_capacity[par][curr]);
            curr = par;
        }

        curr = sink;
        while (curr != src)
        {
            int p = parent[curr];
            res_capacity[p][curr] -= min_res_capacity;
            res_capacity[curr][p] += min_res_capacity;
            curr = p;
        }

        total_flow += min_res_capacity;
    }

    return total_flow;
}

int main()
{

    int N, M, K;
    cin >> N >> M >> K;

    int s;
    s = N + M + 2;

    int src = 0;
    int sink = s - 1;

    vector<vector<int>> adj(s);
    vector<vector<ll>> res_cap(s, vector<ll>(s, 0));

    for (int i = 0; i < K; i++)
    {
        int a, b;
        cin >> a >> b;
        // a--; b--;
        int u = a;
        int v = N + b;
        if (res_cap[u][v] == 0 && res_cap[v][u] == 0)
        {
            adj[u].push_back(v);
            adj[v].push_back(u);
        }
        // ll cap;
        // cin>>cap;
        res_cap[u][v] += 1;
    }

    for (int i = 1; i <= N; i++)
    {
        if (res_cap[src][i] == 0 && res_cap[i][src] == 0)
        {
            adj[src].push_back(i);
            adj[i].push_back(src);
        }
        res_cap[src][i] += 1;
    }
    for (int i = 1; i <= M; i++)
    {
        int girl = N + i;
        if (res_cap[girl][sink] == 0 && res_cap[sink][girl] == 0)
        {
            adj[girl].push_back(sink);
            adj[sink].push_back(girl);
        }
        res_cap[girl][sink] += 1;
    }

    ll max_flow = edmonds_karp_algorithm(s, src, sink, res_cap, adj);
    cout << max_flow << endl;


    for(int i = 1; i <= N; i++){
        for(int j = 1; j <= M;j ++){
            int boy = i;
            int girl = N + j;
            if(res_cap[girl][boy] > 0){
                cout<<i<<" "<<j<<endl;
            }
        }
    }

    return 0;
}