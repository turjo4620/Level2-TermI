#include <bits/stdc++.h>

using namespace std;

const int EMPTY = 0;
const int OCCUPIED = 1;
const int DELETED = 2;


// --------------------------------------------------
// HASH FUNCTIONS
// --------------------------------------------------

int hash1(int key, int N)
{
    string k = to_string(key);

    long long hash_value = 0;

    for (char c : k)
    {
        hash_value = (hash_value * 31 + c) % N;
    }

    return hash_value;
}


int hash2(int key, int N)
{
    string k = to_string(key);

    long long hash_value = 5381;

    for (char c : k)
    {
        hash_value = (hash_value * 37 + c) % N;
    }

    return hash_value;
}


// Forward declaration
class InnerHashTable;


// --------------------------------------------------
// INNER DATA
// user_id -> permission
// --------------------------------------------------

struct InnerData
{
    int user_id;
    string permission;
    int status;
};


// --------------------------------------------------
// OUTER DATA
// group_id -> pointer to InnerHashTable
// --------------------------------------------------

struct OuterData
{
    int group_id;
    InnerHashTable *innertable;
    int status;
};


// ==================================================
// INNER HASH TABLE
// Hash2 + Quadratic Probing
// ==================================================

class InnerHashTable
{
public:
    int N;
    InnerData *table;


    InnerHashTable(int size)
    {
        N = size;

        table = new InnerData[N];

        for (int i = 0; i < N; i++)
        {
            table[i].status = EMPTY;
        }
    }


    // ----------------------------------------------
    // INSERT
    // ----------------------------------------------

    void insert(int user_id, string permission)
    {
        for (int i = 0; i < N; i++)
        {
            // Quadratic Probing
            int index = (hash2(user_id, N) + i * i) % N;


            // Empty or deleted position
            if (table[index].status == EMPTY ||
                table[index].status == DELETED)
            {
                table[index] =
                {
                    user_id,
                    permission,
                    OCCUPIED
                };

                return;
            }


            // Duplicate user
            if (table[index].status == OCCUPIED &&
                table[index].user_id == user_id)
            {
                return;
            }
        }
    }


    // ----------------------------------------------
    // SEARCH
    // Returns the index if found, otherwise -1
    // ----------------------------------------------

    int search(int user_id)
    {
        for (int i = 0; i < N; i++)
        {
            // Same quadratic probe sequence
            int index = (hash2(user_id, N) + i * i) % N;


            // EMPTY means key cannot exist further
            if (table[index].status == EMPTY)
            {
                return -1;
            }


            // Check only occupied entries
            if (table[index].status == OCCUPIED &&
                table[index].user_id == user_id)
            {
                return index;
            }


            // DELETED -> continue probing
        }

        return -1;
    }


    // ----------------------------------------------
    // DELETE
    // ----------------------------------------------

    bool remove(int user_id, string &permission)
    {
        int index = search(user_id);

        if (index == -1)
        {
            return false;
        }

        permission = table[index].permission;

        table[index].status = DELETED;

        return true;
    }


    // ----------------------------------------------
    // PRINT ALL USERS
    // ----------------------------------------------

    void printAll()
    {
        bool first = true;

        for (int i = 0; i < N; i++)
        {
            if (table[i].status == OCCUPIED)
            {
                if (!first)
                {
                    cout << ", ";
                }

                cout << "("
                     << table[i].user_id
                     << ", "
                     << table[i].permission
                     << ")";

                first = false;
            }
        }

        cout << endl;
    }
};



// ==================================================
// OUTER HASH TABLE
// Hash1 + Linear Probing
// ==================================================

class OuterHashTable
{
public:
    int N;
    OuterData *table;


    OuterHashTable(int size)
    {
        N = size;

        table = new OuterData[N];

        for (int i = 0; i < N; i++)
        {
            table[i].status = EMPTY;
            table[i].innertable = nullptr;
        }
    }


    // ----------------------------------------------
    // FIND GROUP
    // Returns index if found
    // Otherwise -1
    // ----------------------------------------------

