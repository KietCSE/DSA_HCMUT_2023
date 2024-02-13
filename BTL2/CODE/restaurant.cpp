#include "main.h"

static int MAXSIZE = 10;

// DECLARE ALL CLASS USING

class Customer;
class BST;
class HashNode;
class ListCustomer;
class HeapNode;
class Help;
class GojoRestaurant;
class SukunaRestaurant;
class huffmanTree;
class EncodeCustomer;
class Restaurant;


// REPARATION FOR DESIGN 


// cutomer for restaurant 
// include time and result num
// use for StackTime, BST (G-restaurent) and ListCustomer (S-restaurant) 
class Customer {
public:
    int CustomerTime; 
    int CustomerResult;
    Customer* next; 
    Customer* prev; 
    Customer* left; 
    Customer* right;
    Customer() : CustomerTime(0), CustomerResult(0), next(nullptr), prev(nullptr), left(nullptr), right(nullptr) {}
    Customer(int time, int result, Customer* next = nullptr, Customer* prev = nullptr, Customer* left = nullptr, Customer* right = nullptr) 
        : CustomerTime(time), CustomerResult(result), next(next), prev(prev) , left(left), right(right) {}
    ~Customer() {
        next = prev = left = right = nullptr;
    }
};

// list to store customer in a heapnode 
// store Customer 
class ListCustomer {
private:
    Customer* head; 
    Customer* tail;
    int NumCustomer;

    friend class Help;
public:
    ListCustomer() : head(nullptr), tail(nullptr), NumCustomer(0) {}
    ~ListCustomer() { Clear(); }

    void Clear() {
        Customer* temp = head;
        while (head != nullptr) {
            temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
        head = nullptr;
        NumCustomer = 0;
    }

    void InsertCustomer(Customer* p) {
        if (head == nullptr) {
            head = p; 
            tail = head; 
        }
        else {
            tail->next = p; 
            p->prev = tail; 
            tail = p; 
        }
        NumCustomer++;
    }

    Customer* DeleteHead() {
        if (NumCustomer == 0) return nullptr;
        Customer* temp = head; 
        head = head->next;
        temp->next = nullptr; 
        temp->prev = nullptr;
        if (head != nullptr) head->prev = nullptr;
        if (head == nullptr) tail = nullptr;
        NumCustomer--;
        return temp;
    }

    Customer* DeleteTail() {
        if (NumCustomer == 0) return nullptr;
        Customer* temp = tail; 
        tail = tail->prev;
        temp->prev = nullptr;
        temp->next = nullptr;
        if (tail != nullptr) tail->next = nullptr;
        if (tail == nullptr) head = nullptr;
        NumCustomer--;
        return temp;
    }

    void PrintLIFO(int IDregion, int num) {
        for (Customer* p = this->tail; p != nullptr && num > 0; p = p->prev) {
            cout << IDregion << '-' << p->CustomerResult << '\n';
            num--;
        }
    }

    void PrintFIFO(int ID) {
        for (Customer* p = this->head; p != nullptr; p = p->next) {
            cout << p->CustomerResult << '-' << ID << '\n';
        }
    }


    // ham test
    void Print() {
        Customer* t = head; 
        while (t != nullptr) {
            cout << t->CustomerResult << ' ' << t->CustomerTime << endl;
            t = t->next;
        }
    }

    int getSize() { return NumCustomer; }
};

// node for heap array 
// include num people, ID, Listcustomer, Time modifed
class HeapNode {
private:
    ListCustomer* region;
    friend class Help;
public:
    int NumOfCustomer;
    int ID;
    int TimeModified;
    int TimeStart;

    HeapNode() : region(new ListCustomer()), NumOfCustomer(0), ID(0), TimeModified(0), TimeStart(0) {}
    ~HeapNode() {
        delete region;
        NumOfCustomer = 0;
    }  

    void InsertHeapRegion(int time, Customer* p) {
        // cap nhap thoi gian bat dau nhan khach 
        if (NumOfCustomer == 0) TimeStart = p->CustomerTime;
        region->InsertCustomer(p);
        NumOfCustomer++;
        TimeModified = time;
    }

