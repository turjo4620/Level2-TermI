#include <bits/stdc++.h>
using namespace std;

enum State
{
    EMPTY,
    OCCUPIED,
    DELETED
};

struct Slot
{
    long long process_id;
    State state;

    Slot()
    {
        process_id = -1;
        state = EMPTY;
    }
};

class MemoryManager
{
private:
    int initialSize;
    int tableSize;
    int P_MAX;

    int occupiedCount;
    int deletionCount;

    // Maximum probe sequence encountered in the current table
    int maxProbeSequence;

    vector<Slot> table;

    // ------------------------------------------------------------
    // Check whether a number is prime
    // ------------------------------------------------------------
    bool isPrime(int n)
    {
        if (n < 2)
            return false;

        if (n == 2)
            return true;

        if (n % 2 == 0)
            return false;

        for (int i = 3; 1LL * i * i <= n; i += 2)
        {
            if (n % i == 0)
                return false;
        }

        return true;
    }

    // ------------------------------------------------------------
    // Smallest prime >= x
    // ------------------------------------------------------------
    int nextPrime(int x)
    {
        while (!isPrime(x))
            x++;

        return x;
    }

    // ------------------------------------------------------------
    // Largest prime <= x
    // ------------------------------------------------------------
    int previousPrime(int x)
    {
        while (x >= 2 && !isPrime(x))
            x--;

        return x;
    }

    // ------------------------------------------------------------
    // Hash function
    // ------------------------------------------------------------
    int hashFunction(long long key)
    {
        return key % tableSize;
    }

    // ------------------------------------------------------------
    // Insert into a table during rehashing
    //
    // No output is produced here.
    // Returns number of probes used.
    // ------------------------------------------------------------
    int insertDuringRehash(long long process_id)
    {
        for (long long j = 0; j < tableSize; j++)
        {
            int index =
                (int)((process_id + j * j) % tableSize);

            int probes = (int)j + 1;

            if (table[index].state == EMPTY)
            {
                table[index].process_id = process_id;
                table[index].state = OCCUPIED;

                return probes;
            }
        }

        return tableSize;
    }

    // ------------------------------------------------------------
    // Calculate maximum probe sequence for the current table
    //
    // This checks how many probes are needed to find each
    // currently occupied process.
    // ------------------------------------------------------------
    int calculateMaxProbeSequence()
    {
        int maximum = 0;

        for (int i = 0; i < tableSize; i++)
        {
            if (table[i].state != OCCUPIED)
                continue;

            long long key = table[i].process_id;

            for (long long j = 0; j < tableSize; j++)
            {
                int index =
                    (int)((key + j * j) % tableSize);

                int probes = (int)j + 1;

                if (index == i)
                {
                    maximum = max(maximum, probes);
                    break;
                }
            }
        }

        return maximum;
    }

    // ------------------------------------------------------------
    // Print rehash report
    // ------------------------------------------------------------
    void printBeforeReport()
    {
        double loadFactor =
            (double)occupiedCount / tableSize;

        cout << "--- Rehash Triggered: BEFORE ---\n";
        cout << "Table Size: " << tableSize << "\n";
        cout << "Active Processes: " << occupiedCount << "\n";
        cout << fixed << setprecision(2);
        cout << "Load Factor: " << loadFactor << "\n";
        cout << "Max Probe Sequence: "
             << maxProbeSequence << "\n";
    }

    void printAfterReport()
    {
        double loadFactor =
            (double)occupiedCount / tableSize;

        cout << "--- Rehash Triggered: AFTER ---\n";
        cout << "Table Size: " << tableSize << "\n";
        cout << "Active Processes: " << occupiedCount << "\n";
        cout << fixed << setprecision(2);
        cout << "Load Factor: " << loadFactor << "\n";
        cout << "Max Probe Sequence: "
             << maxProbeSequence << "\n";
    }

    // ------------------------------------------------------------
    // Rehash table to new size
    // ------------------------------------------------------------
    void rehash(int newSize)
    {
        // Save all currently occupied process IDs
        vector<long long> processes;

        for (int i = 0; i < tableSize; i++)
        {
            if (table[i].state == OCCUPIED)
            {
                processes.push_back(table[i].process_id);
            }
        }

        // Create new table
        tableSize = newSize;
        table.clear();
        table.resize(tableSize);

        // Reset metadata
        occupiedCount = 0;
        deletionCount = 0;
        maxProbeSequence = 0;

        // Reinsert occupied processes
        for (long long process_id : processes)
        {
            int probes = insertDuringRehash(process_id);

            occupiedCount++;

            maxProbeSequence =
                max(maxProbeSequence, probes);
        }
    }

    // ------------------------------------------------------------
    // Scale up
    // ------------------------------------------------------------
    void scaleUp()
    {
        printBeforeReport();

        int newSize =
            nextPrime(2 * tableSize);

        rehash(newSize);

        printAfterReport();
    }

