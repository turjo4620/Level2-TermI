/*

The city of Dhaka is suffering from severe traffic congestion. The city officials have modeled the road
network as a directed graph where intersections are nodes and roads are directed edges with a specific
vehicle capacity (vehicles/hour).
Currently, the traffic flow from the Uttara to Motijheel is strictly limited by congested roads. The officials
have a budget to construct exactly one new flyover. They have a list of P proposed flyover projects. Each
proposal connects two existing intersections with a new high-capacity road.
Task Requirements
Your task is to identify which of the proposed flyovers would strictly increase the maximum traffic flow if
constructed. If multiple projects work, list all of them.
Input Format
● The first line of input will have two space-separated non-negative integers N and M, the number
of intersections and existing roads.
● In the next M lines, there will be three space-separated integers, u, v, c denoting an existing
directed road from u to v with capacity c.
● The next line will contain an integer P, the number of proposed flyover projects.
● In the next P lines, there will be three space-separated integers u, v, c representing a potential new
flyover from u to v with capacity c.
● Assume the Source is always node 1 and the Sink is always node N.
Output Format
● Print a space-separated list of the indices (corresponding to the order in the input) of the proposed
flyovers that, if built, would strictly increase the total max flow.
● If no proposal improves the flow, print None.
*/

#include <bits/stdc++.h>

using namespace std;
using ll = long long;

// struct edge{
//     int u, v;
//     ll wt;
// };

bool bfs_helper(vector<vector<int>> &adj, int src, int sink, vector<vector<ll>> &res_capacity, vector<int> &parent)
{
    queue<int> q;
    q.push(src);

    parent[src] = -1;

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

    int N, M;
    cin >> N >> M;

    vector<vector<int>> adj(N);
    vector<vector<ll>> res_cap(N, vector<ll>(N, 0));

    for (int i = 0; i < M; i++)
    {
        int a, b;
        cin >> a >> b;
        a--;
        b--;
        adj[a].push_back(b);
        adj[b].push_back(a);
        ll cap;
        cin >> cap;
        res_cap[a][b] += cap;
    }

    int p;
    cin >> p;

    struct proposal
    {
        int u, v;
        ll cap;
        int idx;
    };

    vector<proposal> proposals(p);

    for (int i = 0; i < p; i++)
    {
        cin >> proposals[i].u >> proposals[i].v >> proposals[i].cap;
        proposals[i].u--;
        proposals[i].v--;
        proposals[i].idx = i + 1;
    }

    ll max_flow = edmonds_karp_algorithm(N, 0, N - 1, res_cap, adj);

    vector<bool> reachable_from_source(N, false);
    queue<int> q1;

    q1.push(0);

    reachable_from_source[0] = true;

    while (!q1.empty())
    {
        int u = q1.front();
        q1.pop();
        for (int v : adj[u])
        {
            if (!reachable_from_source[v] && res_cap[u][v] > 0)
            {
                reachable_from_source[v] = true;
                q1.push(v);
            }
        }
    }

    vector<bool>can_reach_from_sink(N, false);

    queue<int> q2;

    int sink = N - 1;
    q2.push(sink);
    can_reach_from_sink[sink] = true;

    while (!q2.empty())
    {
        int u = q2.front();
        q2.pop();
        for (int v : adj[u])
        {
            if (!can_reach_from_sink[v] && res_cap[u][v] > 0)
            {
                can_reach_from_sink[v] = true;
                q2.push(v);
            }
        }
    }

    vector<int>valid_index;

    for(auto&p : proposals){
        if(p.cap > 0 && reachable_from_source[p.u] && can_reach_from_sink[p.v]){
            valid_index.push_back(p.idx);
        }
    }

    if(valid_index.empty()){
        cout<<"NONE"<<endl;
    }
    else{
        for(int i : valid_index){
            cout<<i<<" ";
        }
    }

    return 0;
}