    // xoa num khach hang theo thu tu FIFO
    void DeleteCustomer(int time, int num) {
        if (num >= region->getSize()) {
            region->PrintFIFO(this->ID);
            region->Clear();
            NumOfCustomer = 0;
            TimeStart = 0;
            TimeModified = time;
        }
        else {
            for (int i = 0; i < num; i++) {
                Customer* p = region->DeleteHead(); // FIFO
                // in thong tin khach hang bi duoi 
                cout << p->CustomerResult << '-' << ID << endl;
                delete p;
                NumOfCustomer--;
            }
            this->TimeModified = time;
        }
    }

    void setID(int index) { this->ID = index; }

    //CLEAVE
    void PrintLIFOPeople(int num) { region->PrintLIFO(this->ID, num); }
   

    // ham test
    void Testprint() {
        cout << "[ID:" << ID << "num:" << NumOfCustomer << "Modified:" << TimeModified << "Start:" << TimeStart << "]" << endl;
        region->Print();
    }

};

class Help {
    friend class BST;
protected:

    static long Rec(vector<int>& nums, vector<vector<int>>& C) {
        unsigned int n = nums.size();
        if (n <= 2) return 1;
        vector<int> left, right;
        int root = nums[n - 1];

        for (unsigned int i = 0; i < n - 1; i++) {
            if (nums[i] < root) left.push_back(nums[i]);
            else right.push_back(nums[i]);
        }

        long r = Rec(right, C) % MAXSIZE;
        long l = Rec(left, C) % MAXSIZE; 
        unsigned int len = left.size();
        if (len > (n - 1) / 2) len = (n - 1) - len;
        long a = C[n - 1][len];
        return a * r % MAXSIZE * l % MAXSIZE;
    }

public:
    static void swap(HeapNode& a, HeapNode& b) {
        int tempID = a.ID;
        int tempNum = a.NumOfCustomer;
        int tempTime = a.TimeModified;
        Customer* tempHead = a.region->head;
        Customer* tempTail = a.region->tail;
        int numRegion = a.region->NumCustomer;

        a.ID = b.ID;
        a.NumOfCustomer = b.NumOfCustomer;
        a.TimeModified = b.TimeModified;
        a.region->head = b.region->head;
        a.region->tail = b.region->tail;
        a.region->NumCustomer = b.region->NumCustomer;

        b.ID = tempID;
        b.TimeModified = tempTime;
        b.NumOfCustomer = tempNum;
        b.region->head = tempHead;
        b.region->tail = tempTail;
        b.region->NumCustomer = numRegion;
    }

    static int numOfWays(vector<int>& nums) {
        unsigned int n = nums.size();
        if (n == 0) return 0;
        //vector<vector<int>> C = Combiantion(n, n/2+1);
        int k = n / 2 + 1;
        vector<vector<int>> C(n + 1, vector<int>(k + 1, 0));
        for (unsigned int i = 0; i <= n; i++) C[i][0] = 1;

        for (unsigned int i = 1; i <= n; i++) {
            for (int j = 1; j <= k; j++) {
                C[i][j] = (C[i - 1][j - 1] + C[i - 1][j]) % MAXSIZE;
            }
        }

        int a = Rec(nums, C);
        return a % MAXSIZE; 
    }
};

// BST store customer for each region 
class BST {
private:
    Customer* root;
    int NumCustomer;

protected:
    Customer* InsertRec(Customer* root, Customer* p) {
        if (!root) root = p;
        else if (root->CustomerResult <= p->CustomerResult)
            root->right = InsertRec(root->right, p);
        else
            root->left = InsertRec(root->left, p);
        return root;    
    }

    Customer* DeleteRec(Customer* root, const pair<int, int>& p) {
        if (!root) return root;
        if (root->CustomerResult < p.first)
            root->right = DeleteRec(root->right, p);
        else if (root->CustomerResult > p.first)
            root->left = DeleteRec(root->left, p);
        else if (root->CustomerResult == p.first && root->CustomerTime != p.second)
            root->right = DeleteRec(root->right, p);
        else {
            if (root->left == nullptr) {
                Customer* temp = root->right;
                delete root;
                return temp;
            }
            else if (root->right == nullptr) {
                Customer* temp = root->left;
                delete root;
                return temp;
            }
            else {
                Customer* need = root->right;
                while (need->left) need = need->left;
                root->CustomerResult = need->CustomerResult;
                root->CustomerTime = need->CustomerTime;
                root->right = DeleteRec(root->right, { need->CustomerResult, need->CustomerTime });
            }
        }
        return root;
    }

