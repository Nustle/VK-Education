/** 1.1. Количество бит
 * Подсчитать кол-во единичных бит в входном числе,
 * стоящих на четных позициях. Позиции битов нумеруются с 0.
 * Необходимо использование битовых операций.
 * Использование арифметических операций запрещено.
 * **/

#include <iostream>

using namespace std;

unsigned int solve(unsigned int number) {
    unsigned int ans = 0;
    while (number | 0) {
        ans += number & 1;
        number >>= 2;
    }
    return ans;
}

int main() {
    unsigned int number;
    cin >> number;
    cout << solve(number);
}