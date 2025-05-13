/* 4_2. Порядковые статистики
 * Дано число N и N строк. Каждая строка содержащит команду добавления или удаления
 * натуральных чисел, а также запрос на получение k-ой порядковой статистики.
 * Команда добавления числа A задается положительным числом A, команда удаления числа A
 * задается отрицательным числом “-A”. Запрос на получение k-ой порядковой статистики
 * задается числом k. Требуемая скорость выполнения запроса - O(log n).
 * */

#include <iostream>

using namespace std;

class MyComparator {
public:
    bool operator()(const int left, const int right) {
        return left < right;
    }
};

struct Node {
    explicit Node(int data) : data(data), left(nullptr), right(nullptr), height(1), count(1) {}

    int data;
    int count;
    Node *left;
    Node *right;
    size_t height;
};

template<typename Comparator>
class AVLTree {
public:
    explicit AVLTree(Comparator cmp) : root(nullptr), cmp(cmp) {}

    ~AVLTree() {
        DestroyTree(root);
    }

    int KStat(int val);
    Node* KStatInternal(Node *node, int val);

    void Insert(int data);
    void Delete(int pos);

private:
    size_t GetCount(Node *node);
    void FixCount(Node *node);
    size_t GetHeight(Node *node);
    void FixHeight(Node *node);
    int GetBalance(Node *node);
    Node *LeftRotate(Node *node);
    Node *RightRotate(Node *node);
    void DestroyTree(Node *node);
    Node *InsertInternal(Node *node, int data);
    Node *DoBalance(Node *node);
    Node *FindMin(Node *node);
    Node *RemoveMinNode(Node *node);
    Node *DeleteInternal(Node *node, int pos);

    Node *root;
    Comparator cmp;
};

template <typename Comparator>
int AVLTree<Comparator>::KStat(int val) {
    if (root == nullptr) {
        return 0;
    }
    Node *res = KStatInternal(root, val);
    if (res == nullptr) {
        return 0;
    } else {
        return res->data;
    }
}

template <typename Comparator>
Node* AVLTree<Comparator>::KStatInternal(Node *node, int val) {
    if (node == nullptr) {
        return node;
    }
    int delta = val - GetCount(node->left);
    if (delta > 0) {
        return KStatInternal(node->right, delta - 1);
    } else if (delta < 0) {
        return KStatInternal(node->left, val);
    } else {
        return node;
    }
}

template <typename Comparator>
void AVLTree<Comparator>::Insert(int data) {
    root = InsertInternal(root, data);
}

template <typename Comparator>
void AVLTree<Comparator>::Delete(int pos) {
    root = DeleteInternal(root, pos);
}

template <typename Comparator>
size_t AVLTree<Comparator>::GetCount(Node *node) {
    return node ? node->count : 0;
}

template <typename Comparator>
void AVLTree<Comparator>::FixCount(Node *node) {
    node->count = GetCount(node->left) + GetCount(node->right) + 1;
}

template <typename Comparator>
size_t AVLTree<Comparator>::GetHeight(Node *node) {
    return node ? node->height : 0;
}

template <typename Comparator>
void AVLTree<Comparator>::FixHeight(Node *node) {
    node->height = max(GetHeight(node->left), GetHeight(node->right)) + 1;
}

template <typename Comparator>
int AVLTree<Comparator>::GetBalance(Node *node) {
    return GetHeight(node->right) - GetHeight(node->left);
}

template <typename Comparator>
Node* AVLTree<Comparator>::LeftRotate(Node *node) {
    Node *tmp = node->right;
    node->right = tmp->left;
    tmp->left = node;
    FixHeight(node);
    FixCount(node);
    FixHeight(tmp);
    FixCount(tmp);
    return tmp;
}

template <typename Comparator>
Node* AVLTree<Comparator>::RightRotate(Node *node) {
    Node *tmp = node->left;
    node->left = tmp->right;
    tmp->right = node;
    FixHeight(node);
    FixCount(node);
    FixHeight(tmp);
    FixCount(tmp);
    return tmp;
}

template <typename Comparator>
void AVLTree<Comparator>::DestroyTree(Node *node) {
    if (node != nullptr) {
        DestroyTree(node->left);
        DestroyTree(node->right);
        delete node;
    }
}

template <typename Comparator>
Node* AVLTree<Comparator>::InsertInternal(Node *node, int data) {
    if (node == nullptr) {
        return new Node(data);
    }
    if (cmp(node->data, data)) {
        node->right = InsertInternal(node->right, data);
    } else {
        node->left = InsertInternal(node->left, data);
    }
    return DoBalance(node);
}

template <typename Comparator>
Node* AVLTree<Comparator>::DoBalance(Node *node) {
    FixHeight(node);
    FixCount(node);
    switch (GetBalance(node)) {
        case 2: {
            if (GetBalance(node->right) < 0) {
                node->right = RightRotate(node->right);
            }
            return LeftRotate(node);
        }
        case -2: {
            if (GetBalance(node->left) > 0) {
                node->left = LeftRotate(node->left);
            }
            return RightRotate(node);
        }
        default:
            return node;
    }
}

template <typename Comparator>
Node* AVLTree<Comparator>::FindMin(Node *node) {
    while (node->left) {
        node = node->left;
    }
    return node;
}

template <typename Comparator>
Node* AVLTree<Comparator>::RemoveMinNode(Node *node) {
    if (node->left == nullptr) {
        return node->right;
    }
    node->left = RemoveMinNode(node->left);
    return DoBalance(node);
}

template <typename Comparator>
Node* AVLTree<Comparator>::DeleteInternal(Node *node, int data) {
    if (node == nullptr) {
        return node;
    }
    if (cmp(node->data, data)) {
        node->right = DeleteInternal(node->right, data);
    } else if (cmp(data, node->data)) {
        node->left = DeleteInternal(node->left, data);
    } else {
        Node *left = node->left;
        Node *right = node->right;

        delete node;

        if (!right)
            return left;

        Node *min = FindMin(right);
        min->right = RemoveMinNode(right);
        min->left = left;

        return DoBalance(min);
    }
    return DoBalance(node);
}

int main() {
    int n;
    cin >> n;
    AVLTree<MyComparator> root((MyComparator()));
    int val, k = 0;
    for (int i = 0; i < n; i++) {
        cin >> val >> k;
        if (val >= 0) {
            root.Insert(val);
            cout << root.KStat(k) << endl;
        } else {
            root.Delete(-val);
            cout << root.KStat(k) << endl;
        }
    }
}