    void ClearRec(Customer* root) {
        if (!root) return;
        ClearRec(root->left);
        ClearRec(root->right);
        delete root;
    }

    // ham test 
    void print(Customer* root, string s) {
        if (!root) {
            cout << s << "null" << endl;
            return;
        }
        cout << s << root->CustomerResult << endl;
        s += "  ";
        s.push_back(char(179));
        print(root->left, s);
        print(root->right, s);
    }

    void inorder(Customer* root) {
        if (!root) return;
        inorder(root->left);
        cout << root->CustomerResult << '\n';
        inorder(root->right);
    }

    void posorder(Customer* root, vector<int>& arr) {
        if (!root) return;
        posorder(root->left, arr);
        posorder(root->right, arr);
        arr.push_back(root->CustomerResult);
    }

    vector<int> PostorderArray() {
        vector<int> v;
        posorder(this->root, v);
        return v;
    }

public:
    BST() : root(nullptr), NumCustomer(0) {}
    ~BST() { Clear(); }

    // xoa cay quay ve luc dau
    void Clear() {
        ClearRec(root); this->root = nullptr; NumCustomer = 0;
    }

    // them node vao cay 
    void InsertNode(Customer* a) {
        this->root = InsertRec(this->root, a);
        NumCustomer++;
    }

    // xoa mot node tren BST theo thu tu FIFO
    void DeleteNode(const pair<int, int>& p) {
        this->root = DeleteRec(this->root, p);
        NumCustomer--;
    }

    // duyet inorder
    void Inorder() { inorder(root); }

    // dem so nguoi la gian diep 
    int NumOfSpyNode() {
        if (!root) return 0;
        vector<int> arr = PostorderArray();
        int num = Help::numOfWays(arr);
        return num;
    }

    // ham test 
    void builtTree() { this->print(root, ""); }

};


// node for each position in array 
// include BST root and StackTime with pair = {result, time}
class HashNode {
private:
    BST* tree;
    queue<pair<int, int>> queueTime;
    int numOfCustomer;
public:
    HashNode() : tree(new BST()), numOfCustomer(0) {}
    ~HashNode() {
        delete tree;
        numOfCustomer = 0;
    }

    void InsertHashRegion(Customer* p) {
        queueTime.push({ p->CustomerResult, p->CustomerTime });
        numOfCustomer++;
        tree->InsertNode(p);  
    }

    // FIFO
    void Delete() {
        if (numOfCustomer == 0) return;
        pair<int, int> p = queueTime.front();
        queueTime.pop();
        tree->DeleteNode(p); 
        numOfCustomer--;
    }

    // xoa gian diep FIFO   
    void DeleteSpyPeople(int i) {
        int num = tree->NumOfSpyNode();
        //cout << "ID: " << i << " Num: " << num << endl;

        if (num > 0 && num >= numOfCustomer) { 
            tree->Clear(); 
            while (!queueTime.empty()) queueTime.pop();
            numOfCustomer = 0;
        }
        else {
            for (int i = 0; i < num; i++) {
                Delete();
            }
        }
    }

    // LIMITLESS
    void PrintInorderCustomer() { tree->Inorder(); }

    // ham test 
    void TestPrintPeople() { 
        cout << numOfCustomer << endl;
        tree->builtTree();
    }

    void TestPrintStackTime() {
        queue<pair<int, int>> q = queueTime;
        while (!q.empty()) {
            cout << q.front().first << ' ' << q.front().second << endl;
            q.pop();
        }
    }
};


// DESIGN MAIN CLASS 

// include array MAXSIZE elements Hashnode
class GojoRestaurant {
private:
    HashNode* table;
public:
    //GojoRestaurant() : table(new HashNode[MAXSIZE+1]) {}
    GojoRestaurant() : table(nullptr) {}
    ~GojoRestaurant() { delete[] table; }

    void Initial(int SIZE) {
        this->table = new HashNode[SIZE + 1];
    }
    
    void InsertCustomer(int index, Customer* cus) {
        if (index > MAXSIZE) return;
        table[index].InsertHashRegion(cus);
    }

    // FIFO
    void GetOutSpyPeople() {
        for (int i = 1; i <= MAXSIZE; i++) {
            table[i].DeleteSpyPeople(i-1);
        }
    }

