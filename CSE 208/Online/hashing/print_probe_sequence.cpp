#include <bits/stdc++.h>
using namespace std;

const int EMPTY = 0;
const int OCCUPIED = 1;
const int DELETED = 2;

struct Data
{
    string key;
    int value;
    int status;
};

class HashTable
{
public:
    int table_size;
    int num_elements;

    Data *table;

    HashTable(int size)
    {
        table_size = size;
        num_elements = 0;

        table = new Data[table_size];

        for (int i = 0; i < table_size; i++)
        {
            table[i].status = EMPTY;
        }
    }

    int hash_function(string key)
    {
        unsigned long long hash = 0;

        for (char c : key)
        {
            hash = (hash * 31 + c) % table_size;
        }

        return hash;
    }

    bool insert(string key, int value)
    {
        int index = hash_function(key);
        int first_deleted = -1;

        for (int i = 0; i < table_size; i++)
        {
            int current = (index + i) % table_size;

            if (table[current].status == EMPTY)
            {
                if (first_deleted != -1)
                {
                    current = first_deleted;
                }

                table[current].key = key;
                table[current].value = value;
                table[current].status = OCCUPIED;

                num_elements++;

                return true;
            }

            if (table[current].status == DELETED)
            {
                if (first_deleted == -1)
                {
                    first_deleted = current;
                }

                continue;
            }

            if (table[current].key == key)
            {
                return false;
            }
        }

        if (first_deleted != -1)
        {
            table[first_deleted].key = key;
            table[first_deleted].value = value;
            table[first_deleted].status = OCCUPIED;

            num_elements++;

            return true;
        }

        return false;
    }

    bool search(string key)
    {
        int index = hash_function(key);

        for (int i = 0; i < table_size; i++)
        {
            int current = (index + i) % table_size;

            if (table[current].status == EMPTY)
            {
                return false;
            }

            if (table[current].status == DELETED)
            {
                continue;
            }

            if (table[current].key == key)
            {
                return true;
            }
        }

        return false;
    }

    bool remove(string key)
    {
        int index = hash_function(key);

        for (int i = 0; i < table_size; i++)
        {
            int current = (index + i) % table_size;

            if (table[current].status == EMPTY)
            {
                return false;
            }

            if (table[current].status == DELETED)
            {
                continue;
            }

            if (table[current].key == key)
            {
                table[current].status = DELETED;
                num_elements--;

                return true;
            }
        }

        return false;
    }

    bool update(string key, int new_value)
    {
        int index = hash_function(key);

        for (int i = 0; i < table_size; i++)
        {
            int current = (index + i) % table_size;

            if (table[current].status == EMPTY)
            {
                return false;
            }

            if (table[current].status == DELETED)
            {
                continue;
            }

            if (table[current].key == key)
            {
                table[current].value = new_value;
                return true;
            }
        }

        return false;
    }

    bool printProbeSequence(string key)
    {
        int index = hash_function(key);

        bool first = true;

        for (int i = 0; i < table_size; i++)
        {
            int current = (index + i) % table_size;

            if (!first)
            {
                cout << " -> ";
            }

            cout << current;
            first = false;

            if (table[current].status == EMPTY)
            {
                cout << endl;
                return false;
            }

            if (table[current].status == DELETED)
            {
                continue;
            }

            if (table[current].key == key)
            {
                cout << endl;
                return true;
            }
        }

        cout << endl;
        return false;
    }
};

string random_word_generator(int length)
{
    string w = "";

    for (int i = 0; i < length; i++)
    {
        char c = 'a' + (rand() % 26);
        w += c;
    }

    return w;
}

int main()
{
    const int TABLE_SIZE = 20011;

    HashTable h(TABLE_SIZE);

    srand(time(0));

    set<string> words;
    vector<pair<string, int>> data_set;

    int count = 1;
    int word_length;

    cin >> word_length;

    while (words.size() < 10000)
    {
        string w = random_word_generator(word_length);

        if (words.find(w) == words.end())
        {
            words.insert(w);
            data_set.push_back({w, count});
            count++;
        }
    }

    for (auto item : data_set)
    {
        h.insert(item.first, item.second);
    }

    int n;
    cin >> n;

    while (n--)
    {
        string key;
        cin >> key;

        h.printProbeSequence(key);
    }

    return 0;
}