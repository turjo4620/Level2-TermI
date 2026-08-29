// final

#include <bits/stdc++.h>

using namespace std;

template <typename K, typename V>
struct hashdata
{
    K key;
    V value;
    bool is_empty = true;
    bool is_delete = false;
};

template <typename K, typename V>
class hashtable
{
public:
    int initial_size = 13;
    int current_size;
    int num_elements;
    double alpha;
    int insert_count;
    int delete_count;
    int method_type;
    int collision_count;
    int C1;
    int C2;

    int which_hash;
    int last_n;

    int elemets_last_resize = 0;

    list<hashdata<K, V>> *seperate_chain_table;
    hashdata<K, V> *probing_table;

    pair<double, double> load_cut_off = {0.25, 0.5};

    hashtable(int type, int this_hash, int c1, int c2)
    {
        method_type = type;
        current_size = initial_size;
        num_elements = 0;
        insert_count = 0;
        delete_count = 0;
        collision_count = 0;
        C1 = c1;
        C2 = c2;

        which_hash = this_hash;

        if (method_type == 1)
        {
            seperate_chain_table = new list<hashdata<K, V>>[current_size];
        }
        else
        {
            probing_table = new hashdata<K, V>[current_size];
        }
    }

    string convertToString(K k)
    {
        ostringstream oss;
        oss << k;
        return oss.str();
    }

    // updated hash1 function
    int hash1(K k)
    {
        long long hash_value = 0;
        string str_k = convertToString(k);
        for (int i = 1; i <= str_k.size(); i++)
        {
            hash_value += i * str_k[i - 1];
        }

        return int(hash_value) % 13;
    }

    // added weight function
    double weight(K k)
    {
        string str_k = convertToString(k);
        double w = ((str_k[0] + str_k[str_k.size() - 1]) % 100) / 100.0;
        return w;
    }

    int hash2(K k)
    {
        long long hash_value = 5381;
        string str_k = convertToString(k);
        for (int i = 0; i < str_k.size(); i++)
        {
            hash_value = (hash_value * 37 + str_k[i]) % current_size;
        }

        return int(hash_value);
    }

    int auxHash(K k)
    {
        long long hash_value = 0;
        string str_k = convertToString(k);
        for (int i = 0; i < str_k.size(); i++)
        {
            hash_value = (hash_value + str_k[i]);
        }

        hash_value = hash_value % (current_size - 1);
        return int(hash_value) + 1;
    }

    int which_hash_use(K k)
    {
        if (which_hash == 1)
        {
            return hash1(k);
        }
        else
        {
            return hash2(k);
        }
    }

    void rehash(int new_size)
    {
        int old_size = current_size;
        current_size = new_size;

        if (method_type == 1)
        {
            list<hashdata<K, V>> *old_seperate_chain_table = seperate_chain_table;

            seperate_chain_table = new list<hashdata<K, V>>[current_size];

            for (int i = 0; i < old_size; i++)
            {
                for (hashdata<K, V> item : old_seperate_chain_table[i])
                {
                    int new_index = which_hash_use(item.key);
                    if (!seperate_chain_table[new_index].empty())
                    {
                        collision_count++;
                    }
                    seperate_chain_table[new_index].push_back(item);
                }
            }

            delete[] old_seperate_chain_table;
        }
        else
        {
            hashdata<K, V> *old_probing_table = probing_table;

            probing_table = new hashdata<K, V>[current_size];

            for (int i = 0; i < old_size; i++)
            {
                if (!old_probing_table[i].is_empty && !old_probing_table[i].is_delete)
                {
                    K current_key = old_probing_table[i].key;
                    V current_value = old_probing_table[i].value;

                    int index = which_hash_use(current_key);
                    int aux_hash_value = auxHash(current_key);

                    if (!probing_table[index].is_empty)
                    {
                        collision_count++;
                    }

                    int count = 0;

                    while (!probing_table[index].is_empty && !probing_table[index].is_delete)
                    {

                        count++;
                        if (method_type == 2)
                        {
                            index = (which_hash_use(current_key) + count * aux_hash_value) % current_size;
                        }
                        else if (method_type == 3)
                        {
                            index = (which_hash_use(current_key) + C1 * count * aux_hash_value + C2 * count * count) % current_size;
                        }
                    }
                    probing_table[index].key = current_key;
                    probing_table[index].value = current_value;
                    probing_table[index].is_empty = false;
                    probing_table[index].is_delete = false;
                }
            }

            delete[] old_probing_table;
        }

        elemets_last_resize = num_elements;
    }

    double load_factor_calc()
    {
        return (double)num_elements / current_size;
    }