    //LIMITILESS
    void PrintListCustomer(int index) {
        if (index > MAXSIZE) return;
        table[index].PrintInorderCustomer();
    }

    // ham test
    void Print() {
        for (int i = 0; i < MAXSIZE; i++) {
            cout << "region: " << i << " ";
            table[i].TestPrintPeople();
        }
    }
};

// include array MAXSIZE elements Heapnode
class SukunaRestaurant {
private:
    HeapNode* heap;
    int NumOfRegion; // so luong region dang co khach
    int TimeHeap;
protected:
    void reHeapUp(int index) {
        if (index == 0) return;
        int parent = (index - 1) / 2; 
        if (heap[parent].NumOfCustomer > heap[index].NumOfCustomer
            || (heap[parent].NumOfCustomer == heap[index].NumOfCustomer && heap[parent].TimeModified > heap[index].TimeModified)) {
            Help::swap(heap[parent], heap[index]);
            reHeapUp(parent);
        }
    }

    void reHeapDown(int index) {
        if (index < 0 || index > MAXSIZE) return;
        int l = 2 * index + 1; 
        int r = 2 * index + 2; 
        int smallest = index; 
        if (l < NumOfRegion && (heap[smallest].NumOfCustomer > heap[l].NumOfCustomer || 
            (heap[smallest].NumOfCustomer == heap[l].NumOfCustomer && heap[smallest].TimeModified > heap[l].TimeModified))) smallest = l;
        if (r < NumOfRegion && (heap[smallest].NumOfCustomer > heap[r].NumOfCustomer ||
            (heap[smallest].NumOfCustomer == heap[r].NumOfCustomer && heap[smallest].TimeModified > heap[r].TimeModified))) smallest = r;

        if (smallest != index) {
            Help::swap(heap[smallest], heap[index]);
            reHeapDown(smallest);
        }
    }

    // O(nlogn) tim khu vuc it khach va lau roi chua duoc su dung 
    int FindRegion(set<int>& s) {
        int res = 0; 
        while (res < NumOfRegion && s.find(heap[res].ID) != s.end()) res++;

        // O(nlogn)
        for (int i = res+1; i < NumOfRegion; i++) {
            // kiem tra thu da xoa hay chua 
            if (s.find(heap[i].ID) == s.end()) {
                if (heap[i].NumOfCustomer < heap[res].NumOfCustomer ||
                    (heap[i].NumOfCustomer == heap[res].NumOfCustomer && heap[i].TimeModified < heap[res].TimeModified)) {
                    res = i;
                }
            }
        }
        return res;
    }

    void Preorder(int index, int num) {
        if (index >= NumOfRegion) return;
        heap[index].PrintLIFOPeople(num);
        Preorder(2 * index + 1, num);
        Preorder(2 * index + 2, num);
    }

public: 
    SukunaRestaurant() : heap(nullptr), NumOfRegion(0), TimeHeap(0) {}

    ~SukunaRestaurant() { delete[] heap; }

    void Initial(int SIZE) {
        this->heap = new HeapNode[SIZE + 1];
        for (int i = 0; i < SIZE; i++) {
            heap[i].setID(i + 1);
        }
        //NumOfRegion = 0;
        //TimeHeap = 0;
    }
    
    void InsertCustomer(int IDregion, Customer* cus) {
        // tim vi tri cua region
        if (IDregion > MAXSIZE) return;

        int index = -1;
        for (int i = 0; i < MAXSIZE; i++) {
            if (heap[i].ID == IDregion) {
                index = i; 
                break;
            }
        }
        // hien dang ko co khach, khong nam trong heap
        if (index >= NumOfRegion) {
            if (index != NumOfRegion) {
                Help::swap(heap[index], heap[NumOfRegion]);
            }
            heap[NumOfRegion].InsertHeapRegion(++TimeHeap, cus);
            NumOfRegion++;
            reHeapUp(NumOfRegion-1);
        }
        // hien dang co khach, dang nam trong heap
        else {
            heap[index].InsertHeapRegion(++TimeHeap, cus);
            reHeapDown(index);
        }
    }

    void RemoveRegion(int index) {
        if (index >= NumOfRegion) return;
        Help::swap(heap[index], heap[NumOfRegion - 1]);
        NumOfRegion--;
        reHeapUp(index);
        reHeapDown(index); 
    }

