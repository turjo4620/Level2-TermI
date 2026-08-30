#include <bits/stdc++.h>
using namespace std;

template <typename K, typename V>
struct hashdata{
    K key;
    V value;
    bool is_empty = true;
    bool is_delete = false;
};

template <typename K, typename V>
class hashtable{
public:
    int current_size;
    int num_elements;
    list<hashdata<K,V>>* table;

    hashtable(int size = 2003){
        current_size = size;
        num_elements = 0;
        table = new list<hashdata<K,V>>[current_size];
    }

    string convertToString(const K& k){
        ostringstream oss;
        oss << k;
        return oss.str();
    }

    int hash1(const K& k){
        long long hash_value = 0;
        string s = convertToString(k);
        for(char c : s) hash_value = (hash_value * 31 + c) % current_size;
        return (int)hash_value;
    }

    bool insert(const K& key, const V& value){
        int index = hash1(key);
        for(auto& item : table[index]){
            if(item.key == key) return false;
        }
        hashdata<K,V> item;
        item.key = key;
        item.value = value;
        item.is_empty = false;
        item.is_delete = false;
        table[index].push_back(item);
        num_elements++;
        return true;
    }

    V* find(const K& key){
        int index = hash1(key);
        for(auto& item : table[index]){
            if(item.key == key) return &item.value;
        }
        return nullptr;
    }

    bool contains(const K& key){
        return find(key) != nullptr;
    }

    ~hashtable(){
        delete[] table;
    }
};

char complement(char c){
    if(c == 'A') return 'T';
    if(c == 'T') return 'A';
    if(c == 'C') return 'G';
    return 'C';
}

string reverse_complement(const string& s){
    string result;
    result.reserve(s.size());
    for(int i = (int)s.size() - 1; i >= 0; i--){
        result += complement(s[i]);
    }
    return result;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, l;
    cin >> n >> l;

    // key = DNA sequence, value = all previous occurrences of that sequence
    hashtable<string, vector<string>> index(2003);

    for(int i = 0; i < n; i++){
        string s;
        cin >> s;

        string needed = reverse_complement(s);
        vector<string>* matches = index.find(needed);

        if(matches != nullptr){
            for(const string& previous : *matches){
                cout << previous << ' ' << s << '\n';
            }
        }

        vector<string>* same_sequence = index.find(s);
        if(same_sequence == nullptr){
            index.insert(s, vector<string>{s});
        }
        else{
            same_sequence->push_back(s);
        }
    }

    return 0;
}