    bool isprime(int n)
    {
        if (n < 2)
            return false;
        if (n == 2)
            return true;
        if (n % 2 == 0)
            return false;
        for (int i = 3; i * i <= n; i++)
        {
            if (n % i == 0)
            {
                return false;
            }
        }
        return true;
    }
    int if_load_factor_exceeds()
    {
        int a = 2 * current_size;
        while (true)
        {
            a++;
            if (isprime(a))
            {
                return a;
            }
        }
        return -1;
    }
    int if_load_factor_falls()
    {
        int a = current_size / 2;
        while (true)
        {
            a--;
            if (isprime(a))
            {
                return a;
            }
        }
        return -1;
    }

    bool insert(K key, V val)
    {
        if (method_type == 1)
        {
            int index = which_hash_use(key);
            for (hashdata<K, V> item : seperate_chain_table[index])
            {
                if (key == item.key)
                {
                    return false;
                }
            }
            if (!seperate_chain_table[index].empty())
            {
                collision_count++;
            }

            double new_weight = weight(key);

            auto it = seperate_chain_table[index].begin();

            while(it != seperate_chain_table[index].end()){
                double current_weight = weight(it->key);

                if(new_weight > current_weight){
                    break;
                }
                it++;
            }
            seperate_chain_table[index].insert(it, {key, val});
            num_elements++;
            insert_count++;

            // if (load_factor_calc() > load_cut_off.second && insert_count >= elemets_last_resize / 2)
            // {
            //     int new_size = if_load_factor_exceeds();
            //     rehash(new_size);
            //     insert_count = 0;
            // }
            return true;
        }
        else if (method_type == 2)
        {
            int index = which_hash_use(key);
            int aux_hash_value = auxHash(key);

            int count = 0;
            int first_d = -1;
            bool collision_check = false;

            while (!probing_table[index].is_empty && count < current_size)
            {
                if (probing_table[index].is_delete)
                {
                    if (first_d == -1)
                    {
                        first_d = index;
                    }
                }
                else
                {
                    if (probing_table[index].key == key)
                    {
                        return false;
                    }

                    if (!collision_check)
                    {
                        collision_count++;
                        collision_check = true;
                    }
                }
                count++;
                index = (which_hash_use(key) + count * aux_hash_value) % current_size;
            }
            if (count == current_size && first_d == -1)
            {
                return false;
            }

            if (first_d != -1)
            {
                index = first_d;
            }
            probing_table[index].key = key;
            probing_table[index].value = val;
            probing_table[index].is_empty = false;
            probing_table[index].is_delete = false;
            num_elements++;
            insert_count++;
            if (load_factor_calc() > load_cut_off.second && insert_count >= elemets_last_resize / 2)
            {
                int new_size = if_load_factor_exceeds();
                rehash(new_size);
                insert_count = 0;
            }
            return true;
        }
        else if (method_type == 3)
        {
            int index = which_hash_use(key);
            int aux_hash_value = auxHash(key);

            int count = 0;
            int first_d = -1;
            bool collision_check = false;

            while (!probing_table[index].is_empty && count < current_size)
            {

                if (probing_table[index].is_delete)
                {
                    if (first_d == -1)
                    {
                        first_d = index;
                    }
                }
                else
                {
                    if (probing_table[index].key == key)
                    {
                        return false;
                    }

                    if (!collision_check)
                    {
                        collision_count++;
                        collision_check = true;
                    }
                }
                count++;
                index = (which_hash_use(key) + C1 * count * aux_hash_value + C2 * count * count) % current_size;
            }
            if (count == current_size && first_d == -1)
            {
                return false;
            }
            if (first_d != -1)
            {
                index = first_d;
            }
            probing_table[index].key = key;
            probing_table[index].value = val;
            probing_table[index].is_empty = false;
            probing_table[index].is_delete = false;
            num_elements++;
            insert_count++;
            if (load_factor_calc() > load_cut_off.second && insert_count >= elemets_last_resize / 2)
            {
                int new_size = if_load_factor_exceeds();
                rehash(new_size);
                insert_count = 0;
            }
            return true;
        }
    }

