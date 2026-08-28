#include <bits/stdc++.h>

using namespace std;

template <typename K, typename V>
struct data{
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


    list<data<K, V>>* seperate_chain_table;
    data<K, V>* probing_table;  


    pair<double, double>load_cut_off = {0.25, 0.5};


    hashtable(int type, int this_hash, int c1, int c2){
        method_type = type;
        current_size = initial_size;
        num_elements = 0;
        insert_count = 0;
        delete_count = 0;
        collision_count = 0;
        C1 = c1;
        C2 = c2;

        which_hash = this_hash;

        if(method_type == 1){
           seperate_chain_table = new list<data<K, V>>[current_size];
        }
        else{
            probing_table = new data<K, V>[current_size];
        }

    }

    string convertToString(K k) {
        ostringstream oss;
        oss << k;
        return oss.str();
    }

    int hash1(K k){
        long long hash_value = 0;
        string str_k = convertToString(k);
        for(int i = 0; i < str_k.size(); i++){
            hash_value = (hash_value * 31 + str_k[i]) % current_size;
        }

        return int(hash_value);
    }

    int hash2(K k){
        long long hash_value = 5381;
        string str_k = convertToString(k);
        for(int i = 0; i < str_k.size(); i++){
            hash_value = (hash_value * 37 + str_k[i]) % current_size;
        }

        return int(hash_value);
    }

    int auxHash(K k){
        long long hash_value = 0;
        string str_k = convertToString(k);
        for(int i = 0; i < str_k.size(); i++){
            hash_value = (hash_value + str_k[i]);
        }

        hash_value = hash_value % (current_size - 1);
        return int(hash_value) + 1;
    }

    int which_hash_use(K k){
        if(which_hash == 1){
            return hash1(k);
        }
        else{
            return hash2(k);
        }
    }

    void rehash(int new_size){
        int old_size = current_size;
        current_size  = new_size;

        if(method_type == 1){
            list<data<K, V>>* old_seperate_chain_table = seperate_chain_table;

            seperate_chain_table = new list<data<K, V>>[current_size];

            for(int i = 0; i < old_size; i++){
                for(data<K, V> item : old_seperate_chain_table[i]){
                    int new_index = which_hash_use(item.key);
                    seperate_chain_table[new_index].push_back(item);
                }
            }

            delete[] old_seperate_chain_table;
        }
        else{
            data<K, V>* old_probing_table = probing_table;

            probing_table = new data<K, V>[current_size];


            for(int i = 0; i < old_size; i++){
                if(!old_probing_table[i].is_empty && !old_probing_table[i].is_delete){
                    K current_key = old_probing_table[i].key;
                    int current_value = old_probing_table[i].value;

                    int index = which_hash_use(current_key);
                    int aux_hash_value = auxHash(current_key);

                    int count = 0;

                    while(!probing_table[index].is_empty && !probing_table[index].is_delete){

                        count++;
                        if(method_type == 2){
                            index = (which_hash_use(current_key) + count * aux_hash_value) % current_size;
                        }
                        else if(method_type == 3){
                            index = (which_hash_use(current_key) + C1 * count * aux_hash_value + C2 * count * count) % current_size;
                        }
                    }   
                    probing_table[index].key = current_key;
                    probing_table[index].value = current_value;
                }
            }

            delete[] old_probing_table;
        }

        
    }

    double load_factor_calc(){
        return (double)num_elements / current_size;
    }
    