    // O(num(nlogn + num + logn))
    void GetOutPeople(int num) {
        //if (num > MAXSIZE) num = MAXSIZE;
        set<int> s;
        for (int i = 0; i < min(num,MAXSIZE); i++) {
            int index = FindRegion(s); 
            s.insert(heap[index].ID);
            heap[index].DeleteCustomer(++TimeHeap, num);
            if (heap[index].NumOfCustomer == 0) RemoveRegion(index);
            else reHeapUp(index);
        }
    }

    //CLEAVE in thong tin theo preorder
    void PrintPreorder(int num) { Preorder(0, num); }

    // test ham 
    void PrintInfo() {
        for (int i = 0; i < MAXSIZE; i++) {
            heap[i].Testprint();
        }
    }
};

// HUFFMAN TREE
class huffmanTree {
private:
    class Node {
    public:
        int value;
        int time;
        char key;
        Node* left;
        Node* right;
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

    void EncodeChar(Node* root, string& s, map<char, string>& mp) {
        if (root->left) {
            s.push_back('0');
            EncodeChar(root->left, s, mp);
            s.pop_back();
        }
        if (root->right) {
            s.push_back('1');
            EncodeChar(root->right, s, mp);
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
     
    //void TravelRotationRec(Node*& root, int& index) {
    //    if (!root || index >= 3) return;
    //    Node* nextRoot = nullptr;
    //    //this->print(root, "");
    //    bool flag = false;
    //    while (index < 3 && RotateNode(root, nextRoot)) {
    //        flag = false; index++;
    //    }
    //    if (flag) { 
    //        index++; 
    //        TravelRotationRec(nextRoot->left, index);
    //        TravelRotationRec(nextRoot->right, index);
    //        nextRoot->height = 1 + max(getHeight(root->left), getHeight(root->right));
    //    }
    //    else {
    //        TravelRotationRec(root->left, index);
    //        TravelRotationRec(root->right, index);
    //        root->height = 1 + max(getHeight(root->left), getHeight(root->right));
    //    }
    //}

    void Travel(Node*& root, bool& check) {
        if (!root) return;
        Node* nextRoot = nullptr;
        if (RotateNode(root, nextRoot)) {
            check = true; 
            return;
        }
        else {
            if (!check) Travel(root->left, check); 
            if (!check) Travel(root->right, check);
            root->height = 1 + max(getHeight(root->left), getHeight(root->right));
        }
    }

    void TravelRotation(Node*& root) {
        if (!root) return;
        int index = 0; 
        bool check = false;
        while (index < 3) {
            Travel(root, check); 
            if (check == true) {
                index++;
                check = false;
            }
            else break;
        }
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

    void CreateTree(const vector<pair<const char, int>>& arr) {
        priority_queue<Node*, vector<Node*>, CompareNode> q;

        for (const pair<char, int>& x : arr) {
            Node* temp = new Node(x.second, 0, x.first);
            q.push(temp);
        }

        while (q.size() > 1) {
            Node* temp1 = q.top(); q.pop();
            Node* temp2 = q.top(); q.pop();
            Node* temp3 = new Node(temp1->value + temp2->value, ++Time, '#', temp1, temp2);
            // update height
            temp3->height = 1 + max(getHeight(temp1), getHeight(temp2));
            // rotate
            TravelRotation(temp3);
            // update lai time
            temp3->time = ++Time;
            q.push(temp3);
        }

        this->root = q.top();
        q.pop();
    }

    string EncodeString(const string& name) {
        map<char, string> mp;
        string s = "";
        EncodeChar(this->root, s, mp);
        if (mp.size() <= 1) return "0";

        string res = "";
        for (unsigned int i = 0; i < name.length(); i++) {
            res += mp[name[i]];
        }

        /*string result = "";
        unsigned int n = res.length();
        for (unsigned int i = n - 1; i >= 0 && i >= n - 10; i--) {
            result.push_back(res[i]);
        }*/

        string result = ""; 
        unsigned int n = res.length(); 
        if (n >= 10) result = res.substr(res.length() - 10);
        else result = res; 
        reverse(result.begin(), result.end());
        
        return result;
    }

    // in thong tin cay 
    void InorderCustomer() {
        this->InorderPrint(this->root);
    }

    // ham test
    void builtTree() { this->print(root, ""); }
};



// Casear code, huffman tree
class EncodeCustomer {
private:
    huffmanTree* tree;
    string name;
protected:

    int BinaryToDecimal(const string& code) {
        int result = 0; 
        unsigned int n = code.length() - 1;
        for (int i = n; i >= 0; i--) {
            if (code[i] == '1') result += pow(2, n - i);
        }
        return result;
    }

    vector<pair<const char,int>> CaesarEncode() {
        map<char, int> mp;
        for (char &x : this->name) mp[x]++;
        
        // khach hang chua it hon 3 ky tu khac nhau 
        if (mp.size() <= 2) {
            vector<pair<const char, int>> v;
            return v;
        }

        // chuyen doi ten khach hang 
        for (char& x : this->name) {
            if (x >= 'A' && x <= 'Z') {
                x = (x - 65 + mp[x]) % 26 + 65;
            }
            else if (x >= 'a' && x <= 'z') {
                x = (x - 97 + mp[x]) % 26 + 97;
            }
        }

        // cong don danh sach X 
        map<char, int> mp2;
        for (auto &it : mp) {
            if (it.first >= 'A' && it.first <= 'Z') {
                char x = (it.first - 65 + it.second) % 26 + 65; 
                mp2[x] += it.second;
            }
            else if (it.first >= 'a' && it.first <= 'z') {
                char x = (it.first - 97 + it.second) % 26 + 97;
                mp2[x] += it.second;
            }
        }
    
        vector<pair<const char, int>> res(mp2.begin(), mp2.end());
        return res;

    }

public:
    EncodeCustomer() : tree(nullptr), name("") {};
    ~EncodeCustomer() { delete tree; }

    int Encode(string name) {
        this->name = std::move(name);

        vector<pair<const char, int>> arr = CaesarEncode(); // thay doi this->name
        //cout << this->name << endl;
        //for (auto it : arr) cout << it.first << ' ' << it.second << endl;

        // ten khach hang co it hon 3 ky tu nen khong duoc vao
        if (arr.empty()) return -1;

        // cay huffman chi co 1 node 
        //if (arr.size() == 1) return -1;

        if (!tree) tree = new huffmanTree();
        tree->ClearTree();

        tree->CreateTree(arr);
        
        string code = tree->EncodeString(this->name);

        int result = BinaryToDecimal(code);
       
        return result;
    }

    // HAND
    void InorderPrint() { if (tree) tree->InorderCustomer(); }

    // ham test
    void TestCaesar(string s) {
        this->name = s;
        vector<pair<const char, int>> arr = CaesarEncode();
        for (const auto &it : arr) cout << it.first << ' ' << it.second << endl;
    }
};

// include G restaurent, S restaurant, time, EncodeCustomer
class Restaurant {
private:
    GojoRestaurant* gojo;
    SukunaRestaurant* sukuna;
    EncodeCustomer* block;
    int time;
public: 
    Restaurant() : gojo(new GojoRestaurant()), sukuna(new SukunaRestaurant()), block(new EncodeCustomer()), time(0) {}
    ~Restaurant() {
        delete gojo; 
        delete sukuna; 
        delete block;
        time = 0;
    }

    void Initial(int SIZE) {
        gojo->Initial(SIZE);
        sukuna->Initial(SIZE);
    }

    void LAPSE(string name) {
        int result;
        if (name[0] >= '0' && name[0] <= '9') result = stoi(name);
        else result = block->Encode(name); 
        
        //cout << result << endl;
        // khong cho vao nha hang 
        if (result == -1) return;
        //cout << "Result: " << result << endl;

        // Sukuna restaurant
        if (result % 2 == 0) {
            int ID = result % MAXSIZE + 1;
            Customer* cus = new Customer(++time, result);
            sukuna->InsertCustomer(ID, cus);
        }
        // Gojo restaurant 
        else {
            int ID = result % MAXSIZE + 1;
            Customer* cus = new Customer(++time, result);
            gojo->InsertCustomer(ID, cus);
        }
    }

    void KOKUSEN() {
        gojo->GetOutSpyPeople();
    }

    void KEITEIKEN(int num) {
        sukuna->GetOutPeople(num);
    }

    void HAND() {
        block->InorderPrint();
    }

    void LIMITLESS(int num) {
        gojo->PrintListCustomer(num);
    }

    void CLEAVE(int num) {
        sukuna->PrintPreorder(num);
    }
};

inline void simulate(string filename) {
    ifstream ss(filename);
    if (ss.fail()) {
        cout << "Not find file" << endl;
        return;
    }  
    int count = 0;
    Restaurant* r = new Restaurant();
    string str, maxsize, name, NUM;
    while (ss >> str) {
        count++;
        if (str == "MAXSIZE")
        {
            //cout << "MAXSIZE: " << count << endl;
            ss >> maxsize;   
            MAXSIZE = stoi(maxsize);
            r->Initial(MAXSIZE);
        }
        else if (str == "LAPSE") 
        {
            //cout << "LAPSE: " << count << endl;
            ss >> name;
           
            r->LAPSE(name);
        }
        else if (str == "KOKUSEN") 
        {
            //cout << "KOKUSEN: " << count << endl;
            r->KOKUSEN();
        }
        else if (str == "KEITEIKEN") 
        {
            ss >> NUM;
            //cout << "KEITEIKEN " << NUM << " : " << count << endl;
            r->KEITEIKEN(stoi(NUM));
        }
        else if (str == "HAND")
        {
            //cout << "HAND: " << count << endl;
            r->HAND();
        }
        else if (str == "LIMITLESS") 
        {
            ss >> NUM;
            //cout << "LIMITLESS " << NUM << " : " << count << endl;
            r->LIMITLESS(stoi(NUM));
        }
        else if (str == "CLEAVE") 
        {
            ss >> NUM;
            //cout << "CLEAVE " << NUM << " : " << count << endl;
            r->CLEAVE(stoi(NUM));
        }
    }
    delete r;
}


//inline void Test() {

    

    //BST* tree = new BST();
    //tree->InsertNode(new Customer(1, 10));
    //tree->InsertNode(new Customer(2, 20));
    //tree->InsertNode(new Customer(3, 30));
    //tree->InsertNode(new Customer(4, 15));
    //tree->InsertNode(new Customer(5, 50));
    //tree->InsertNode(new Customer(6, 5));
    //tree->InsertNode(new Customer(7, 3));
    //tree->InsertNode(new Customer(8, 5));
    //tree->InsertNode(new Customer(9, 7));
    //tree->InsertNode(new Customer(10, 10));
    //tree->InsertNode(new Customer(11, 20));

    //tree->builtTree();

    //tree->DeleteNode({ 10,1 });
    //tree->builtTree();   
    //
    //tree->DeleteNode({ 30,3 });
    //tree->builtTree();
    //
    //tree->DeleteNode({ 10,10 });
    //tree->builtTree();  

    //cout << tree->NumOfSpyNode();
    //delete tree; 
    
    //MAXSIZE = 5;
    //HashNode* node = new HashNode();
    //node->InsertHashRegion(new Customer(1, 71));
    //node->InsertHashRegion(new Customer(2, 377));
    //node->InsertHashRegion(new Customer(8, 149));
    //node->InsertHashRegion(new Customer(9, 23));
    //node->InsertHashRegion(new Customer(10, 657));
    //node->DeleteSpyPeople(0);
    //node->InsertHashRegion(new Customer(11, 75));
    //node->InsertHashRegion(new Customer(15, 401));
    //node->DeleteSpyPeople(0);
    //delete node;

    //ListCustomer* list = new ListCustomer(); 
    //list->InsertCustomer(new Customer(1, 10));
    //list->InsertCustomer(new Customer(2, 20));
    //list->InsertCustomer(new Customer(3, 30));
    //list->InsertCustomer(new Customer(4, 40));

    //list->InsertCustomer(new Customer(3, 30));
    //list->InsertCustomer(new Customer(4, 40));
    //list->InsertCustomer(new Customer(5, 50));
    //list->InsertCustomer(new Customer(6, 60));
    //list->InsertCustomer(new Customer(7, 70));
    //list->InsertCustomer(new Customer(8, 80));
    //list->Print();
    //cout << "------" << endl;
    //ListCustomer* list2 = new ListCustomer();
    //list2 = list;
    //list2->Print();
    //cout << "------" << endl;
    //delete list;
    //delete list2;


    //HeapNode* heap = new HeapNode();
    //heap->setID(10);
    //heap->InsertHeapRegion(new Customer(1, 10));
    //heap->InsertHeapRegion(new Customer(2, 15));
    //heap->InsertHeapRegion(new Customer(3, 20));
    //heap->InsertHeapRegion(new Customer(4, 25));
    //heap->print();

    //HeapNode* heap2 = new HeapNode();
    //heap2->InsertHeapRegion(new Customer(3, 20));
    //heap2->InsertHeapRegion(new Customer(4, 25));
    //heap2->print();
    //cout << "...." << endl;
    //Swap::swap(heap, heap2);

    //heap2->print();
    //
    //heap2->InsertHeapRegion(new Customer(12, 100));
    //cout << "..." << endl;
    //heap2->print();
    //heap2->DeleteCustomer(10);
    //cout << "sau khi xoa" << endl;
    //heap2->print();

    //delete heap;
    //delete heap2;
    //delete heap;
    /*MAXSIZE = 9;
    GojoRestaurant* a = new GojoRestaurant();
    a->Initial(9);
    a->InsertCustomer(9, new Customer(1, 71));
    a->InsertCustomer(9, new Customer(2, 377));
    a->InsertCustomer(6, new Customer(3, 149));
    a->InsertCustomer(6, new Customer(4, 23));
    a->InsertCustomer(1, new Customer(5, 657));
    a->GetOutSpyPeople();
    a->InsertCustomer(4, new Customer(6, 75));
    a->InsertCustomer(6, new Customer(7, 401));
    a->GetOutSpyPeople();*/
    /*a->Print();
    cout << "---------------------------" << endl;
    a->GetOutSpyPeople();
    a->Print();
    cout << "----------------------------" << endl;
    a->PrintListCustomer(4);*/
    

    
    /*SukunaRestaurant* a = new SukunaRestaurant();
    a->PrintInfo();
    cout << "------------------------" << endl; 
    a->InsertCustomer(2, new Customer(1, 10));
    a->InsertCustomer(5, new Customer(3, 80));
    a->InsertCustomer(7, new Customer(5, 90));
    a->InsertCustomer(7, new Customer(6, 90));
    a->InsertCustomer(7, new Customer(7, 90));
    a->InsertCustomer(4, new Customer(9, 80));
    a->PrintInfo();
    cout << "------------------------" << endl;
    a->RemoveRegion(0);
    a->RemoveRegion(1);
    a->PrintInfo();
    cout << "------------------------" << endl; 
    a->GetOutPeople(2);
    a->PrintInfo();
    delete a;*/

 /*   Restaurant* a = new Restaurant();
    delete a;*/

    //EncodeCustomer* a = new EncodeCustomer(); 
    ////cout << a->Encode("lakilhfILafdJmLDdhcFkiAfcNBFcflaAideMIhaLLlHEiMahFiEiLHLdCJiflbMaFcbaIaCfmAfLmaeFILMMcmHkDNcKjJMIIelAmNbliMOjHlbMmhKaClIdAdKmiFfbfFeCBBAhcdlKCahiAMbLAldIBDAEiCeiflDdnCmBDcFiliLLFIDaDHFbmElABmkealImFEfLkIillDamdFfBMAlAmJdAFcMDFKffHDmmeFMdDIiaocMhHLHIcDCdjmMAdLDLblCClKILIfamaickIfGmEACFmHMmBfDacMcAdilLLLaImdAFAfDacEjcfDFaFBCFMfLfMiKdHIdLhmIkCFiIaKbMInkMdiCEAcFDmhkCIikhJgFfMCeAAKAfMaHnMADKDjC");
    //cout << a->Encode("MzfFKMtOWhSQdnoFiISUtIUglRNvBLUpOPVibNieOHFO");
    ////a->InorderPrint();
    //delete a;
        
    //huffmanTree* tree = new huffmanTree();
    //vector<pair<const char, int>> arr = { {'a',1},{'b', 1},{'A',1}, {'B',2},{'c',1}, {'L',2}, {'l',2}, {'v',2}, {'t',10}, {'G',20} };
    ////vector<pair<const char, int>> arr = { {'Z',2}, {'K', 7}, {'M',24}, {'C', 32}, {'U',37}, {'D',42}, {'L',42}, {'E',120} };
    //tree->CreateTree(arr);
    //tree->builtTree();
    //tree->InorderCustomer();
    ////string a = tree.EncodeString("abL");
    ////cout << a;
    //delete tree;


//}      