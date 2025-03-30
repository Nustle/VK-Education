/* 5.4. Закраска прямой в один слой
 * На числовой прямой окрасили N отрезков.
 * Известны координаты левого и правого концов каждого отрезка [Li, Ri].
 * Найти сумму длин частей числовой прямой, окрашенных ровно в один слой.
 * N ≤ 10000. Li, Ri — целые числа в диапазоне [0, 10e9].
 * */

#include <iostream>
#include <cstring>

using namespace std;

struct Point {
    Point() : x(0), delta(0) {}
    int x, delta;
};

istream &operator>>(istream &in, Point &point) {
    in >> point.x;
    return in;
}

ostream &operator<<(ostream &out, Point &point) {
    out << "(" << point.x << ", " << point.delta << ")";
    return out;
}

class PointComparator {
public:
    PointComparator() {}
    bool operator()(const Point &p1, const Point &p2) {
        return p1.x < p2.x;
    }
};

template <typename T, typename Comparator>
void Merge(T *arr, int left, int mid, int right, Comparator cmp) {
    int buf_size = right - left + 1;
    int offset = left;
    T *buf = new T[buf_size];
    int left_second = mid + 1;
    int index = 0;

    while (left <= mid && left_second <= right) {
        if (cmp(arr[left_second], arr[left])) {
            buf[index] = arr[left_second++];
        } else {
            buf[index] = arr[left++];
        }
        index++;
    }

    int left_end = (left_second > right) ? left : left_second;
    for (int i = index; i < buf_size; i++) {
        buf[i] = arr[left_end++];
    }
    memcpy(arr + offset, buf, sizeof(T) * buf_size);
    delete[] buf;
}

template <typename T, typename Comparator>
void MergeSortRec(T *arr, int left, int right, Comparator cmp) {
    if (left == right) {
        return;
    }
    int mid = (left + right) / 2;
    MergeSortRec(arr, left, mid, cmp);
    MergeSortRec(arr, mid + 1, right, cmp);
    Merge(arr, left, mid, right, cmp);
}

template <typename T, typename Comparator = less<T>>
void MergeSort(T *arr, int len, Comparator cmp = Comparator()) {
    MergeSortRec(arr, 0, len - 1, cmp);
}

void Run(istream &input, ostream &output) {
    int n;
    input >> n;
    int len = 2 * n;
    Point *arr = new Point[len];
    int j = 0;
    for (int i = 0; i < n; i++, j++) {
        input >> arr[i + j] >> arr[i + j + 1];
        arr[i + j].delta = 1;
        arr[i + j + 1].delta = -1;
    }
    MergeSort(arr, len, PointComparator());
    int colored_sum = 0;
    int balance = 0;
    for (int i = 0; i < len; i++) {
        Point p = arr[i];
        int peek = balance + p.delta;
        if (balance == 0 && peek == 1 || balance == 2 && peek == 1) {
            colored_sum -= p.x;
        } else if (balance == 1 && peek == 2 || balance == 1 && peek == 0) {
            colored_sum += p.x;
        }
        balance += p.delta;
    }
    delete[] arr;
    output << colored_sum << endl;
}

int main() {
    Run(cin, cout);
    return 0;
}