    bool isprime(int n){
        if(n < 2) return false;
        if(n == 2) return true;
        if(n % 2 == 0) return false;
        for(int i = 3; i * i <= n; i++){
            if(n % i == 0){
                return false;
            }
        }
        return true;
    }
    int if_load_factor_exceeds(){
        int a = 2 * current_size;
        while(true){
            a++;
            if(isprime(a)){
                return a;
            }
        }
        return -1;
    }
    int if_load_factor_falls(){
        int a = current_size / 2;
        while(true){
            a--;
            if(isprime(a)){
                return a;
            }
        }
        return -1;
    }


    
   bool insert(K key, V val){
    if(method_type == 1){
        int index = which_hash_use(key);
        for(data<K, V> item : seperate_chain_table[index]){
            if(key == item.key){
                return false;
            }
        }
        if(!seperate_chain_table[index].empty()){
            collision_count++;
        }
        seperate_chain_table[index].push_back({key, val});
        num_elements++;
        insert_count++;
        
        if(load_factor_calc() > load_cut_off.second && insert_count >= num_elements / 2){
            int new_size = if_load_factor_exceeds();
            rehash(new_size);
            insert_count = 0;
        }
        return true;
    }
    else if(method_type == 2){
        int index = which_hash_use(key);
        int aux_hash_value = auxHash(key);

        int count = 0;
        bool collision_check = false;

        while(!probing_table[index].is_empty && !probing_table[index].is_delete){
            if(probing_table[index].key == key){
                return false;
            }

            if(!collision_check){
                collision_count++;
                collision_check = true;
            }
            count++;
            index = (which_hash_use(key) + count * aux_hash_value) % current_size;
        }
        probing_table[index].key = key;
        probing_table[index].value = val;
        probing_table[index].is_empty = false;
        probing_table[index].is_delete = false;
        num_elements++;
        insert_count++;
        if(load_factor_calc() > load_cut_off.second && insert_count >= num_elements / 2){
            int new_size = if_load_factor_exceeds();
            rehash(new_size);
            insert_count = 0;
        }
        return true;
    }
    else if(method_type == 3){
        int index = which_hash_use(key);
        int aux_hash_value = auxHash(key);

        int count = 0;
        bool collision_check = false;

        while(!probing_table[index].is_empty && !probing_table[index].is_delete){
            if(probing_table[index].key == key){
                return false;
            }

            if(!collision_check){
                collision_count++;
                collision_check = true;
            }
            count++;
            index = (which_hash_use(key) + C1 * count * aux_hash_value + C2 * count * count) % current_size;
        }
        probing_table[index].key = key;
        probing_table[index].value = val;
        probing_table[index].is_empty = false;
        probing_table[index].is_delete = false;
        num_elements++;
        insert_count++;
        if(load_factor_calc() > load_cut_off.second && insert_count >= num_elements / 2){
            int new_size = if_load_factor_exceeds();
            rehash(new_size);
            insert_count = 0;
        }
        return true;
    }
   }
    
