#include <bits/stdc++.h>

using namespace std;
using ll = long long;

class Edge
{
public:
    int u, v;
    ll wt;
    int id;

    Edge(int u, int v, ll wt, int id)
    {
        this->u = u;
        this->v = v;
        this->wt = wt;
        this->id = id;
    }

    bool operator<(const Edge &other) const
    {
        return this->wt < other.wt;
    }
};

class Graph
{
public:
    int V;
    vector<Edge> edges;
    vector<int> par, rnk;

    Graph(int V)
    {
        this->V = V;

        for (int i = 0; i < V; i++)
        {
            par.push_back(i);
            rnk.push_back(0);
        }
    }
    void addEdge(int u, int v, ll wt, int id)
    {
        edges.push_back(Edge(u, v, wt, id));
    }

    // find function

    int find(int x)
    {
        if (par[x] == x)
        {
            return x;
        }
        // path compression
        return par[x] = find(par[x]);
    }

    // union by rank

    bool unionByRank(int u, int v)
    {
        int parA = find(u);
        int parB = find(v);

        if (parA == parB)
            return false;

        if (rnk[parA] == rnk[parB])
        {
            par[parB] = parA;
            rnk[parA]++;
        }
        else if (rnk[parA] > rnk[parB])
        {
            par[parB] = parA;
        }
        else
        {
            par[parA] = parB;
        }

        return true;
    }
    void krsukal(int m)
    {
        sort(edges.begin(), edges.end()); // O(ElogE)

        vector<string> ans(m);
        int n_edges = edges.size();

        // for(int i = 0; i < n_edges;){
        //     int j = i;

        //     while(j < n_edges && edges[j].wt == edges[i].wt){
        //         j++;
        //     }

        //      for(int k = i; k < j; k++){
        //         if(find(edges[k].u) != find(edges[k].v)){
        //             ans[edges[k].id] = "YES";
        //         }
        //         else{
        //             ans[edges[k].id] = "NO";
        //         }
        //      }

        //      for(int k = i; k < j; k++){
        //         unionByRank(edges[k].u, edges[k].v) ;
        //      }
        // }

        for (int i = 0; i < n_edges;)
        {
            int j = i;
            while (j < n_edges && edges[i].wt == edges[j].wt)
            {
                j++;
            }

            for (int k = i; k < j; i++)
            {
                if (find(edges[k].u) != find(edges[k].v))
                {
                    ans[edges[k].id] = "YES";
                }
                else
                {
                    ans[edges[k].id] = "NO";
                }
            }

            for(int k = i; k < j; k++){
                unionByRank(edges[k].u, edges[k].v);
            }
        }
    }
};

int main()
{

    int n;
    cin >> n;
    Graph graph(n);

    int m;
    cin >> m;
    for (int i = 0; i < m; i++)
    {
        int p, q;
        ll r;
        cin >> p >> q >> r;
        p--;
        q--;
        graph.addEdge(p, q, r, i);
    }

    graph.krsukal(m);

    return 0;
}