//final

#include <bits/stdc++.h>

using namespace std;

template <typename K, typename V>
struct hashdata{
    K key;
    V value;
    int frequency = 1;
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


    list<hashdata<K, V>>* seperate_chain_table;
    hashdata<K, V>* probing_table;  


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
           seperate_chain_table = new list<hashdata<K, V>>[current_size];
        }
        else{
            probing_table = new hashdata<K, V>[current_size];
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
            list<hashdata<K, V>>* old_seperate_chain_table = seperate_chain_table;

            seperate_chain_table = new list<hashdata<K, V>>[current_size];

            for(int i = 0; i < old_size; i++){
                for(hashdata<K, V> item : old_seperate_chain_table[i]){
                    int new_index = which_hash_use(item.key);
                    if(!seperate_chain_table[new_index].empty()){
                        collision_count++;
                        }
                    seperate_chain_table[new_index].push_back(item);
                }
            }

            delete[] old_seperate_chain_table;
        }
        else{
            hashdata<K, V>* old_probing_table = probing_table;

            probing_table = new hashdata<K, V>[current_size];


            for(int i = 0; i < old_size; i++){
                if(!old_probing_table[i].is_empty && !old_probing_table[i].is_delete){
                    K current_key = old_probing_table[i].key;
                    V current_value = old_probing_table[i].value;

                    int index = which_hash_use(current_key);
                    int aux_hash_value = auxHash(current_key);

                    if(!probing_table[index].is_empty){
                        collision_count++;
                    }

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
                    probing_table[index].is_empty = false;
                    probing_table[index].is_delete = false;
                }
            }

            delete[] old_probing_table;
        }

        elemets_last_resize = num_elements;
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
        
        for(auto it = seperate_chain_table[index].begin(); it != seperate_chain_table[index].end(); ++it){
            if(key == it->key){
                it->frequency ++;
                hashdata<K, V> updated_node = *it;
                seperate_chain_table[index].erase(it);
            

            auto insert_it = seperate_chain_table[index].begin();

            while(insert_it != seperate_chain_table[index].end() && insert_it->frequency > updated_node.frequency){
                insert_it++;
            }
            seperate_chain_table[index].insert(insert_it, updated_node);
            return true;
        }
        }
        if(!seperate_chain_table[index].empty()){
            collision_count++;
        }
        
        hashdata<K, V>newnode;
        newnode.key = key;
        newnode.value = val;
        newnode.frequency = 1;
        seperate_chain_table[index].push_back(newnode);
        num_elements++;
        insert_count++;
        
        if(load_factor_calc() > load_cut_off.second && insert_count >= elemets_last_resize / 2){
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
        int first_d = -1;
        bool collision_check = false;

        while(!probing_table[index].is_empty && count < current_size){
            if(probing_table[index].is_delete){
                if(first_d == -1){
                    first_d = index;
                }
            }
            else{ if(probing_table[index].key == key){
                return false;
            }

            if(!collision_check){
                collision_count++;
                collision_check = true;
            }}
            count++;
            index = (which_hash_use(key) + count * aux_hash_value) % current_size;
        }
        if(count == current_size && first_d == -1){
            return false;
        }

        if(first_d != -1){
            index = first_d;
        }
        probing_table[index].key = key;
        probing_table[index].value = val;
        probing_table[index].is_empty = false;
        probing_table[index].is_delete = false;
        num_elements++;
        insert_count++;
        if(load_factor_calc() > load_cut_off.second && insert_count >= elemets_last_resize / 2){
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
        int first_d = -1;
        bool collision_check = false;

        while(!probing_table[index].is_empty && count < current_size){

            if(probing_table[index].is_delete){
                if(first_d == -1){
                    first_d = index;
                }
            }
            else {if(probing_table[index].key == key){
                return false;
            }

            if(!collision_check){
                collision_count++;
                collision_check = true;
            }}
            count++;
            index = (which_hash_use(key) + C1 * count * aux_hash_value + C2 * count * count) % current_size;
        }
        if(count == current_size && first_d == -1){
            return false;
        }        
        if(first_d != -1){
            index = first_d;
        }
        probing_table[index].key = key;
        probing_table[index].value = val;
        probing_table[index].is_empty = false;
        probing_table[index].is_delete = false;
        num_elements++;
        insert_count++;
        if(load_factor_calc() > load_cut_off.second && insert_count >= elemets_last_resize / 2){
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
                if(load_factor_calc() < load_cut_off.first && delete_count >= elemets_last_resize / 2){
                int new_size = if_load_factor_falls();
                if(current_size != initial_size){
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
               if(load_factor_calc() < load_cut_off.first && delete_count >= elemets_last_resize / 2){
                int new_size = if_load_factor_falls();
                if(current_size != initial_size){
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
               if(load_factor_calc() < load_cut_off.first && delete_count >= elemets_last_resize / 2){
                int new_size = if_load_factor_falls();
                if(current_size != initial_size){
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
        for(hashdata<K, V> item : seperate_chain_table[index]){
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

   bool deleteLowestFrequency() {
        if (method_type != 1 || num_elements == 0) return false;

        int min_freq = INT_MAX;
        int target_index = -1;
        
        // We must use 'typename' here because we are inside a template class
        typename list<hashdata<K, V>>::iterator target_it; 

        // Scan the entire 2D structure (Array of Linked Lists)
        for (int i = 0; i < current_size; i++) {
            for (auto it = seperate_chain_table[i].begin(); it != seperate_chain_table[i].end(); ++it) {
                
                // If we find a strictly smaller frequency, update our target markers
                if (it->frequency < min_freq) {
                    min_freq = it->frequency;
                    target_index = i;
                    target_it = it;
                }
            }
        }

        // If a valid target was found, erase it
        if (target_index != -1) {
            seperate_chain_table[target_index].erase(target_it);
            num_elements--;
            delete_count++;
            
            // Standard load factor check for deletion
            if (load_factor_calc() < load_cut_off.first && delete_count >= elemets_last_resize / 2) {
                int new_size = if_load_factor_falls();
                if (current_size != initial_size) {
                    rehash(new_size);
                    delete_count = 0;
                }
            }
            return true;
        }
        return false;
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


int main() {
    vector<string> words = {
        "node", "tree", "data", "node", "graph", 
        "data", "edge", "node", "tree", "path", "data"
    };
    
    hashtable<string, int> freq_chain(1, 1, 0, 0);
    
    freq_chain.current_size = 13;
    freq_chain.initial_size = 13;
    freq_chain.load_cut_off.second = 100.0;
    

    for(int i = 0; i < words.size(); i++){
        freq_chain.insert(words[i], i + 1);
    }
    
    cout << "Expected Chain (head -> tail)\n";
    for(int i = 0; i < freq_chain.current_size; i++) {
        if(!freq_chain.seperate_chain_table[i].empty()) {
            cout << "Bucket " << i << ": ";
            
            bool first = true;
            for(auto it = freq_chain.seperate_chain_table[i].begin(); 
                     it != freq_chain.seperate_chain_table[i].end(); ++it) {
                
                if(!first) cout << " -> ";
                
                cout << it->key << " (" << it->frequency << ")";
                first = false;
            }
            cout << "\n";
        }
    }
    
    return 0;
}