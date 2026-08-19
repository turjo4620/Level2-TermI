#include <bits/stdc++.h>

using namespace std;

class Node
{
public:
    int st, end, id, maxEnd;
    signed int h;
    Node *left_c;
    Node *right_c;

    Node(int st, int end, int id)
    {
        this->st = st;
        this->end = end;
        this->id = id;
        maxEnd = end;
        left_c = nullptr;
        right_c = nullptr;
        h = 1;
    }
};

int id = 1;

class AVLTree
{

    Node *insert_helper(Node *node, int st, int end, int c_id, bool &done)
    {
        if (node == nullptr)
        {
            Node *new_node = new Node(st, end, c_id);
            done = true;
            return new_node;
        }

        if (node->st != st)
        {
            if (st < node->st)
            {
                node->left_c = insert_helper(node->left_c, st, end, c_id, done);
            }
            else if (st > node->st)
            {
                node->right_c = insert_helper(node->right_c, st, end, c_id, done);
            }
        }
        else
        {
            if (c_id < node->id)
            {
                node->left_c = insert_helper(node->left_c, st, end, c_id, done);
            }
            else if (c_id > node->id)
            {
                node->right_c = insert_helper(node->right_c, st, end, c_id, done);
            }
            else
            {
                done = false;
                return node;
            }
        }

        node->h = 1 + max(height(node->left_c), height(node->right_c));
        node->maxEnd = max({node->end, maxEnd_get(node->left_c), maxEnd_get(node->right_c)});

        signed int balance_factor = balance_factor_calc(node);

        if (balance_factor > 1){
            if(st < node->left_c->st){
                return right_rotation(node);
            }
            else if(st == node->left_c->st && c_id < node->left_c->id){
                return right_rotation(node);
            }
            else{
                return left_right_rotation(node);
            }
        }

        if(balance_factor < -1){
            if(st > node->right_c->st){
                return left_rotation(node);
            }
            else if(st == node->right_c->st && c_id > node->right_c->id){
                return left_rotation(node);
            }
            else{
                return right_left_rotation(node);
            }
        }
        return node;
    }

    Node *successor(Node *node)
    {
        Node *curr = node;
        while (curr->left_c != nullptr)
        {
            curr = curr->left_c;
        }
        return curr;
    }

    Node *remove_helper(Node *node, int st, int end, int c_id, bool &done)
    {
        if (node == nullptr)
        {
            done = false;
            return node;
        }

        if (st < node->st)
        {
            node->left_c = remove_helper(node->left_c, st, end, c_id, done);
        }
        else if (st == node->st && c_id < node->id)
        {
            node->left_c = remove_helper(node->left_c, st, end, c_id, done);
            
        }
        else if(st > node->st){
            node->right_c = remove_helper(node->right_c, st, end, c_id, done);
        }
        else if(st == node->st && c_id > node->id){
            node->right_c = remove_helper(node->right_c, st, end, c_id, done);

        }
        else
        {
            done = true;
            if (node->left_c == nullptr)
            {
                Node *temp = node->right_c;
                delete node;
                return temp;
            }
            if (node->right_c == nullptr)
            {
                Node *temp = node->left_c;
                delete node;
                return temp;
            }

            bool d;

            Node *successor_node = successor(node->right_c);
            node->st = successor_node->st;
            node->end = successor_node->end;
            node->id = successor_node->id;
            node->right_c = remove_helper(node->right_c, successor_node->st, successor_node->end, successor_node->id, d);
        }

        node->h = 1 + max(height(node->left_c), height(node->right_c));

        node->maxEnd = max({node->end, maxEnd_get(node->left_c), maxEnd_get(node->right_c)});

        signed int balance_factor = balance_factor_calc(node);

        if (balance_factor > 1 && balance_factor_calc(node->left_c) >= 0)
        {
            return right_rotation(node);
        }
        if (balance_factor > 1 && balance_factor_calc(node->left_c) < 0)
        {
            return left_right_rotation(node);
        }

        if (balance_factor < -1 && balance_factor_calc(node->right_c) <= 0)
        {
            return left_rotation(node);
        }

        if (balance_factor < -1 && balance_factor_calc(node->right_c) > 0)
        {
            return right_left_rotation(node);
        }

        return node;
    }

    void overlap_helper(Node* node, int s, int e, vector<int>&ans){
        if(node == nullptr){
            return;
        }
        if(node->left_c != nullptr && node->left_c->maxEnd > s){
            overlap_helper(node->left_c, s, e, ans);
        }
        if(node->st < e && s < node->end){
            ans.push_back(node->id);
        }
        if(node->right_c != nullptr && node->st < e){
            overlap_helper(node->right_c, s, e, ans);
        }
    }

    Node* next_helper(Node* node, int t){
        Node* n = nullptr;
        while(node != nullptr){
            if(node->st >= t){
                n = node;
                node = node->left_c;
            }
            else{
                node = node->right_c;
            }
        }
        return n;
    }

public:
    Node *root;

    AVLTree()
    {
        root = nullptr;
    }

    int height(Node *node)
    {
        if (node == nullptr)
        {
            return 0;
        }

        return node->h;
    }

    int maxEnd_get(Node *node)
    {
        if (node == nullptr)
        {
            return INT_MIN;
        }
        return node->maxEnd;
    }

