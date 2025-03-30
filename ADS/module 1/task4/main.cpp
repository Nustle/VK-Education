/* 4.3. Планировщик процессов
 * В операционной системе Technux есть планировщик процессов.
 * Каждый процесс характеризуется:
 * приоритетом P
 * временем, которое он уже отработал t
 * временем, которое необходимо для завершения работы процесса T
 * Планировщик процессов выбирает процесс с минимальным значением P * (t + 1), выполняет
 * его время P и кладет обратно в очередь процессов.
 * Если выполняется условие t >= T, то процесс считается завершенным и удаляется из очереди.
 * Требуется посчитать кол-во переключений процессора.
 * Требования:
 * В качестве очереди с приоритетом нужно использовать кучу.
 * Куча должна быть реализована в виде шаблонного класса.
 * Решение должно поддерживать передачу функции сравнения снаружи.
 * Куча должна быть динамической.
 * */

#include <iostream>
#include <cassert>
#include <sstream>

using namespace std;

template <typename T>
class Vector {
public:
    Vector() {
        len = 0;
        cap = 1;
        buf = new T[cap];
    }

    ~Vector() {
        delete[] buf;
    }

    Vector(const Vector &other) {
        len = other.len;
        cap = other.cap;
        buf = other.buf;
    }

    Vector &operator=(const Vector &other) {
        if (this == &other) {
            return *this;
        }
        len = other.len;
        cap = other.cap;
        buf = new T[cap];
        for (int i = 0; i < len; i++) {
            buf[i] = other.buf[i];
        }
        return *this;
    }

    T operator[](int index) const {
        assert(index >= 0 && index < len && buf != nullptr);
        return this->buf[index];
    }

    T &operator[](int index) {
        assert(index >= 0 && index < len && buf != nullptr);
        return this->buf[index];
    }

    Vector &operator--(int) {
        assert(len > 0 && buf != nullptr);
        len--;
        return *this;
    }

    int Size() const;
    bool IsEmpty();
    void PushBack(T x);
private:
    int len;
    int cap;
    T *buf;

    void Resize();
};

template <typename T>
void Vector<T>::Resize() {
    T *new_buf = new T[cap * 2];
    for (int i = 0; i < len; i++) {
        new_buf[i] = buf[i];
    }
    delete[] buf;
    buf = new_buf;
    cap *= 2;
}

template <typename T>
int Vector<T>::Size() const {
    return len;
}

template <typename T>
bool Vector<T>::IsEmpty() {
    return len == 0;
}

template <typename T>
void Vector<T>::PushBack(T x) {
    if (len == cap) {
        Resize();
    }
    buf[len++] = x;
}

struct Process {
    Process() : p(0), t(0), T(0) {}
    Process(int p, int T) : p(p), t(0), T(T) {}
    int p, t, T;
};

istream &operator>>(istream &in, Process &process) {
    in >> process.p >> process.T;
    return in;
}

class ProcessComparator {
public:
    ProcessComparator() {}
    bool operator()(const Process &p1, const Process &p2) {
        return p1.p * (p1.t + 1) < p2.p * (p2.t + 1);
    }
};

template <typename T, typename Comparator = less<T>>
class Heap {
public:
    explicit Heap(Comparator cmp) : cmp(cmp) {}

    Heap(const Vector<T> &_arr, Comparator _cmp) {
        arr = _arr;
        cmp = _cmp;
        Heapify();
    }

    ~Heap() = default;

    Heap(const Heap<T, Comparator> &other) {
        arr = other.arr;
        cmp = other.cmp;
    }

    Heap &operator=(const Heap<T, Comparator> &other) {
        if (this == &other) {
            return *this;
        }
        arr = other.arr;
        cmp = other.cmp;
    }

    bool IsEmpty();
    void Insert(T element);
    T Extract();
    T Peek() const;
private:
    Vector<T> arr;
    Comparator cmp;

    void Heapify();
    void SiftDown(int i);
    void SiftUp(int i);
};

template <typename T, typename Comparator>
bool Heap<T, Comparator>::IsEmpty() {
    return arr.IsEmpty();
}

template <typename T, typename Comparator>
void Heap<T, Comparator>::SiftDown(int i) {
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    int largest = i;
    if (left < arr.Size() && cmp(arr[left], arr[largest])) {
        largest = left;
    }
    if (right < arr.Size() && cmp(arr[right], arr[largest])) {
        largest = right;
    }
    if (largest != i) {
        swap(arr[i], arr[largest]);
        SiftDown(largest);
    }
}

template <typename T, typename Comparator>
void Heap<T, Comparator>::SiftUp(int i) {
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (!cmp(arr[i], arr[parent])) {
            swap(arr[i], arr[parent]);
        }
        i = parent;
    }
}

template <typename T, typename Comparator>
void Heap<T, Comparator>::Heapify() {
    for (int i = arr.Size() / 2 - 1; i >= 0; i--) {
        SiftDown(i);
    }
}

template <typename T, typename Comparator>
void Heap<T, Comparator>::Insert(T element) {
    arr.PushBack(element);
    SiftUp(arr.Size() - 1);
}

template <typename T, typename Comparator>
T Heap<T, Comparator>::Extract() {
    assert(!arr.IsEmpty());
    T element = arr[0];
    swap(arr[0], arr[arr.Size() - 1]);
    arr--;
    if (!arr.IsEmpty()) {
        SiftDown(0);
    }
    return element;
}

template <typename T, typename Comparator>
T Heap<T, Comparator>::Peek() const {
    assert(!arr.IsEmpty());
    return arr[0];
}

void Run(istream &input, ostream &output) {
    int n;
    input >> n;
    Vector<Process> input_processes;
    for (int i = 0; i < n; i++) {
        Process p;
        input >> p;
        input_processes.PushBack(p);
    }
    Heap<Process, ProcessComparator> processes = Heap<Process, ProcessComparator>(input_processes, ProcessComparator());

    int switches = 0;
    while (!processes.IsEmpty()) {
        Process process = processes.Extract();
        switches++;
        process.t += process.p;
        if (process.t < process.T) {
            processes.Insert(process);
        }
    }

    output << switches << endl;
}

int main() {
    Run(cin, cout);
    return 0;
}