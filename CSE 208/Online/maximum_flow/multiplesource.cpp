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

    int N, M, K, L;
    // N = nodes, M = edges, K = number of sources, L = number of sinks
    cin >> N >> M >> K >> L;

    // 1. Create the Super Source and Super Sink
    int super_source = N;
    int super_sink = N + 1;
    int total_nodes = N + 2;

    vector<vector<int>> adj(total_nodes);
    vector<vector<ll>> res_cap(total_nodes, vector<ll>(total_nodes, 0));

    ll INF = 1e18; // Representing Infinity

    // 2. Read the K sources and connect them to Super Source
    for (int i = 0; i < K; i++)
    {
        int s;
        cin >> s;

        adj[super_source].push_back(s);
        adj[s].push_back(super_source);

        res_cap[super_source][s] = INF; // Infinite supply from the Super Source
    }

    // 3. Read the L sinks and connect them to Super Sink
    for (int i = 0; i < L; i++)
    {
        int t;
        cin >> t;

        adj[t].push_back(super_sink);
        adj[super_sink].push_back(t);

        res_cap[t][super_sink] = INF; // Infinite demand at the Super Sink
    }

    // 4. Read the M normal edges just like you usually do
    for (int i = 0; i < M; i++)
    {
        int u, v;
        ll cap;
        cin >> u >> v >> cap;

        if (res_cap[u][v] == 0 && res_cap[v][u] == 0)
        {
            adj[u].push_back(v);
            adj[v].push_back(u);
        }
        res_cap[u][v] += cap;
    }

    // 5. Run Edmonds-Karp from Super Source to Super Sink!
    ll max_flow = edmonds_karp_algorithm(total_nodes, super_source, super_sink, res_cap, adj);

    cout << max_flow << endl;

    return 0;
}