    int findGroup(int group_id)
    {
        for (int i = 0; i < N; i++)
        {
            // Linear Probing
            int index = (hash1(group_id, N) + i) % N;


            // Group does not exist
            if (table[index].status == EMPTY)
            {
                return -1;
            }


            if (table[index].status == OCCUPIED &&
                table[index].group_id == group_id)
            {
                return index;
            }


            // DELETED -> continue probing
        }

        return -1;
    }


    // ----------------------------------------------
    // INSERT
    // INSERT group_id user_id permission
    // ----------------------------------------------

    void insert(int group_id,
                int user_id,
                string permission)
    {
        for (int i = 0; i < N; i++)
        {
            // Linear Probing
            int index = (hash1(group_id, N) + i) % N;


            // Group already exists
            if (table[index].status == OCCUPIED &&
                table[index].group_id == group_id)
            {
                table[index].innertable
                    ->insert(user_id, permission);

                return;
            }


            // New group
            if (table[index].status == EMPTY ||
                table[index].status == DELETED)
            {
                table[index].group_id = group_id;

                table[index].status = OCCUPIED;

                // Create inner table
                table[index].innertable =
                    new InnerHashTable(N);

                // Insert first user
                table[index].innertable
                    ->insert(user_id, permission);

                return;
            }
        }
    }


    // ----------------------------------------------
    // SEARCH group_id user_id
    // ----------------------------------------------

    void search(int group_id, int user_id)
    {
        int group_index = findGroup(group_id);


        // Group not found
        if (group_index == -1)
        {
            cout << "Group not found" << endl;
            return;
        }


        // Search user in inner table
        int user_index =
            table[group_index]
                .innertable
                ->search(user_id);


        if (user_index == -1)
        {
            cout << "User not found in group "
                 << group_id
                 << endl;

            return;
        }


        cout <<
            table[group_index]
                .innertable
                ->table[user_index]
                .permission
            << endl;
    }


    // ----------------------------------------------
    // SEARCH group_id
    // ----------------------------------------------

    void search(int group_id)
    {
        int group_index = findGroup(group_id);


        if (group_index == -1)
        {
            cout << "Group not found" << endl;
            return;
        }


        table[group_index]
            .innertable
            ->printAll();
    }


    // ----------------------------------------------
    // DELETE group_id user_id
    // ----------------------------------------------

    void remove(int group_id, int user_id)
    {
        int group_index = findGroup(group_id);


        if (group_index == -1)
        {
            cout << "Group not found" << endl;
            return;
        }


        string permission;


        bool deleted =
            table[group_index]
                .innertable
                ->remove(user_id, permission);


        if (!deleted)
        {
            cout << "User not found in group "
                 << group_id
                 << endl;

            return;
        }


        cout << "("
             << user_id
             << ", "
             << permission
             << ") deleted"
             << endl;
    }
};



// ==================================================
// MAIN
// ==================================================

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);


    int N, Q;

    cin >> N >> Q;


    OuterHashTable hashTable(N);


    while (Q--)
    {
        string command;

        cin >> command;


        // INSERT group_id user_id permission
        if (command == "INSERT")
        {
            int group_id;
            int user_id;
            string permission;

            cin >> group_id
                >> user_id
                >> permission;


            hashTable.insert(
                group_id,
                user_id,
                permission
            );
        }


        // SEARCH group_id
        // SEARCH group_id user_id
        else if (command == "SEARCH")
        {
            string line;

            getline(cin, line);


            stringstream ss(line);

            int group_id;
            int user_id;


            ss >> group_id;


            // Two-key search
            if (ss >> user_id)
            {
                hashTable.search(
                    group_id,
                    user_id
                );
            }

            // One-key search
            else
            {
                hashTable.search(group_id);
            }
        }


        // DELETE group_id user_id
        else if (command == "DELETE")
        {
            int group_id;
            int user_id;

            cin >> group_id >> user_id;


            hashTable.remove(
                group_id,
                user_id
            );
        }
    }


    return 0;
}