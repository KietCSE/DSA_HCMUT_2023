#include <bits/stdc++.h>
using namespace std;

class huffmanTree {
private:
    class Node {
    public:
        char key;
        int value;
        Node* left;
        Node* right;
        int time;
        int height; 
        Node(int value, int time, char x = '#', Node* left = nullptr, Node* right = nullptr)
            : value(value), time(time), key(x), left(left), right(right), height(1) {}
        // ky tu '#' la nhung node noi 
    };

    struct CompareNode {
        bool operator()(const Node* a, const Node* b) const {
            if (a->value != b->value) {
                return a->value > b->value;
            }
            else if (a->time != b->time) {
                return a->time > b->time;
            }
            else {
                if ((a->key >= 'a' && a->key <= 'z' && b->key >= 'A' && b->key <= 'Z') ||
                    (a->key >= 'A' && a->key <= 'Z' && b->key >= 'a' && b->key <= 'z')) {
                    return a->key < b->key;
                }
                else return a->key > b->key;
            }
        }
    };

    int Time;
    Node* root;

protected:
    void ClearRec(Node* root) {
        if (!root) return;
        ClearRec(root->left);
        ClearRec(root->right);
        delete root;
    }

    void EncodeChar(Node* root, string& s, int index, map<char,string>& mp) {
        if (root->left) {
            s.push_back('0');
            EncodeChar(root->left, s, index + 1, mp);
            s.pop_back();
        }
        if (root->right) {
            s.push_back('1');
            EncodeChar(root->right, s, index + 1, mp);
            s.pop_back();
        }
        if (root->right == nullptr && root->left == nullptr) {
            mp[root->key] = s;
        }
    }

    int getHeight(Node* root) { if (!root) return 0; else return root->height; }
    int getBalance(Node* root) { if (!root) return 0; else return getHeight(root->left) - getHeight(root->right); }

    Node* rotateRight(Node* root) {
        Node* t = root->left; 
        root->left = t->right;
        t->right = root;

        // update height 
        root->height = 1 + max(getHeight(root->left), getHeight(root->right));
        t->height = 1 + max(getHeight(t->left), getHeight(t->right));

        return t;
    }

    Node* rotateLeft(Node* root) {
        Node* t = root->right; 
        root->right = t->left; 
        t->left = root;

        // update height
        root->height = 1 + max(getHeight(root->left), getHeight(root->right));
        t->height = 1 + max(getHeight(t->left), getHeight(t->right));

        return t;
    }

    bool RotateNode(Node*& root, Node*& nextRoot) {
        int balance = getBalance(root);
        // left > right
        if (balance > 1) {
            // left of left 
            if (getBalance(root->left) >= 0) {
                nextRoot = root->left;  // giu lai vi tri node root moi 
                root = rotateRight(root);
            }
            // right of left
            else {
                nextRoot = root->left->right;
                root->left = rotateLeft(root->left);
                root = rotateRight(root);
            }
            return true;
         }
        // right > left
        else if (balance < -1) {
            // right of right 
            if (getBalance(root->right) <= 0) {
                nextRoot = root->right;
                root = rotateLeft(root);
            }
            // left of right
            else {
                nextRoot = root->right->left;
                root->right = rotateRight(root->right);
                root = rotateLeft(root);
            }
            return true;
        }
        // khong xoay cay
        return false;
    }

    void TravelRotationRec(Node*& root, int& index) {
        if (!root || index >= 3) return;
        Node* nextRoot = nullptr;
        //this->print(root, "");
        if (RotateNode(root, nextRoot)) {
            index++;
            TravelRotationRec(nextRoot->left, index);
            TravelRotationRec(nextRoot->right, index);
        }
        else {
            TravelRotationRec(root->left, index);
            TravelRotationRec(root->right, index);
        }
    }

    void TravelRotation(Node*& root) {
        int index = 0;
        TravelRotationRec(root, index);
    }

    void InorderPrint(Node* root) {
        if (!root) return;
        InorderPrint(root->left); 
        if (root->key == '#') cout << root->value << '\n';
        else cout << root->key << '\n';
        InorderPrint(root->right);
    }

    // ham test 
    void print(Node* root, string s) {
        if (!root) {
            cout << s << "null" << endl;
            return;
        }
        if (root->left == nullptr || root->right == nullptr) {
            cout << s << root->value << "/" << root->key << endl;
        }
        else cout << s << root->value << endl;
        s += "  ";
        s.push_back(char(179));
        print(root->left, s);
        print(root->right, s);
    }

public:
    huffmanTree() : Time(0), root(nullptr) {}
    ~huffmanTree() { ClearTree(); }

    void ClearTree() {
        ClearRec(root); 
        Time = 0;
        root = nullptr;
    }

    void CreateTree(const vector<pair<char, int>>& arr) {
        priority_queue<Node*, vector<Node*>, CompareNode> q;

        for (const pair<char, int>& x : arr) {
            Node* temp = new Node(x.second, 0, x.first);
            q.push(temp);
        }

        //while (!q.empty()) {
        //    Node* temp = q.top(); 
        //    q.pop(); 
        //    cout << temp->value << ' ' << temp->key << ' ' << temp->time << endl;
        //}

        while (q.size() > 1) {  
            Node* temp1 = q.top(); q.pop(); 
            Node* temp2 = q.top(); q.pop();
            Node* temp3 = new Node(temp1->value + temp2->value, ++Time, '#', temp1, temp2);
            // update height
            temp3->height = 1 + max(getHeight(temp1), getHeight(temp2));
            // rotate
            TravelRotation(temp3);
            print(temp3, "");
            cout << "-----------------------------" << endl;
            q.push(temp3);
        } 
        
        this->root = q.top(); 
        q.pop();
    }

    string EncodeString(const string& name) {
        map<char, string> mp;
        string s = "";
        EncodeChar(this->root, s, 0, mp);
        string res = ""; 
        for (unsigned int i = 0; i < name.length(); i++) {
            res += mp[name[i]];
        }
        string result = "";
        int n = res.length();
        for (int i = n - 1; i >= 0 && i >= n - 10; i--) {
            result.push_back(res[i]);
        }
        return result;
    }

    // in thong tin cay 
    void InorderCustomer() {
        this->InorderPrint(this->root);
    }

    // ham test
    void builtTree() { this->print(root, ""); }
};
           
int main() {
    huffmanTree* tree = new huffmanTree(); 
    //vector<pair<char, int>> arr = { {'a',1},{'b', 1},{'A',1}, {'B',2},{'c',1}, {'L',2}, {'l',2}, {'v',2}, {'t',10}, {'G',20}, {'T',30}};
    vector<pair<char, int>> arr = { {'Z',2}, {'K', 7}, {'M',24}, {'C', 32}, {'U',37}, {'D',42}, {'L',42}, {'E',120} };
    tree->CreateTree(arr);  // abABBcLLllvvttttttttttGGGGGGGGGGGGGGGGGGGGTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT
    tree->builtTree();
    tree->InorderCustomer();
    //string a = tree.EncodeString("abL");
    //cout << a;
    delete tree;
    return 0;
}

