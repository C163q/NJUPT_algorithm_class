/**
 * encoded in UTF-8 with BOM
 * 题目：试给出n!的递归定义式，并设计一个递归函数计算n!。
 * 参阅：无
 */

#include<iostream>

// solution
unsigned factorial(unsigned n) {
    if (n < 2) return 1;
    return n * factorial(n - 1);
}

int main() {
    int n;
    std::cin >> n;
    std::cout << n << "! = " << factorial(n) << std::endl;
}

/**
 * << 0
 * >> 0! = 1
 * 
 * << 1
 * >> 1! = 1
 * 
 * << 2
 * >> 2! = 2
 * 
 * << 3
 * >> 3! = 6
 * 
 * << 10
 * >> 10! = 3628800
 */

