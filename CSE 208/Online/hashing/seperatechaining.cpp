#include <bits/stdc++.h>
using namespace std;

struct Data {
    int key;
    string value;
};

class HashTable {
public:
    int table_size;
    int num_elements;

    list<Data>* table;

    // Constructor
    HashTable(int size) {
        table_size = size;
        num_elements = 0;

        table = new list<Data>[table_size];
    }

    // Hash function
    int hash_function(int key) {
        return key % table_size;
    }

    // Insert
    bool insert(int key, string value) {

        int index = hash_function(key);

        // Check duplicate
        for (auto item : table[index]) {
            if (item.key == key) {
                return false;
            }
        }

        // Insert at the end of the chain
        table[index].push_back({key, value});

        num_elements++;

        return true;
    }

    // Search
    bool search(int key) {

        int index = hash_function(key);

        for (auto item : table[index]) {

            if (item.key == key) {
                return true;
            }
        }

        return false;
    }

    // Delete
    bool remove(int key) {

        int index = hash_function(key);

        for (auto it = table[index].begin();
             it != table[index].end();
             ++it) {

            if (it->key == key) {

                table[index].erase(it);

                num_elements--;

                return true;
            }
        }

        return false;
    }

    // Display
    void display() {

        for (int i = 0; i < table_size; i++) {

            cout << i << " : ";

            for (auto item : table[i]) {
                cout << "(" << item.key
                     << ", " << item.value << ") ";
            }

            cout << endl;
        }
    }
};


int main() {

    HashTable h(10);

    h.insert(10, "Turjo");
    h.insert(20, "Prince");
    h.insert(15, "BUET");
    h.insert(7, "CSE");

    h.display();

    cout << endl;

    if (h.search(20))
        cout << "Found\n";
    else
        cout << "Not Found\n";

    h.remove(20);

    h.display();

    return 0;
}