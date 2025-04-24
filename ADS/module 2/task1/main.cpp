/* 1.1 Хеш-таблица
 * Реализуйте структуру данных типа “множество строк” на основе динамической хеш-таблицы с
 * открытой адресацией. Хранимые строки непустые и состоят из строчных латинских букв.
 * Хеш-функция строки должна быть реализована с помощью вычисления значения многочлена методом Горнера.
 * Начальный размер таблицы должен быть равным 8-ми. Перехеширование выполняйте при добавлении элементов
 * в случае, когда коэффициент заполнения таблицы достигает 3/4. Структура данных должна поддерживать операции
 * добавления строки в множество, удаления строки из множества и проверки принадлежности данной строки множеству.
 *  Для разрешения коллизий используйте квадратичное пробирование.
 *  i-ая проба g(k, i) = g(k, i-1) + i (mod m). m - степень двойки.
 * */

#include <iostream>
#include <vector>
#include <string>

using namespace std;

const size_t DEFAULT_SIZE = 8;

template<typename T>
struct Node {
    Node() : key(), status(Status::NIL) {}

    explicit Node(const T &key) : key(key), status(Status::KEY) {}

    enum class Status {
        KEY,
        DEL,
        NIL
    };

    Status status;
    T key;
};

class StringHasher {
public:
    explicit StringHasher(size_t prime = 71) : prime(prime) {}

    size_t operator()(const string &str) const {
        size_t hash = 0;
        for (char ch : str) {
            hash = hash * prime + ch;
        }
        return hash;
    }
private:
    size_t prime;
};

template <typename T, typename Hasher = hash<T>>
class Set {
public:
    Set(size_t initial_size = DEFAULT_SIZE, Hasher hasher= Hasher())
    : size(0), set(initial_size), hasher(hasher) {}

    bool Has(const T &key);
    bool Add(const T &key);
    bool Delete(const T &key);
private:
    vector<Node<T>> set;
    size_t size;
    Hasher hasher;
    void Grow();
};

template <typename T, typename Hasher>
void Set<T, Hasher>::Grow() {
    vector<Node<T>> rehashed_set(2 * set.size());
    for (int i = 0; i < set.size(); i++) {
        Node<T> node = set[i];
        if (node.status != Node<T>::Status::KEY) {
            continue;
        }
        int rehashed_prob = hasher(node.key) % rehashed_set.size();
        for (int j = 0; i < rehashed_set.size(); j++) {
            rehashed_prob = (rehashed_prob + j) % rehashed_set.size();
            if (rehashed_set[rehashed_prob].status == Node<T>::Status::NIL) {
                rehashed_set[rehashed_prob] = node;
                break;
            }
        }
    }
    swap(set, rehashed_set);
}

template <typename T, typename Hasher>
bool Set<T, Hasher>::Has(const T &key) {
    int prob = hasher(key) % set.size();
    for (int i = 0; i < set.size(); i++) {
        prob = (prob + i) % set.size();
        if (set[prob].status == Node<T>::Status::NIL) {
            return false;
        } else if (set[prob].status == Node<T>::Status::KEY && set[prob].key == key) {
            return true;
        }
    }
    return false;
}

template <typename T, typename Hasher>
bool Set<T, Hasher>::Add(const T &key) {
    if (4 * size > 3 * set.size()) {
        Grow();
    }

    int prob = hasher(key) % set.size();
    int del_prob = -1;
    for (int i = 0; i < set.size(); i++) {
        prob = (prob + i) % set.size();
        if (set[prob].status == Node<T>::Status::NIL) {
            if (del_prob != -1) {
                prob = del_prob;
            }
            set[prob] = Node<T>(key);
            size++;
            return true;
        } else if (del_prob == -1 && set[prob].status == Node<T>::Status::DEL) {
            del_prob = prob;
        } else if (set[prob].status == Node<T>::Status::KEY && set[prob].key == key) {
            return false;
        }
    }
    set[del_prob] = Node<T>(key);
    size++;
    return true;
}

template <typename T, typename Hasher>
bool Set<T, Hasher>::Delete(const T &key) {
    int prob = hasher(key) % set.size();
    for (int i = 0; i < set.size(); i++) {
        prob = (prob + i) % set.size();
        if (set[prob].status == Node<T>::Status::NIL) {
            return false;
        } else if (set[prob].status == Node<T>::Status::KEY && set[prob].key == key) {
            set[prob].status = Node<T>::Status::DEL;
            size--;
            return true;
        }
    }
    return false;
}

int main() {
    Set<string, StringHasher> set;

    char op;
    string key;

    while (cin >> op >> key) {
        switch (op) {
            case '?':
                cout << (set.Has(key) ? "OK" : "FAIL") << endl;
                break;
            case '+':
                cout << (set.Add(key) ? "OK" : "FAIL") << endl;
                break;
            case '-':
                cout << (set.Delete(key) ? "OK" : "FAIL") << endl;
                break;
        }
    }
    return 0;
}