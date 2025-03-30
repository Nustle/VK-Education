/* 3.2. Дек с динамическим буфером
 * Реализовать дек с динамическим зацикленным буфером.
 * Обрабатывать команды push * и pop *.
 * Формат ввода
 * В первой строке количество команд n. n ≤ 1000000.
 * Каждая команда задаётся как 2 целых числа: a b.
 * a = 1 - push front
 * a = 2 - pop front
 * a = 3 - push back
 * a = 4 - pop back
 * Если дана команда pop *, то число b - ожидаемое значение.
 * Если команда pop * вызвана для пустой структуры данных, то ожидается “-1”.
 * */

#include <iostream>

using namespace std;

class Dequeue {
public:
    Dequeue() {
        cap = 1;
        head = 0;
        tail = 0;
        buf = new int[cap];
    }

    ~Dequeue() {
        delete[] buf;
    }

    Dequeue(const Dequeue& other) {
        buf = new int[other.cap];
        for (int i = other.head; i != other.tail; i = (i + 1) % other.cap) {
            buf[i] = other.buf[i];
        }
        cap = other.cap;
        head = other.head;
        tail = other.tail;
    }

    Dequeue& operator=(const Dequeue &other) {
        buf = new int[other.cap];
        for (int i = other.head; i != other.tail + 1; i = (i + 1) % other.cap) {
            buf[i] = other.buf[i];
        }
        cap = other.cap;
        head = other.head;
        tail = other.tail;
        return *this;
    }

    int Size() const {
        if (tail < head) {
            return cap - head + tail;
        } else {
            return tail - head;
        }
    }

    bool IsEmpty() const {
        return Size() == 0;
    }

    void PushFront(int number) {
        if (head == (tail + 1) % cap) {
            Resize();
        }
        head = (head - 1 + cap) % cap;
        buf[head % cap] = number;
    }

    int PopFront() {
        int element = buf[head];
        head = (head + 1) % cap;
        return element;
    }

    void PushBack(int number) {
        if (head == (tail + 1) % cap) {
            Resize();
        }
        buf[tail] = number;
        tail = (tail + 1) % cap;
    }

    int PopBack() {
        tail = (tail - 1 + cap) % cap;
        int element = buf[tail];
        return element;
    }

private:
    int cap;
    int head;
    int tail;
    int *buf;

    void Resize() {
        int *new_buf = new int[cap * 2];
        for (int i = 0; i < cap - 1; i++) {
            new_buf[i] = buf[head];
            head = (head + 1) % cap;
        }
        delete[] buf;
        head = 0;
        tail = cap - 1;
        cap *= 2;
        buf = new_buf;
    }
};

void Run(istream &input, ostream &output) {
    int n = 0;
    input >> n;
    Dequeue deq;
    for (int i = 0; i < n; i++) {
        int command = 0;
        int input_value = 0;
        input >> command >> input_value;
        int result;
        switch (command) {
            case 2:
                if (deq.IsEmpty()) {
                    result = -1;
                } else {
                    result = deq.PopFront();
                }
                if (result != input_value) {
                    output << "NO" << endl;
                    return;
                }
                break;
            case 4:
                if (deq.IsEmpty()) {
                    result = -1;
                } else {
                    result = deq.PopBack();
                }
                if (result != input_value) {
                    output << "NO" << endl;
                    return;
                }
                break;
            case 1:
                deq.PushFront(input_value);
                break;
            case 3:
                deq.PushBack(input_value);
                break;
            default:
                output << "NO" << endl;
        }
    }
    output << "YES" << endl;
}

int main() {

    Run(cin, cout);
    return 0;
}