    bool remove(K key)
    {
        if (method_type == 1)
        {
            int index = which_hash_use(key);
            for (auto iterator = seperate_chain_table[index].begin(); iterator != seperate_chain_table[index].end(); iterator++)
            {
                if (iterator->key == key)
                {
                    seperate_chain_table[index].erase(iterator);
                    num_elements--;
                    delete_count++;
                    if (load_factor_calc() < load_cut_off.first && delete_count >= elemets_last_resize / 2)
                    {
                        int new_size = if_load_factor_falls();
                        if (current_size != initial_size)
                        {
                            rehash(new_size);
                            delete_count = 0;
                        }
                    }
                    return true;
                }
            }
            return false;
        }
        else if (method_type == 2)
        {
            int index = which_hash_use(key);
            int aux_hash_value = auxHash(key);
            int count = 0;

            while (!probing_table[index].is_empty)
            {
                if (!probing_table[index].is_delete && probing_table[index].key == key)
                {
                    probing_table[index].is_delete = true;
                    num_elements--;
                    delete_count++;
                    if (load_factor_calc() < load_cut_off.first && delete_count >= elemets_last_resize / 2)
                    {
                        int new_size = if_load_factor_falls();
                        if (current_size != initial_size)
                        {
                            rehash(new_size);
                            delete_count = 0;
                        }
                    }
                    return true;
                }

                count++;
                index = (which_hash_use(key) + count * aux_hash_value) % current_size;
            }
            return false;
        }
        else if (method_type == 3)
        {
            int index = which_hash_use(key);
            int aux_hash_value = auxHash(key);
            int count = 0;

            while (!probing_table[index].is_empty)
            {
                if (!probing_table[index].is_delete && probing_table[index].key == key)
                {
                    probing_table[index].is_delete = true;
                    num_elements--;
                    delete_count++;
                    if (load_factor_calc() < load_cut_off.first && delete_count >= elemets_last_resize / 2)
                    {
                        int new_size = if_load_factor_falls();
                        if (current_size != initial_size)
                        {
                            rehash(new_size);
                            delete_count = 0;
                        }
                    }
                    return true;
                }

                count++;
                index = (which_hash_use(key) + C1 * count * aux_hash_value + C2 * count * count) % current_size;
            }
            return false;
        }
    }

    int search(K key)
    {
        if (method_type == 1)
        {
            int hits_count = 1;
            int index = which_hash_use(key);
            for (hashdata<K, V> item : seperate_chain_table[index])
            {
                if (item.key == key)
                {
                    return hits_count;
                }
                hits_count++;
            }
            return hits_count;
        }

        else if (method_type == 2)
        {
            int index = which_hash_use(key);
            int aux_hash_value = auxHash(key);
            int count = 0;
            int hits_count = 1;

            while (!probing_table[index].is_empty)
            {
                if (!probing_table[index].is_delete && probing_table[index].key == key)
                {
                    return hits_count;
                }
                hits_count++;
                count++;
                index = (which_hash_use(key) + count * aux_hash_value) % current_size;
            }
            return hits_count;
        }
        else if (method_type == 3)
        {
            int index = which_hash_use(key);
            int aux_hash_value = auxHash(key);
            int count = 0;
            int hits_count = 1;

            while (!probing_table[index].is_empty)
            {
                if (!probing_table[index].is_delete && probing_table[index].key == key)
                {
                    return hits_count;
                }
                hits_count++;
                count++;
                index = (which_hash_use(key) + C1 * count * aux_hash_value + C2 * count * count) % current_size;
            }
            return hits_count;
        }
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

    // srand(time(0));

    // set<string>words;
    // vector<pair<string, int>>data_set;
    // int count = 1;

    // int word_length;
    // cin>>word_length;
    // while(words.size() < 10000){
    //     string w = random_word_generator(word_length);
    //     if(words.find(w) == words.end()){
    //         words.insert(w);
    //         data_set.push_back({w, count});
    //         count++;
    //     }
    // }

    int method_type;
    int which_hash;

    // insert part
    method_type = 1;
    which_hash = 1;

    int c1, c2;
    cin >> c1 >> c2;

    hashtable<string, double> chain_hash1(method_type, which_hash, c1, c2);

    int n;
    cin >> n;
    vector<pair<string, double>> v;

    for (int i = 0; i < n; i++)
    {
        string word;
        cin >> word;
        v.push_back({word, chain_hash1.weight(word)});
    }

    vector<pair<string, double>> v_copy = v;

    // sort(v_copy.begin(), v_copy.end(),
    //      [](const pair<string, double> &a,
    //         const pair<string, double> &b)
    //      {
    //          return a.second > b.second;
    //      });

    for (int i = 0; i < 12; i++)
    {
        chain_hash1.insert(v_copy[i].first, v_copy[i].second);
    }

    for (int i = 0; i < 12; i++)
    {
        cout << v[i].first << " " << chain_hash1.hash1(v[i].first) << " " << v[i].second << endl;
    }


    for(int i = 0; i < 12; i++){
        for(hashdata<string, double> item : chain_hash1.seperate_chain_table[i]){
            cout<<i<<" "<<item.key<<"("<<item.value<<")"<<"->";
        }
        cout<<endl;
    }

    for(int i = 0; i <= 12; i++){
        if(chain_hash1.seperate_chain_table[i].size() == 0){
            cout<<i<<",";
        }
    }
    cout<<"empty";

    return 0;
}