    signed int balance_factor_calc(Node *node)
    {
        if (node == nullptr)
        {
            return 0;
        }
        return height(node->left_c) - height(node->right_c);
    }

    Node *right_rotation(Node *node)
    {
        Node *x = node->left_c;
        Node *y = x->right_c;

        x->right_c = node;
        node->left_c = y;

        node->h = 1 + max(height(node->left_c), height(node->right_c));
        node->maxEnd = max({node->end, maxEnd_get(node->left_c), maxEnd_get(node->right_c)});
        x->h = 1 + max(height(x->left_c), height(x->right_c));
        x->maxEnd = max({x->end, maxEnd_get(x->left_c), maxEnd_get(x->right_c)});

        return x;
    }

    Node *left_rotation(Node *node)
    {
        Node *x = node->right_c;
        Node *y = x->left_c;

        x->left_c = node;
        node->right_c = y;

        node->h = 1 + max(height(node->left_c), height(node->right_c));
        node->maxEnd = max({node->end, maxEnd_get(node->left_c), maxEnd_get(node->right_c)});

        x->h = 1 + max(height(x->left_c), height(x->right_c));
        x->maxEnd = max({x->end, maxEnd_get(x->left_c), maxEnd_get(x->right_c)});


        return x;
    }

    Node *left_right_rotation(Node *node)
    {
        node->left_c = left_rotation(node->left_c);
        return right_rotation(node);
    }

    Node *right_left_rotation(Node *node)
    {
        node->right_c = right_rotation(node->right_c);
        return left_rotation(node);
    }

    bool insert(int st, int end, int id)
    {
        bool done = false;
        root = insert_helper(root, st, end, id, done);
        return done;
    }

    // vector<int> traverse()
    // {
    //     vector<int> ans;
    //     inorder_helper(root, ans);
    //     return ans;
    // }

    void print(Node *node)
    {
        if (node == nullptr)
            return;
        cout << node->id;
        if (node->left_c == nullptr && node->right_c == nullptr)
        {
            return;
        }
        else
        {
            cout << '(';
            print(node->left_c);
            cout << ',';
            print(node->right_c);
            cout << ')';
        }
    }

    bool erase(int st, int end, int id)
    {
        bool done = false;
        root = remove_helper(root, st, end, id, done);
        return done;
    }

    vector<int> OVERLAPS(int s, int e){
        vector<int>ans;
        overlap_helper(root ,s, e, ans);
        return ans;
    }

    Node* next(int t){
        return next_helper(root, t);
    }
};

int main()
{

    AVLTree AT;

    string c;
    int a, s, e;


    int next_id = 1;
    unordered_map<int, pair<int, int>>interval;


    while (cin >> c)
    {

        // if (c != 'T')
        // {
        //     cin >> a;
        // }

        if(c == "ADD"){
            cin>>s>>e;
            interval[next_id] = {s, e};
            AT.insert(s, e, next_id++);
            AT.print(AT.root);
            cout<<endl;
        }
        else if(c == "UPDATE"){
            int target, new_s, new_e;
            cin>>target>>new_s>>new_e;
            if(interval.find(target) != interval.end()){
                int old_s = interval[target].first;
                int old_e = interval[target].second;
                AT.erase(old_s, old_e, target);
                interval[target] = {new_s, new_e};
                AT.insert(new_s, new_e, target);
                AT.print(AT.root);
                cout<<endl;
            }
            else{
                cout<<"not found"<<endl;
            }
        }
        else if(c == "REMOVE"){
            int t;
            cin>>t;
            if(interval.find(t) != interval.end()){
                int s = interval[t].first;
                int e = interval[t].second;
                AT.erase(s, e, t);
                interval.erase(t);
                AT.print(AT.root);
                cout<<endl;
            }
            else{
                cout<<"not found"<<endl;
            }
        }
        else if(c == "OVERLAPS"){
            cin>>s>>e;
            vector<int>ans;
            ans = AT.OVERLAPS(s, e);
            if(ans.size() <= 0){
                cout<<"none"<<endl;
            }
            else{
                for(int i = 0; i < ans.size(); i++){
                    cout<<ans[i];
                    if(i != ans.size() - 1){
                        cout<<" ";
                    }
                }
                cout<<endl;
            }
            
        }
        else if(c == "AT"){
            cin>>s;
            vector<int>ans;
            ans = AT.OVERLAPS(s, s + 1);
            if(ans.size() <= 0){
                cout<<"none"<<endl;
            }
            else{
                for(int i = 0; i < ans.size(); i++){
                    cout<<ans[i];
                    if(i != ans.size() - 1){
                        cout<<" ";
                    }
                }
                cout<<endl;
            }
            
        }
        else if(c == "NEXT"){
            cin>>s;
            Node* ans = AT.next(s);
            if(ans == nullptr) cout<<"none"<<endl;
            else{
                cout<<ans->id<<" "<<ans->st<<" "<<ans->end<<endl;
            }
        }
        else if(c == "CONFLICT"){
            cin>>s>>e;
            vector<int>ans = AT.OVERLAPS(s, e);
            if(ans.size() > 0){
                cout<<"yes"<<endl;
            }
            else{
                cout<<"no"<<endl;
            }
        }
    }

    return 0;
}