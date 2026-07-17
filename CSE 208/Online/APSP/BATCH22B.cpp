/*
🧪 Problem: The United Kingdom!
The Kingdom of Rohan is under threat, and the King has issued strategic relocation orders for his armies. There are N army stations connected by a network of M directed roads, each with a known travel cost.

There are N armies, each currently stationed at one unique station. The King has assigned each to a target station (a permutation of all stations). The goal is to carry out these relocations using minimum total travel cost, allowing armies to move through intermediate stations to reach their targets optimally.

Note: It's not guarranteed that all armies can reach their target stations. You need to minimize the total travel cost of the armies that can reach their target stations.

King can only relocate (From start station to target station) armies in groups of K at a time (e.g., due to limited transport). After relocating each batch of K armies, the road network deteriorates and the minimum travel cost between any two stations doubles. That means, if minimum travel cost from A to B is X, then after one batch of K armies, the travel cost from A to B will be 2X. After second batch, the travel cost from A to B will be 4X. And so on. Thus order of relocation matters. You need to think about the optimal order.

Note: King will relocate armies based on the initial optimal paths. He won't calculate the optimal paths after each batch. So, finding the initial optimal paths is enough. Keep it simple.

⚠️ Special Note:
For this assignment, you are not allowed to use Single Source Shortest Path (SSSP) algorithms like Dijkstra or Bellman-Ford. Instead, you must implement a solution using All Pairs Shortest Path (APSP) approaches such as the Floyd-Warshall algorithm or equivalent logic. This ensures fair grading and emphasizes the intended learning outcome.
📥 Input Format:
The first line contains three space-separated integers: N, M and K.

The second line contains N space-separated unique integers si (Current station of army i).

The second line contains N space-separated unique integers ti (Target station of army i).

The next M lines contain three space-separated integers: ui, vi, and wi. This means there is a directed road from station ui to station vi with a cost of wi.

N M K
s1 s2 s3 ... sN
t1 t2 t3 ... tN
u1 v1 w1
u2 v2 w2
...
uM vM wM
📤 Output Format:
Print a single integer — the total minimum cost for relocating all possible armies to their target stations.

📦 Sample Input 1:
6 10 3
4 5 6 1 2 3
3 6 1 2 4 5
1 2 4
2 3 5
3 4 2
4 5 1
5 6 7
6 1 3
1 3 5
2 4 6
3 5 2
4 6 1
✅ Sample Output 1:
40
🧠 Explanation 1:
Optimal path for each army is:

1st army: 4 → 6 → 1 → 3. Cost: 3 + 1 + 5 = 9.
2nd army: 5 → 6. Cost: 7.
3rd army: 6 → 1. Cost: 3.
4th army: 1 → 2. Cost: 4.
5th army: 2 → 4. Cost: 6.
6th army: 3 → 4. Cost: 2.
The armies can be relocated in the following order:
1st batch: 1st, 2nd and 5th armies. Cost: 9, 7 and 6.
2nd batch: 3rd, 4th and 6th armies. Cost: 2*3=6, 2*4=8 and 2*2=4. Each army will pay double because this is the 2nd batch.
📦 Sample Input 2:
6 5 2
4 5 6 1 2 3
3 6 1 2 4 5
1 2 4
3 4 2
4 5 1
5 6 7
6 1 3
✅ Sample Output 2:
23
🧠 Explanation 2:
1st and 5th armies cannot reach their target stations. Total minimum travel cost for the remaining armies is 23.
🏷️ Tentative Marks Distribution:
 10%: Correctly reading the input and constructing the graph.
 30%: Finding all pairs shortest path.
 40%: Finding the minimum cost for all armies to reach their target station.
 20%: Handling the case where some targets are not reachable.
🧪 Test Case Checker:
For evaluation purposes only. May ignore this section.
To check your solution, you can use the provided test case checker evaluator.py. It will help you verify if your solution is correct and meets the requirements.

Steps:
Your C++ file should be in the same directory as evaluator.py and test cases.
Run the following command in that directory:
python evaluator.py <cpp-file>
Make sure to replace <cpp-file> with the actual name of your C++ file.

Note: Your program shouldn't read input from file or write output to file. evaluator.py will handle that for you.

If your code is correct, the output will be:

Test case 1 passed!
Test case 2 passed!
Test case 3 passed!
Test case 4 passed!
Test case 5 passed!
Test case 6 passed!
Test case 7 passed!
Test case 8 passed!
Test case 9 passed!
Test case 10 passed!
Total test cases passed: 10/10
If your code is incorrect, the output will be like:

Test case 1 passed!
Test case 2 passed!
Test case 3 passed!
> Line 1 mismatch:
> Expected: 4
> Got: 7
Test case 4 failed!
Test case 5 passed!
Test case 6 passed!
Test case 7 passed!
Test case 8 passed!
Test case 9 passed!
Test case 10 passed!
Total test cases passed: 9/10
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

    Graph(int V){
        this->V = V;
    }

    void addEdge(int u, int v, ll wt){
        edges.push_back(Edge(u, v,wt));
    }

    void floydWarshall(int K, vector<int> &s, vector<int> &t){
        vector<vector<ll>>dist(V, vector<ll>(V, INF));
        
        for(int i = 0; i < V; i++){
            dist[i][i] = 0;
        }

        for(Edge e : edges){
            dist[e.u][e.v] = min(dist[e.u][e.v] , e.wt);
        }

        for(int k = 0; k < V; k++){
            for(int i = 0; i < V; i++){
                for(int j = 0; j < V; j++){
                    if(dist[i][k] != INF && dist[k][j] != INF){
                        dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                    }
                }
            }
        }

        vector<ll>shortest_distances(V);
        for(int i = 0; i < V; i++){
            ll cost = dist[s[i]][t[i]];
            if(cost != INF){
                shortest_distances.push_back(cost);
            }
        }

        sort(shortest_distances.begin(), shortest_distances.end(), greater<ll>());


        ll final_cost = 0;
        int j = 0;

        for(int i = 0; j < shortest_distances.size(); i++){
            ll cost_multi = pow(2, i);
            int count = 0;

            while(count < K && j < shortest_distances.size()){
                final_cost += cost_multi * shortest_distances[j];
                count++;
                j++;
            }
        }


        // ll final_cost = 0;
        // int j = 0;
        // for(int i = 0; j < shortest_distances.size(); i++){
        //     ll cost_multi = pow(2, i);
        //     int count = 0;
        //     while (count < K && j < shortest_distances.size())
        //     {
        //         final_cost += cost_multi * shortest_distances[j];
        //         count++;
        //         j++;
        //     }
            
        // }
        cout<<final_cost<<endl;
    }
};




int main() {
    int n, m, k;
    // ইনপুট না পেলে বন্ধ হবে
    if (!(cin >> n >> m >> k)) return 0;
    
    // আর্মির সোর্স (0-indexed)
    vector<int> s(n);
    for (int i = 0; i < n; i++) {
        cin >> s[i];
        s[i]--;
    }

    // আর্মির টার্গেট (0-indexed)
    vector<int> t(n);
    for (int i = 0; i < n; i++) {
        cin >> t[i];
        t[i]--;
    }
    
    Graph graph(n);
    
    // রাস্তাগুলোর ইনপুট (0-indexed)
    for (int i = 0; i < m; i++) {
        int u, v;
        ll w;
        cin >> u >> v >> w;
        u--; v--;
        graph.addEdge(u, v, w);
    }

    graph.floydWarshall(k, s, t);

    return 0;
}

