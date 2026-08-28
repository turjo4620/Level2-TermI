#include <bits/stdc++.h>
using namespace std;

const int EMPTY = 0;
const int OCCUPIED = 1;
const int DELETED = 2;

struct Data {
    int key;
    string value;
    int status;
};

class HashTable {
public:
    int table_size;
    int num_elements;

    Data* table;

    // Constructor
    HashTable(int size) {
        table_size = size;
        num_elements = 0;

        table = new Data[table_size];

        for (int i = 0; i < table_size; i++) {
            table[i].status = EMPTY;
        }
    }

    // Hash function
    int hash_function(int key) {
        return key % table_size;
    }

    // Insert
    bool insert(int key, string value) {

        int index = hash_function(key);

        int first_deleted = -1;

        for (int i = 0; i < table_size; i++) {

            int current = (index + i) % table_size;

            // Empty slot
            if (table[current].status == EMPTY) {

                // Prefer previously found deleted slot
                if (first_deleted != -1) {
                    current = first_deleted;
                }

                table[current].key = key;
                table[current].value = value;
                table[current].status = OCCUPIED;

                num_elements++;

                return true;
            }

            // Deleted slot
            if (table[current].status == DELETED) {

                if (first_deleted == -1) {
                    first_deleted = current;
                }

                continue;
            }

            // Occupied slot
            if (table[current].key == key) {
                return false;
            }
        }

        // Table completely full
        if (first_deleted != -1) {

            table[first_deleted].key = key;
            table[first_deleted].value = value;
            table[first_deleted].status = OCCUPIED;

            num_elements++;

            return true;
        }

        return false;
    }

    // Search
    bool search(int key) {

        int index = hash_function(key);

        for (int i = 0; i < table_size; i++) {

            int current = (index + i) % table_size;

            // If EMPTY, key cannot exist further
            if (table[current].status == EMPTY) {
                return false;
            }

            // Ignore DELETED
            if (table[current].status == DELETED) {
                continue;
            }

            // Check occupied slot
            if (table[current].key == key) {
                return true;
            }
        }

        return false;
    }

    // Delete
    bool remove(int key) {

        int index = hash_function(key);

        for (int i = 0; i < table_size; i++) {

            int current = (index + i) % table_size;

            // Stop at EMPTY
            if (table[current].status == EMPTY) {
                return false;
            }

            // Ignore DELETED
            if (table[current].status == DELETED) {
                continue;
            }

            // Found key
            if (table[current].key == key) {

                table[current].status = DELETED;

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

            if (table[i].status == EMPTY) {
                cout << "EMPTY";
            }
            else if (table[i].status == DELETED) {
                cout << "DELETED";
            }
            else {
                cout << "("
                     << table[i].key
                     << ", "
                     << table[i].value
                     << ")";
            }

            cout << endl;
        }
    }
};


int main() {

    HashTable h(10);

    h.insert(10, "Turjo");
    h.insert(20, "Prince");
    h.insert(30, "BUET");
    h.insert(15, "CSE");

    h.display();

    cout << endl;

    if (h.search(20))
        cout << "Found\n";
    else
        cout << "Not Found\n";

    h.remove(20);

    cout << endl;

    h.display();

    return 0;
}