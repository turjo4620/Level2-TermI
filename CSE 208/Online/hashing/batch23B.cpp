#include <bits/stdc++.h>

using namespace std;

struct data{
    string key;
    int value;
};

class hashtable
{
public:

    int current_size;
    int num_elements;
    int which_hash;

    data* probing_table;

    hashtable(int this_hash){

        current_size = 13;
        num_elements = 0;
        which_hash = this_hash;

        probing_table = new data[current_size];

        for(int i = 0; i < current_size; i++){
            probing_table[i].key = "";
        }
    }

    ~hashtable(){
        delete[] probing_table;
    }

    int hash1(string k){

        long long hash_value = 0;

        for(int i = 0; i < k.size(); i++){
            hash_value = (hash_value * 31 + k[i]) % current_size;
        }

        return (int)hash_value;
    }

    int hash2(string k){

        long long hash_value = 5381;

        for(int i = 0; i < k.size(); i++){
            hash_value = (hash_value * 37 + k[i]) % current_size;
        }

        return (int)hash_value;
    }

    int auxHash(string k){

        long long hash_value = 0;

        for(int i = 0; i < k.size(); i++){
            hash_value += k[i];
        }

        hash_value = hash_value % (current_size - 1);

        return (int)hash_value + 1;
    }

    int which_hash_use(string k){

        if(which_hash == 1){
            return hash1(k);
        }
        else{
            return hash2(k);
        }
    }

    bool isPrime(int n){

        if(n < 2){
            return false;
        }

        if(n == 2){
            return true;
        }

        if(n % 2 == 0){
            return false;
        }

        for(int i = 3; i * i <= n; i += 2){

            if(n % i == 0){
                return false;
            }
        }

        return true;
    }

    int getNextSize(){

        int candidate = 2 * current_size + 1;

        while(!isPrime(candidate)){
            candidate++;
        }

        return candidate;
    }

    void rehash(int new_size){

        data* old_table = probing_table;

        int old_size = current_size;

        current_size = new_size;

        probing_table = new data[current_size];

        for(int i = 0; i < current_size; i++){
            probing_table[i].key = "";
        }

        for(int i = 0; i < old_size; i++){

            if(old_table[i].key != "" &&
               old_table[i].key != "DELETE"){

                string key = old_table[i].key;
                int value = old_table[i].value;

                int index = which_hash_use(key);
                int aux_hash_value = auxHash(key);

                int count = 0;

                while(probing_table[index].key != ""){

                    count++;

                    index = (which_hash_use(key)
                            + count * aux_hash_value)
                            % current_size;
                }

                probing_table[index].key = key;
                probing_table[index].value = value;
            }
        }

        delete[] old_table;
    }

    bool insert(string key, int val){

        if((double)(num_elements + 1) / current_size > 0.5){

            int new_size = getNextSize();

            rehash(new_size);
        }

        int index = which_hash_use(key);

        int aux_hash_value = auxHash(key);

        int count = 0;

        while(probing_table[index].key != "" &&
              probing_table[index].key != "DELETE"){

            if(probing_table[index].key == key){
                return false;
            }

            count++;

            index = (which_hash_use(key)
                    + count * aux_hash_value)
                    % current_size;
        }

        probing_table[index].key = key;
        probing_table[index].value = val;

        num_elements++;

        return true;
    }

    int search(string key){

        int index = which_hash_use(key);

        int aux_hash_value = auxHash(key);

        int count = 0;

        int hits_count = 1;

        while(probing_table[index].key != ""){

            if(probing_table[index].key == key){
                return hits_count;
            }

            hits_count++;
            count++;

            index = (which_hash_use(key)
                    + count * aux_hash_value)
                    % current_size;
        }

        return hits_count;
    }

    void printProbeSequence(string key){

    int index = which_hash_use(key);

    int aux_hash_value = auxHash(key);

    int count = 0;

    while(true){

        cout << index << " -> "
             << probing_table[index].key << endl;

        if(probing_table[index].key == key ||
           probing_table[index].key == ""){

            return;
        }

        count++;

        index = (which_hash_use(key)
                + count * aux_hash_value)
                % current_size;
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

    set<string> words;
    vector<pair<string, int>> data_set;

    vector<string> test_words = {
        "hala",
        "khuka",
        "baba",
        "mama",
        "dada",
        "kaka",
        "nana",
        "tata"
    };

    int count = 1;

    for(string w : test_words){

        words.insert(w);
        data_set.push_back({w, count});

        count++;
    }

    while(words.size() < 10000){

        string w = random_word_generator(10);

        if(words.find(w) == words.end()){

            words.insert(w);
            data_set.push_back({w, count});

            count++;
        }
    }

    int which_hash = 1;

    hashtable double_hashing_hash1(which_hash);

    for(int i = 0; i < data_set.size(); i++){

        double_hashing_hash1.insert(
            data_set[i].first,
            data_set[i].second
        );
    }

    int t;

    cin >> t;

    while(t--){

        string key;

        cin >> key;

        double_hashing_hash1.printProbeSequence(key);
    }

    return 0;
}