    // ------------------------------------------------------------
    // Scale down
    // ------------------------------------------------------------
    void scaleDown()
    {
        int target =
            previousPrime(tableSize / 2);

        // Do not shrink below initial size
        if (target < initialSize)
            return;

        // If for some reason target is not smaller,
        // do nothing.
        if (target >= tableSize)
            return;

        printBeforeReport();

        rehash(target);

        printAfterReport();
    }

public:

    // ------------------------------------------------------------
    // Constructor
    // ------------------------------------------------------------
    MemoryManager(int N, int P)
    {
        initialSize = N;
        tableSize = N;
        P_MAX = P;

        occupiedCount = 0;
        deletionCount = 0;
        maxProbeSequence = 0;

        table.resize(tableSize);
    }

    // ------------------------------------------------------------
    // ALLOCATE
    // ------------------------------------------------------------
    void allocate(long long process_id)
    {
        int firstDeleted = -1;

        int probes = 0;
        int insertedIndex = -1;

        for (long long j = 0; j < tableSize; j++)
        {
            int index =
                (int)((process_id + j * j) % tableSize);

            probes++;

            // Empty slot
            if (table[index].state == EMPTY)
            {
                if (firstDeleted != -1)
                    insertedIndex = firstDeleted;
                else
                    insertedIndex = index;

                break;
            }

            // Deleted slot
            if (table[index].state == DELETED)
            {
                if (firstDeleted == -1)
                    firstDeleted = index;
            }

            // Occupied slot with same process ID
            if (table[index].state == OCCUPIED &&
                table[index].process_id == process_id)
            {
                // Already allocated.
                // The problem statement does not specify
                // duplicate allocation behavior.
                // We simply report the existing location.
                insertedIndex = index;
                break;
            }
        }

        // If no EMPTY slot was found but a DELETED slot exists
        if (insertedIndex == -1 && firstDeleted != -1)
        {
            insertedIndex = firstDeleted;
        }

        // Table completely full
        if (insertedIndex == -1)
        {
            // In practice this should be handled by resizing.
            // Scale up and retry.
            scaleUp();
            allocate(process_id);
            return;
        }

        // If this was a new allocation
        if (table[insertedIndex].state != OCCUPIED)
        {
            table[insertedIndex].process_id = process_id;
            table[insertedIndex].state = OCCUPIED;

            occupiedCount++;

            // A new allocation removes one deleted slot
            if (table[insertedIndex].state == DELETED)
            {
                // This condition cannot be reached because
                // state has already been changed to OCCUPIED.
            }
        }

        maxProbeSequence =
            max(maxProbeSequence, probes);

        cout << "ALLOCATE " << process_id
             << ": Inserted at index "
             << insertedIndex
             << " (probes: "
             << probes
             << ")\n";

        // Probe count exceeds P_MAX
        if (probes > P_MAX)
        {
            scaleUp();
        }
    }

    // ------------------------------------------------------------
    // FREE
    // ------------------------------------------------------------
    void freeProcess(long long process_id)
    {
        for (long long j = 0; j < tableSize; j++)
        {
            int index =
                (int)((process_id + j * j) % tableSize);

            // If EMPTY is reached, process does not exist
            if (table[index].state == EMPTY)
            {
                cout << "FREE " << process_id
                     << ": Process not found\n";
                return;
            }

            if (table[index].state == OCCUPIED &&
                table[index].process_id == process_id)
            {
                table[index].state = DELETED;

                occupiedCount--;
                deletionCount++;

                cout << "FREE " << process_id
                     << ": Freed from index "
                     << index << "\n";

                // After every 5 deletions
                if (deletionCount % 5 == 0)
                {
                    double loadFactor =
                        (double)occupiedCount / tableSize;

                    if (loadFactor < 0.2)
                    {
                        int target =
                            previousPrime(tableSize / 2);

                        if (target >= initialSize &&
                            target < tableSize)
                        {
                            scaleDown();
                        }
                    }
                }

                return;
            }
        }

        cout << "FREE " << process_id
             << ": Process not found\n";
    }

    // ------------------------------------------------------------
    // Process commands
    // ------------------------------------------------------------
    void processCommand(string command, long long process_id)
    {
        if (command == "ALLOCATE")
        {
            allocate(process_id);
        }
        else if (command == "FREE")
        {
            freeProcess(process_id);
        }
    }
};

// ================================================================
// MAIN
// ================================================================

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, P_MAX, Q;

    cin >> N >> P_MAX >> Q;

    MemoryManager manager(N, P_MAX);

    for (int i = 0; i < Q; i++)
    {
        string command;
        long long process_id;

        cin >> command >> process_id;

        manager.processCommand(command, process_id);
    }

    return 0;
}