   bool remove(K key){
    if(method_type == 1){
        int index = which_hash_use(key);
        for(auto iterator = seperate_chain_table[index].begin(); iterator != seperate_chain_table[index].end(); iterator++){
            if(iterator->key == key){
                seperate_chain_table[index].erase(iterator);
                num_elements--;
                delete_count++;
                if(load_factor_calc() < load_cut_off.first && delete_count >= num_elements / 2){
                int new_size = if_load_factor_falls();
                if(new_size != initial_size){
                rehash(new_size);
                delete_count = 0;
                    }   
                }    
                return true;
            }
        }
        return false;

    }
    else if(method_type == 2){
        int index = which_hash_use(key);
        int aux_hash_value = auxHash(key);
        int count = 0;

        while(!probing_table[index].is_empty){
            if(!probing_table[index].is_delete && probing_table[index].key == key){
               probing_table[index].is_delete = true;
               num_elements--;
               delete_count++;
               if(load_factor_calc() < load_cut_off.first && delete_count >= num_elements / 2){
                int new_size = if_load_factor_falls();
                if(new_size != initial_size){
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
    else if(method_type == 3){
        int index = which_hash_use(key);
        int aux_hash_value = auxHash(key);
        int count = 0;

        while(!probing_table[index].is_empty){
            if(!probing_table[index].is_delete && probing_table[index].key == key){
               probing_table[index].is_delete = true;
               num_elements--;
               delete_count++;
               if(load_factor_calc() < load_cut_off.first && delete_count >= num_elements / 2){
                int new_size = if_load_factor_falls();
                if(new_size != initial_size){
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
    
   int search(K key){
    if(method_type == 1){
        int hits_count = 1;
        int index = which_hash_use(key);
        for(data<K, V> item : seperate_chain_table[index]){
            if(item.key == key){
                return hits_count;
            }
            hits_count++;
        }
        return hits_count;
    }

    else if(method_type == 2){
        int index = which_hash_use(key);
        int aux_hash_value = auxHash(key);
        int count = 0;
        int hits_count = 1;

        while(!probing_table[index].is_empty){
            if(!probing_table[index].is_delete && probing_table[index].key == key){
               return hits_count;
            }
            hits_count++;
            count++;
            index = (which_hash_use(key) + count * aux_hash_value) % current_size;
        }
        return hits_count;
    }
    else if(method_type == 3){
        int index = which_hash_use(key);
        int aux_hash_value = auxHash(key);
        int count = 0;
        int hits_count = 1;

        while(!probing_table[index].is_empty){
            if(!probing_table[index].is_delete && probing_table[index].key == key){
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


string random_word_generator(int length){
    string w = "";
    for(int i = 0; i < length; i++){
        char c = 'a' + (rand() % 26);
        w += c;
    }
    return w;
}


int main(){
    
    srand(time(0));

    set<string>words;
    vector<pair<string, int>>data_set;
    int count = 1;

    while(words.size() < 10000){
        string w = random_word_generator(10);
        if(words.find(w) == words.end()){
            words.insert(w);
            data_set.push_back({w, count});
            count++;
        }
    }

    int method_type;
    int which_hash;


    // insert part
    method_type = 1;
    which_hash = 1;

    int c1, c2;
    cin>>c1>>c2;
    
    hashtable<string, int> chain_hash1(method_type, which_hash, c1, c2);

    which_hash = 2;
    hashtable<string, int> chain_hash2(method_type, which_hash, c1, c2);

    for(int i = 0; i < data_set.size(); i++){
        chain_hash1.insert(data_set[i].first, data_set[i].second);
    }

    int collision_chain_hash1 = chain_hash1.collision_count;
    
    for(int i = 0; i < data_set.size(); i++){
        chain_hash2.insert(data_set[i].first, data_set[i].second);
    }

    int collision_chain_hash2 = chain_hash2.collision_count;

    method_type = 2; // double hashing
    which_hash = 1;

    hashtable<string, int> double_hashing_hash1(method_type, which_hash, c1, c2);

    which_hash = 2;

    hashtable<string, int> double_hashing_hash2(method_type, which_hash, c1, c2);


    for(int i = 0; i < data_set.size(); i++){
        double_hashing_hash1.insert(data_set[i].first, data_set[i].second);
    }

    int collision_double_hashing_hash1 = double_hashing_hash1.collision_count;
    
    for(int i = 0; i < data_set.size(); i++){
        double_hashing_hash2.insert(data_set[i].first, data_set[i].second);
    }

    int collision_double_hashing_hash2 = double_hashing_hash2.collision_count;



    method_type = 3;
    which_hash = 1;

    hashtable<string, int> custom_probing_hash1(method_type, which_hash, c1, c2);

    which_hash = 2;

    hashtable<string, int> custom_probing_hash2(method_type, which_hash, c1, c2);

    for(int i = 0; i < data_set.size(); i++){
        custom_probing_hash1.insert(data_set[i].first, data_set[i].second);
    }

    int collision_custom_probing_hash1 = custom_probing_hash1.collision_count;
    
    for(int i = 0; i < data_set.size(); i++){
        custom_probing_hash2.insert(data_set[i].first, data_set[i].second);
    }

    int collision_custom_probing_hash2 = custom_probing_hash2.collision_count;

   
    
    int total_hit_count_chain_hash1 = 0;
    int total_hit_count_chain_hash2 = 0;
    int total_hit_count_double_hash1 = 0;
    int total_hit_count_double_hash2 = 0;
    int total_hit_count_custom_hash1 = 0;
    int total_hit_count_custom_hash2 = 0;

    vector<pair<string, int>>temp_search = data_set;


    random_device rd;
    mt19937 g(rd());

    shuffle(temp_search.begin(), temp_search.end(), g);

    for(int i = 0; i < 1000; i++){
        total_hit_count_chain_hash1 += chain_hash1.search(temp_search[i].first);
    }
    for(int i = 0; i < 1000; i++){
        total_hit_count_chain_hash2 += chain_hash2.search(temp_search[i].first);
    }
    for(int i = 0; i < 1000; i++){
        total_hit_count_double_hash1 += double_hashing_hash1.search(temp_search[i].first);
    }
    for(int i = 0; i < 1000; i++){
        total_hit_count_double_hash2 += double_hashing_hash2.search(temp_search[i].first);
    }
    for(int i = 0; i < 1000; i++){
        total_hit_count_custom_hash1 += custom_probing_hash1.search(temp_search[i].first);
    }
    for(int i = 0; i < 1000; i++){
        total_hit_count_custom_hash2 += custom_probing_hash2.search(temp_search[i].first);
    }

    double average_hits_chain_hash1 = total_hit_count_chain_hash1 / 1000.0;
    double average_hits_chain_hash2 = total_hit_count_chain_hash2 / 1000.0;
    double average_hits_double_hash1 = total_hit_count_double_hash1 / 1000.0;
    double average_hits_double_hash2 = total_hit_count_double_hash2 / 1000.0;
    double average_hits_custom_hash1 = total_hit_count_custom_hash1 / 1000.0;
    double average_hits_custom_hash2 = total_hit_count_custom_hash2 / 1000.0;


    cout << "                         HASH TABLE PERFORMANCE REPORT                   \n";

    cout << setw(20) << left << "Chaining Method" 
         << setw(18) << collision_chain_hash1 
         << setw(16) << fixed << setprecision(4) << average_hits_chain_hash1 
         << setw(18) << collision_chain_hash2 
         << setw(16) << fixed << setprecision(4) << average_hits_chain_hash2 << endl;

    cout << setw(20) << left << "Double Hashing" 
         << setw(18) << collision_double_hashing_hash1 
         << setw(16) << fixed << setprecision(4) << average_hits_double_hash1 
         << setw(18) << collision_double_hashing_hash2 
         << setw(16) << fixed << setprecision(4) << average_hits_double_hash2 << endl;

    cout << setw(20) << left << "Custom Probing" 
         << setw(18) << collision_custom_probing_hash1 
         << setw(16) << fixed << setprecision(4) << average_hits_custom_hash1 
         << setw(18) << collision_custom_probing_hash2 
         << setw(16) << fixed << setprecision(4) << average_hits_custom_hash2 << endl;
    

    return 0;
}