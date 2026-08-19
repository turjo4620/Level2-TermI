#include <bits/stdc++.h>

using namespace std;

class Node{
public:
    signed int key;
    signed int h;
    Node* left_c;
    Node* right_c;


    Node(int val){
        this->key = val;
        left_c = nullptr;
        right_c = nullptr;
        h = 1;
    }
};



class AVLTree{

        Node* insert_helper(Node* node, int key, bool& done){
        if(node == nullptr){
            Node* new_node = new Node(key);
            done = true;
            return new_node;
        }

        if(key < node->key){
            node->left_c = insert_helper(node->left_c, key, done);
        }
        else if (key > node->key){
            node->right_c = insert_helper(node->right_c, key, done);
        }
        else{
            done = false;
            return node;
        }

        node->h = 1 + max(height(node->left_c), height(node->right_c));


        signed int balance_factor = balance_factor_calc(node);

        if(balance_factor > 1 && key < node->left_c->key){
            return right_rotation(node);
        }
        else if(balance_factor > 1 && key > node->left_c->key){
            return left_right_rotation(node);
        }
        else if(balance_factor < -1 && key > node->right_c->key){
            return left_rotation(node);
        }
        else if(balance_factor < -1 && key < node->right_c->key){
            return right_left_rotation(node);
        }
        else{
            return node;
        }
    }

    void inorder_helper(Node* node, vector<int>&ans){
        if(node == nullptr) return;

        inorder_helper(node->left_c, ans);
        ans.push_back(node->key);
        inorder_helper(node->right_c, ans);
    }

    Node* successor(Node* node){
        Node* curr = node;
        while(curr->left_c != nullptr){
            curr = curr->left_c;
        }
        return curr;
    }

    Node* remove_helper(Node* node, int key, bool& done){
        if(node == nullptr){
            done = false;
            return node;
        }

        if(key < node->key){
            node->left_c = remove_helper(node->left_c, key, done);
        }
        else if(key > node->key){
            node->right_c = remove_helper(node->right_c, key, done);
        }
        else{
            done = true;
            if(node->left_c == nullptr){
                Node* temp = node->right_c;
                delete node;
                return temp;
            }
            if(node->right_c == nullptr){
                Node* temp = node->left_c;
                delete node;
                return temp;
            }
            
            bool d;

            Node* successor_node = successor(node->right_c);
            node->key = successor_node->key;
            node->right_c = remove_helper(node->right_c, successor_node->key, d);
            
        }

        node->h = 1 + max(height(node->left_c), height(node->right_c));

        signed int balance_factor = balance_factor_calc(node);

        if(balance_factor > 1 && balance_factor_calc(node->left_c) >= 0){
            return right_rotation(node);
        }
        if(balance_factor > 1 && balance_factor_calc(node->left_c) < 0){
            return left_right_rotation(node);
        }

        if(balance_factor < -1 && balance_factor_calc(node->right_c) <= 0){
            return left_rotation(node);
        }
        
        if(balance_factor < -1 && balance_factor_calc(node->right_c) > 0){
            return right_left_rotation(node);
        }

        return node;
    }



public:

    Node* root;

    AVLTree(){
        root = nullptr;
    }

    int height(Node* node){
        if(node == nullptr){
            return 0;
        }

        return node->h;
    }


    signed int balance_factor_calc(Node* node){
        if(node == nullptr){
            return 0;
        }
        return height(node->left_c) - height(node->right_c);
    }

    Node* right_rotation(Node* node){
        Node* x = node->left_c;
        Node* y = x->right_c;

        x->right_c = node;
        node->left_c = y;

        node->h = 1 + max(height(node->left_c), height(node->right_c));
        x->h = 1 + max(height(x->left_c), height(x->right_c));

        return x;
    }

    Node* left_rotation(Node* node){
        Node* x = node->right_c;
        Node* y = x->left_c;

        x->left_c = node;
        node->right_c = y;

        node->h = 1 + max(height(node->left_c), height(node->right_c));
        x->h = 1 + max(height(x->left_c), height(x->right_c));

        return x;        
    }


    Node* left_right_rotation(Node* node){
        node->left_c = left_rotation(node->left_c);
        return right_rotation(node);
    }


    Node* right_left_rotation(Node* node){
        node->right_c = right_rotation(node->right_c);
        return left_rotation(node);
    }

    bool insert(int key){
        bool done = false;
        root  = insert_helper(root, key, done);
        return done;
    }

    bool find(int key){
        Node* curr = root;
        while(curr != nullptr){
            if(curr->key == key){
                return true;
            }
            if(key < curr->key){
                curr = curr->left_c;
            }
            else if(key > curr->key){
                curr = curr->right_c;
            }
        }
        return false;
    }


    vector<int> traverse(){
        vector<int>ans;
        inorder_helper(root, ans);
        return ans;
    }


    void print(Node* node){
        if(node == nullptr) return; 
        cout<<node->key;
        if(node->left_c == nullptr && node->right_c == nullptr){
            return;
        }
        else{
            cout<<'(';
            print(node->left_c);
            cout<<',';
            print(node->right_c);
            cout<<')';
        }
    }

    bool erase(int key){
        bool done = false;
        root = remove_helper(root, key, done);
        return done;
    }
    


};




int main(){

    AVLTree AT;

    char c;
    int a;

    while(cin>>c){

        if(c != 'T'){
            cin>>a;
        }
        
        switch (c) {
        
        case 'I':
            if(AT.insert(a)){
                AT.print(AT.root);
                cout<<endl;
            }
            else{
                cout<<"duplicate"<<endl;
            }
            break;
        
        case 'D':
            if(AT.erase(a)){
                AT.print(AT.root);
                cout<<endl;
            }
            else{
                cout<<"not found"<<endl;
            }
            break;

        case 'F':
            if(AT.find(a)){
                cout<<"found"<<endl;
            }
            else{
                cout<<"not found"<<endl;
            }
            break;

        case 'T':
            {vector<int>ans = AT.traverse();
            for(int i = 0; i < ans.size(); i++){
                cout<<ans[i];
                if(i != ans.size() - 1){
                    cout<<" ";
                }
            }
            cout<<endl;
            break;}

        default:
            break;
        }
    }

    
    return 0;
}