#include <bits/stdc++.h>

using namespace std;

// int hashing

class hashSet
{
public:
    int table_size;
    vector<list<int>> data;

    hashSet(int size)
    {
        table_size = size;
        data.resize(size);
    }

    int hash_function(int key)
    {
        return key % table_size;
    }

    bool contains(int key)
    {
        int index = hash_function(key);

        for (int x : data[index])
        {
            if (x == key)
            {
                return true;
            }
        }
        return false;
    }

    void insert(int key)
    {
        if (contains(key))
        {
            return;
        }
        int index = hash_function(key);
        data[index].push_back(key);
    }
};

int main()
{

    int n;
    cin >> n;
    vector<int> A(n);
    hashSet setA(100);
    for (int i = 0; i < n; i++)
    {
        cin >> A[i];
        setA.insert(A[i]);
    }
    int m;
    cin >> m;
    vector<int> B(m);
    hashSet setB(100);
    for (int i = 0; i < m; i++)
    {
        cin >> B[i];
        setB.insert(B[i]);
    }

    vector<int> uni;
    vector<int> intersec;
    vector<int> diff;

    for (int x : A)
    {
        if (setB.contains(x))
        {
            intersec.push_back(x);
        }
        else
        {
            diff.push_back(x);
        }
    }

    for (int x : A)
    {
        uni.push_back(x);
    }

    for (int x : B)
    {
        if (!setA.contains(x))
        {
            uni.push_back(x);
        }
    }

    sort(intersec.begin(), intersec.end());
    sort(uni.begin(), uni.end());
    sort(diff.begin(), diff.end());

    cout << "Intersection: ";

    for (int x : intersec)
    {

        cout << x << " ";
    }

    cout << endl;

    cout << "Union: ";

    for (int x : uni)
    {

        cout << x << " ";
    }

    cout << endl;

    cout << "Diff (A-B): ";

    for (int x : diff)
    {

        cout << x << " ";
    }

    cout << endl;

    return 0;
}