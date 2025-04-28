/* 2_2. Обход дерева в порядке pre-order
 * Дано число N < 10e6 и последовательность целых чисел из [-2e31..2e31] длиной N.
 * Требуется построить бинарное дерево, заданное наивным порядком вставки.
 * Т.е., при добавлении очередного числа K в дерево с корнем root, если root→Key ≤ K,
 * то узел K добавляется в правое поддерево root; иначе в левое поддерево root.
 * Выведите элементы в порядке pre-order (сверху вниз).
 * Рекурсия запрещена.
 * */

#include <iostream>
#include <stack>
#include <unordered_set>
#include <vector>

using namespace std;

template <typename T>
struct Node {
    explicit Node(const T &data) : data(data), left(nullptr), right(nullptr) {}

    T data;
    Node *left;
    Node *right;
};

template <typename T>
class Tree {
public:
    Tree() : root(nullptr) {}

    ~Tree() {
        vector<Node<T>* > post_ordered_nodes = PostOrderedNodes();

        for (int i = 0; i < post_ordered_nodes.size(); i++) {
            delete post_ordered_nodes[i];
        }
    }

    void Add(const T &in_data);
    void PreOrderPrint();
private:
    Node<T> *root;
    vector<Node<T>* > PostOrderedNodes();
};

template <typename T>
vector<Node<T>* > Tree<T>::PostOrderedNodes() {
    stack<Node<T>* > node_stack;
    vector<Node<T>* > post_ordered_nodes;
    unordered_set<Node<T>* > visited;

    node_stack.push(root);
    while (!node_stack.empty()) {
        Node<T> *node_top = node_stack.top();
        if ((node_top->left == nullptr || visited.find(node_top->left) != visited.end()) &&
            (node_top->right == nullptr || visited.find(node_top->right) != visited.end())
        ) {
            node_stack.pop();
            post_ordered_nodes.push_back(node_top);
            visited.insert(node_top);
        } else {
            if (node_top->right != nullptr && visited.find(node_top->right) == visited.end()) {
                node_stack.push(node_top->right);
            }
            if (node_top->left != nullptr && visited.find(node_top->left) == visited.end()) {
                node_stack.push(node_top->left);
            }
        }
    }

    return post_ordered_nodes;
}

template <typename T>
void Tree<T>::Add(const T &in_data) {
    if (root == nullptr) {
        root = new Node<T>(in_data);
        return;
    }
    Node<T> *cur_node = root;
    while (true) {
        if (in_data < cur_node->data) {
            if (cur_node->left == nullptr) {
                cur_node->left = new Node<T>(in_data);
                break;
            } else {
                cur_node = cur_node->left;
            }
        } else {
            if (cur_node->right == nullptr) {
                cur_node->right = new Node<T>(in_data);
                break;
            } else {
                cur_node = cur_node->right;
            }
        }
    }
}

template <typename T>
void Tree<T>::PreOrderPrint() {
    stack<Node<T>* > node_stack;

    node_stack.push(root);
    while (!node_stack.empty()) {
        Node<T> *node_top = node_stack.top();
        node_stack.pop();
        cout << node_top->data << " ";
        if (node_top->right != nullptr) {
            node_stack.push(node_top->right);
        }
        if (node_top->left != nullptr) {
            node_stack.push(node_top->left);
        }
    }
}

int main() {
    int n, data;
    cin >> n;
    Tree<int> tree;

    for (int i = 0; i < n; i++) {
        cin >> data;
        tree.Add(data);
    }
    tree.PreOrderPrint();
    return 0;
}