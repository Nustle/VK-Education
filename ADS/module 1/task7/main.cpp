/* 7.3. Binary MSD для long long
 * Дан массив неотрицательных целых 64-разрядных чисел.
 * Количество чисел не больше 1000000.
 * Отсортировать массив методом MSD по битам (бинарный QuickSort).
 * */

#include <iostream>

using namespace std;

int Partition(long long *arr, int left, int right, int rank) {
    int i = left;
    for (int j = left; j < right; j++) {
        if ((arr[j] >> rank) % 2 == 0) {
            swap(arr[j], arr[i]);
            i++;
        }
    }
    return i;
}

void QuickSortBinaryRec(long long *arr, int left, int right, int rank) {
    if (rank < 0 || left == right) {
        return;
    }
    int one_bit_start = Partition(arr, left, right, rank--);
    QuickSortBinaryRec(arr, left, one_bit_start, rank);
    QuickSortBinaryRec(arr, one_bit_start, right, rank);
}

void QuickSortBinary(long long *arr, int n) {
    QuickSortBinaryRec(arr, 0, n, 63);
}


void Run(istream &input, ostream &output) {
    int n;
    input >> n;
    long long *arr = new long long[n];
    for (int i = 0; i < n; i++) {
        input >> arr[i];
    }
    QuickSortBinary(arr, n);
    for (int i = 0; i < n; i++) {
        output << arr[i] << " ";
    }
    delete[] arr;
}

int main() {
    Run(cin, cout);
    return 0;
}
