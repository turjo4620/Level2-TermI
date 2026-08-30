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

struct Citizen{
    string name;
    string nid;
    string phone;
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    hashtable<string, Citizen> by_name(2003);
    hashtable<string, Citizen> by_nid(2003);
    hashtable<string, Citizen> by_phone(2003);

    int choice;

    while(true){
        cout << "Enter choice: 1. Enter Data, 2. Search Index\n";

        if(!(cin >> choice)) break;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if(choice == 1){
            Citizen citizen;

            cout << "Enter name: ";
            getline(cin, citizen.name);

            cout << "Enter NID no: ";
            getline(cin, citizen.nid);

            cout << "Enter phone no: ";
            getline(cin, citizen.phone);

            by_name.insert(citizen.name, citizen);
            by_nid.insert(citizen.nid, citizen);
            by_phone.insert(citizen.phone, citizen);
        }
        else if(choice == 2){
            string query;

            cout << "Enter search string: ";
            getline(cin, query);

            Citizen* result = by_name.find(query);
            if(result == nullptr) result = by_nid.find(query);
            if(result == nullptr) result = by_phone.find(query);

            if(result != nullptr){
                cout << "Match Found:\n";
                cout << "Name: " << result->name << '\n';
                cout << "NID no: " << result->nid << '\n';
                cout << "Phone no: " << result->phone << '\n';
            }
            else{
                cout << "No match found\n";
            }
        }
    }

    return 0;
}
