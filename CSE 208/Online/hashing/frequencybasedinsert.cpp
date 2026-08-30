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

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int l, k;
    cin >> l >> k;

    string dna;
    cin >> dna;

    hashtable<string, int> frequency(2003);
    vector<string> order;

    for(int i = 0; i + k <= l; i++){
        string kmer = dna.substr(i, k);   // O(k)

        int* count = frequency.find(kmer);
        if(count == nullptr){
            frequency.insert(kmer, 1);
            order.push_back(kmer);
        }
        else{
            (*count)++;
        }
    }

    for(const string& kmer : order){
        cout << kmer << ' ' << *frequency.find(kmer) << '\n';
    }

    return 0;
}
