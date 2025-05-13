/* 3_2. B-дерево
 * Постройте B-дерево минимального порядка t и выведите его по слоям.
 * В качестве ключа используются числа, лежащие в диапазоне [0..2e32-1]
 * Требования:
 * B-дерево должно быть реализовано в виде шаблонного класса.
 * Решение должно поддерживать передачу функции сравнения снаружи.
 * */

#include <iostream>
#include <vector>
#include <queue>

using namespace std;

template<typename T>
class MyComparator {
public:
    bool operator()(const T &left, const T &right) {
        return left < right;
    }
};

template<typename T, typename Comparator>
class BTree {
public:
    struct Node {
        explicit Node(bool leaf) : leaf(leaf) {}

        ~Node() {
            for (Node *child: children) {
                delete child;
            }
        }

        bool leaf;
        vector<T> keys;
        vector<Node *> children;
    };

    explicit BTree(size_t min_degree, Comparator cmp) : t(min_degree), cmp(cmp), root(nullptr) {}

    ~BTree() {
        delete root;
    }

    void Insert(const T &key);
    void BFSPrint();

private:
    void BFSPrintInternal(Node *node);
    bool IsNodeFull(Node *node);
    void SplitChild(Node *cur_root, Node *node, size_t index);
    void InsertNonFull(Node *node, const T &key);

    size_t t;
    Node *root;
    Comparator cmp;
};

template <typename T, typename Comparator>
void BTree<T, Comparator>::Insert(const T &key) {
    if (root == nullptr) {
        root = new Node(true);
    }

    if (IsNodeFull(root)) {
        Node *new_root = new Node(false);
        new_root->children.push_back(root);
        root = new_root;
        SplitChild(root, root->children[0], 0);
    }

    InsertNonFull(root, key);
}

template <typename T, typename Comparator>
void BTree<T, Comparator>::BFSPrint() {
    BFSPrintInternal(root);
}

template <typename T, typename Comparator>
void BTree<T, Comparator>::BFSPrintInternal(Node *node) {
    queue<pair<Node *, int>> q;
    q.push(pair<Node *, int>(node, 0));
    int prev_level = 0;
    while (!q.empty()) {
        Node *cur_node = q.front().first;
        int cur_level = q.front().second;
        if (cur_level != prev_level) {
            cout << endl;
        }
        q.pop();
        for (auto it = cur_node->keys.begin(); it != cur_node->keys.end(); it++) {
            cout << (*it) << " ";
        }
        for (auto child: cur_node->children) {
            q.push(pair<Node *, int>(child, cur_level + 1));
        }
        prev_level = cur_level;
    }
}

template <typename T, typename Comparator>
bool BTree<T, Comparator>::IsNodeFull(Node *node) {
    return node->keys.size() == 2 * t - 1;
}

template <typename T, typename Comparator>
void BTree<T, Comparator>::SplitChild(BTree::Node *cur_root, BTree::Node *node, size_t index) {
    Node *new_child = new Node(node->leaf);
    new_child->keys = vector<T>(t - 1);
    for (size_t i = 0; i < t - 1; i++) {
        new_child->keys[i] = node->keys[i + t];
    }
    if (node->leaf == false) {
        new_child->children = vector<Node *>(t);
        for (size_t i = 0; i < t; i++) {
            new_child->children[i] = node->children[i + t];
        }
        node->children = vector<Node *>(node->children.begin(), node->children.begin() + t);
    }
    auto count_keys = cur_root->keys.size();
    node->keys = vector<T>(node->keys.begin(), node->keys.begin() + t);
    cur_root->children.resize(cur_root->children.size() + 1);
    for (int j = count_keys; j >= index + 1; j--) {
        cur_root->children[j + 1] = cur_root->children[j];
    }
    cur_root->children[index + 1] = new_child;
    cur_root->keys.resize(cur_root->keys.size() + 1);
    for (int j = count_keys - 1; j >= int(index); j--) {
        cur_root->keys[j + 1] = cur_root->keys[j];
    }
    cur_root->keys[index] = node->keys[t - 1];
    node->keys = vector<T>(node->keys.begin(), node->keys.begin() + t - 1);
}

template <typename T, typename Comparator>
void BTree<T, Comparator>::InsertNonFull(BTree::Node *node, const T &key) {
    int pos = node->keys.size() - 1;
    if (node->leaf) {
        node->keys.resize(node->keys.size() + 1);
        while (pos >= 0 && cmp(key, node->keys[pos])) {
            node->keys[pos + 1] = node->keys[pos];
            pos--;
        }
        node->keys[pos + 1] = key;
    } else {
        while (pos >= 0 && cmp(key, node->keys[pos])) {
            pos--;
        }
        if (IsNodeFull(node->children[pos + 1])) {
            SplitChild(node, node->children[pos + 1], pos + 1);
            if (!cmp(key, node->keys[pos + 1])) {
                pos++;
            }
        }
        InsertNonFull(node->children[pos + 1], key);
    }
}

int main() {
    size_t t = 0;
    cin >> t;
    BTree<int, MyComparator<int>> tree(t, MyComparator<int>());
    int val;
    while (cin >> val) {
        tree.Insert(val);
    }
    tree.BFSPrint();
    return 0;
}