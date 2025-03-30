/** 2.2. Унимодальная последовательность
 * Дан массив целых чисел А[0..n-1]. Известно, что на интервале [0, m] значения массива
 * строго возрастают, а на интервале [m, n-1] строго убывают. Найти m за O(log(m)).
 * 2 ≤ n ≤ 10000.
 * **/

#include <iostream>

using namespace std;

int binarySearch(const int A[], int n, int left, int right) {
    while (left < right) {
        int mid = (left + right) / 2;
        if ((mid == 0 || A[mid - 1] < A[mid]) && (mid == n - 1 || A[mid] < A[mid + 1])) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }
    return left;
}

int expSearch(const int A[], int n) {
    int left = 0;
    int right = 1;

    while (right + 1 < n && A[right] < A[right + 1]) {
        left = right;
        right *= 2;
    }
    right = (right >= n) ? n - 1 : right;
    return binarySearch(A, n, left, right);
}

int main() {
    int n;
    cin >> n;
    int *A = new int[n];
    for (int i = 0; i < n; i++) {
        cin >> A[i];
    }
    int ans = expSearch(A, n);
    cout << ans;
    delete[] A;
}