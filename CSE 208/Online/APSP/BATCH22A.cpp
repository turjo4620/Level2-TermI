#include <bits/stdc++.h>

using namespace std;
using ll = long long;
const ll INF = 1e18;

class Edge{
public:
    int u, v;
    ll wt;

    Edge(int u, int v, ll wt){
        this->u = u;
        this->v = v;
        this->wt = wt;
    }
    bool operator <(const Edge &other) const{
        return this->wt < other.wt;
    }
};

class Graph{
public:
    int V;
    vector<Edge>edges;

    Graph(int V){
        this->V = V;
    }

    void addEdge(int u, int v, ll wt){
        edges.push_back(Edge(u, v,wt));
        edges.push_back(Edge(v, u, wt));
    }

    void solveTuitionPath(vector<int>&a, vector<ll>&h, int k, ll x){
        vector<vector<ll>>dist(V, vector<ll>(V, INF));
        vector<vector<ll>>next_node(V, vector<ll>(V, -1));
        
        for(int i = 0; i < V; i++){
            dist[i][i] = 0;
        }

        for(Edge e : edges){
            dist[e.u][e.v] = min(dist[e.u][e.v] , e.wt);
            next_node[e.u][e.v] = e.v;
        }

        for(int k = 0; k < V; k++){
            for(int i = 0; i < V; i++){
                for(int j = 0; j < V; j++){
                    if(dist[i][k] != INF && dist[k][j] != INF){
                        dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                        next_node[i][j] = next_node[i][k]; 
                    }
                }
            }
        }

    ll best_city = -1;
    ll min_total_cost = INF;
    ll final_max_time = 0;

        for(int c = 0; c < V; c++){
            bool possible = false;
            ll current_total_cost = 0;
            ll current_max_time = 0;

            for(int i = 0; i < k; i++){
                int lord_city = a[i];
                ll timetaken = dist[lord_city][c];

                if(timetaken > x){
                    possible = false;
                    break;
                }
                current_total_cost += timetaken * h[i];
                current_max_time = max(timetaken, current_max_time);
            }
            
            if(possible && current_total_cost < min_total_cost){
                min_total_cost = current_total_cost;
                best_city = c; 
                final_max_time = current_max_time;
            }
        }

        if(best_city == -1){
            cout<<-1;
            return;
        }

        cout<<best_city<<min_total_cost<<final_max_time;

        for(int i = 0; i < k; i++){
            int curr = a[i];
            ll cost = dist[curr][best_city]  * h[i];

            if(curr == best_city){
                cout<<curr;
            }
            else{
                cout<<curr;
                while(curr != best_city){
                    curr = next_node[curr][best_city];
                    cout<<"->"<<curr;
                }
                cout<<cost;
            }
        }
    }
};




int main() {
    // ফাস্ট I/O এর জন্য
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m, k;
    ll x;
    
    // ফাইল এন্ড বা ইনপুট শেষ হওয়া চেক করার জন্য
    if (!(cin >> n >> m >> k >> x)) return 0;

    // লর্ডদের বর্তমান শহরের ইনপুট (0-indexed করা হচ্ছে)
    vector<int> a(k);
    for (int i = 0; i < k; i++) {
        cin >> a[i];
        a[i]--; // 1-based থেকে 0-based
    }

    // লর্ডদের আওয়ারলি রেট
    vector<ll> h(k);
    for (int i = 0; i < k; i++) {
        cin >> h[i];
    }

    
    Graph graph(n);

    for (int i = 0; i < m; i++) {
        int u, v;
        ll t;
        cin >> u >> v >> t;
        
        u--; 
        v--; 
        
        graph.addEdge(u, v, t);
    }

    graph.solveTuitionPath(a, h, k, x);

    return 0;
}