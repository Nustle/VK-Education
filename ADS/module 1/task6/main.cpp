/* 6.4 Порядковая статистика и параметры множества
 * Дано множество целых чисел из [0..10e9] размера n.
 * Используя алгоритм поиска k-ой порядковой статистики, требуется найти следующие параметры множества:
 * 10% перцентиль
 * медиана
 * 90% перцентиль
 * Требования:
 * К дополнительной памяти: O(n).
 * Среднее время работы: O(n)
 * Должна быть отдельно выделенная функция partition.
 * Рекурсия запрещена.
 * Решение должно поддерживать передачу функции сравнения снаружи.
 * Стратегия выбора опорного элемента: “случайный элемент”.
 * Функцию Partition должна быть реализована методом прохода двумя итераторами от конца массива к началу.
 * */

#include <iostream>
#include "random"

using namespace std;

int Random(int i, int j) {
    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<> dist(i, j);
    return dist(gen);
}

template <typename T, typename Comparator = less<T>>
int Partition(T *arr, int left, int right, int n, Comparator cmp = Comparator()) {
    int r = Random(left, right);
    swap(arr[left], arr[r]);
    int pivot = left;
    int i = right;
    for (int j = right; j > left; j--) {
        if (cmp(arr[pivot], arr[j])) {
            swap(arr[i], arr[j]);
            i--;
        }
    }
    swap(arr[i], arr[pivot]);
    return i;
}

template <typename T, typename Comparator = less<T>>
T QuickSelect(T *arr, int n, int k, Comparator cmp = Comparator()) {
    int left = 0;
    int right = n - 1;
    int pivot_pos = Partition(arr, left, right, n, cmp);
    while (pivot_pos != k) {
        if (pivot_pos > k) {
            right = pivot_pos - 1;
        } else {
            left = pivot_pos + 1;
        }
        pivot_pos = Partition(arr, left, right, n, cmp);
    }
    return arr[k];
}

void Run(istream &input, ostream &output) {
    int n;
    input >> n;
    int *arr = new int[n];
    for (int i = 0; i < n; i++) {
        input >> arr[i];
    }
    int percentile_10 = QuickSelect(arr, n, int(n * 0.1));
    int median = QuickSelect(arr, n, int(n * 0.5));
    int percentile_90 = QuickSelect(arr, n, int(n * 0.9));
    output << percentile_10 << endl;
    output << median << endl;
    output << percentile_90 << endl;
    delete[] arr;
}

int main() {
    Run(cin, cout);
    return 0;
}
