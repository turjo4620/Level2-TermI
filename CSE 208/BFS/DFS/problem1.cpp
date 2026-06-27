#include <bits/stdc++.h>

using namespace std;


bool isConnected(const string &s1, const string &s2){
    int count = 0;
    for(int i = 0; i < s1.length(); i++){
        if(s1[i] != s2[i]){
            count++;
        }
    }
    return count == 1;
}

class Graph{
public:
    int V; 
    vector<vector<int>>adj;

    Graph(int V){
        this->V = V;
        adj.resize(V);
    }

    void addEdge(vector<string>&wordlist){
        int n = wordlist.size();
        for(int i = 0; i < n; i++){
            for(int j = i + 1; j < n; j++){
                if(isConnected(wordlist[i], wordlist[j])){
                    adj[i].push_back(j);
                    adj[j].push_back(i);
                }
            }
        }
    }

    int bfs(string src, string target, unordered_map<string, int>&wordToIndex){
        int s = wordToIndex[src];
        queue<int>q;
        vector<bool>visited(V, false);
        vector<int>distance(V, -1);

        if (src == target) return 0;

        q.push(s);
        visited[s] = true;
        distance[s] = 0;

        while(q.size() > 0){
            int u = q.front();
            q.pop();
            for(int neigh : adj[u]){
                if(!visited[neigh]){
                    visited[neigh] = true;
                    q.push(neigh);
                    distance[neigh] = 1 + distance[u];
                    if(neigh == wordToIndex[target]){
                        return distance[neigh];
                    }
                }
            }
        }
        return -1;
    }

};




int main(){
    
    int n;
    cin>>n;
    vector<string>wordlist(n);
    unordered_map<string, int>wordToIndex;

    Graph graph(n);

    for(int i = 0; i < n; i++){
        cin>>wordlist[i];
        wordToIndex[wordlist[i]] = i;
    }
    graph.addEdge(wordlist);

    string src, target;
    cin>>src>>target;

    cout<<graph.bfs(src, target, wordToIndex);

    

    return 0;
}