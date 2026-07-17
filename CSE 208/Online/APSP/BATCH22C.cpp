/*
🧪 Problem: Too Few DSA Labs!
In the future, the ECE department has N DSA labs, connected by M bidirectional roads. Each road has a walking cost (effort to move between labs).

Each lab can hold only a certain number of students (it has a capacity).

You need to answer Q queries. Each query contains the starting lab from where all students enter the lab network.

K students will enter the network one by one (student 1 enters first, followed by student 2, and so on).

For each student, determine the best lab they should choose as their destination, such that their total walking cost is minimized. Once a student chooses a lab, the lab's capacity is reduced by 1.

Due to renovation work, a few labs are under maintenance. Students can't choose the labs under maintenance as their destination or pass through them to reach other labs.

Note:

A student can't choose a lab as destination if it's already full (capacity reached) or under maintenance.
However, students can still pass through a lab (even if it is full) to reach other labs. But can't pass through a lab that is under maintenance.
If it is impossible for a student to choose a lab as destination, the student gets a cost of -1.
⚠️ Special Note:
For this assignment, you are not allowed to use Single Source Shortest Path (SSSP) algorithms like Dijkstra or Bellman-Ford. Instead, you must implement a solution using All Pairs Shortest Path (APSP) approaches such as the Floyd-Warshall algorithm or equivalent logic. This ensures fair grading and emphasizes the intended learning outcome.
📥 Input Format:
The first line contains three space-separated integers: N, M, and K.

The second line contains N space-separated integers ci (capacity of lab i). If a lab is under maintenance, its capacity is -1.

The next M lines contain three space-separated integers: ui, vi, and wi. This means there is a road from lab ui to lab vi with a cost of wi.

The next line contains an integer Q, the number of queries.

The last line contains Q space-separated integers ui (the starting lab for query i). Note that labs under maintenance are not included as starting labs.

N M K
c1 c2 c3 ... cN
u1 v1 w1
u2 v2 w2
...
uM vM wM
Q
u1 u2 u3 ... uQ 
📤 Output Format:
Print Q lines. Each line should contain K space-separated integers c1, c2, ... , cK, where ci is the cost of the ith student.

If a student can't find a lab with remaining space, print -1 for that student.

📦 Sample Input 1: All labs are operational
5 6 5
1 2 1 1 2
1 2 2
4 5 1
1 4 3
1 3 1
3 4 1
2 4 2
2
1 2
✅ Sample Output 1:
0 1 2 2 2
0 0 2 2 3
🧠 Explanation 1:
In the first query, all students start at Lab 1. First student can stay in Lab 1 (Cost 0). Second student can go to Lab 3 (Cost 1, Path: 1 → 3). Third student can go to Lab 2 (Cost 2, Path: 1 → 2). Fourth student can also go to Lab 2 (Cost 2, Path: 1 → 2). Fifth student can go to Lab 4 (Cost 2, Path: 1 → 3 → 4).

In the second query, all students start at Lab 2. First student can stay in Lab 2 (Cost 0). Second student can also stay in Lab 2 (Cost 0). Third student can go to Lab 1 (Cost 2, Path: 2 → 1). Fourth student can go to Lab 4 (Cost 2, Path: 2 → 4). Fifth student can also go to Lab 5 (Cost 3, Path: 2 → 4 → 5).

📦 Sample Input 2: Some students didn't find a lab
5 6 5
1 0 1 1 1
1 2 2
4 5 1
1 4 3
1 3 1
3 4 1
2 4 2
2
1 2
✅ Sample Output 2:
0 1 2 3 -1
2 2 3 3 -1
📦 Sample Input 3: Some labs are under maintenance
5 6 5
1 1 -1 1 1
1 2 2
4 5 1
1 4 3
1 3 1
3 4 1
2 4 2
2
1 2
✅ Sample Output 3:
0 2 3 4 -1
0 2 2 3 -1
🧠 Explanation 3:
In the first query, all students start at Lab 1. First student can stay in Lab 1 (Cost 0). Unlike the previous cases, the second student can't go to Lab 3 because it is under maintenance. So, the second student can go to Lab 2 (Cost 2, Path: 1 → 2). Third student can go to Lab 4 (Cost 3, Path: 1 → 4). Fourth student can go to Lab 5 (Cost 4, Path: 1 → 4 → 5). No lab available for fifth student (Cost -1).

🏷️ Tentative Marks Distribution:
 60%: All labs are operational.
 10%: Some students didn't find a lab.
 30%: Some labs are under maintenance.
*/

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
    vector<vector<ll>>dist;

    Graph(int V){
        this->V = V;
        dist.assign(V, vector<ll>(V, INF));
        for(int i = 0; i < V; i++) dist[i][i] = 0;
    }

    void addEdge(int u, int v, ll wt, vector<int>&capacity){
        if(capacity[u] == -1 || capacity[v] == -1) return;
        edges.push_back(Edge(u, v,wt));
        dist[u][v] = min(dist[u][v], wt);
        dist[v][u] = min(dist[v][u], wt);
    }


    void runFloydWarshell(const vector<int>&capacity){
        for(int k = 0; k < V; k++){
            if(capacity[k] == -1) continue;
            for(int i = 0; i < V; i++){
                for(int j = 0; j < V; j++){
                    if(dist[i][k] != INF && dist[k][j] != INF){
                        dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                    }
                }
            }
        }
    }

    void processQuery(int K, const vector<int>&capacity, int start){
        vector<int>temp_c = capacity;

        for(int s = 0; s < K; s++){
            int best_lab = -1;
            ll min_dist = INF;

            for(int i = 0; i < V; i++){
                if(temp_c[i] <= 0)continue;
                if(dist[start][i] < min_dist){
                    min_dist = dist[start][i];
                    best_lab = i;
                }
            }

            if(best_lab != -1){
                cout<<min_dist<<" ";
                temp_c[best_lab]--;
            }
            else{
                cout<<-1<<" ";
            }
        }
        cout<<endl;
    }
};





int main(){
    int N, M, K;
    cin>>N>>M>>K;

    vector<int>capacity(N);
    for(int i = 0; i < N; i++){
        cin>>capacity[i];
    }

    Graph graph(N);

    for(int i = 0; i < M; i++){
        int a, b;
        ll w;
        cin>>a>>b>>w;
        a--; b--;
        if (capacity[a] != -1 && capacity[b] != -1) {
                 graph.addEdge(a, b, w, capacity);
            }
    }

    graph.runFloydWarshell(capacity);
    int Q;
    cin>>Q;

    while(Q--){
        int start; cin>>start;
        start--;
        graph.processQuery(K, capacity, start);
    }

    

    return 0;
}