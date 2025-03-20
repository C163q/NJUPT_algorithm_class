/**
 * encoded in UTF-8 with BOM
 * 题目：写一个递归算法和一个迭代算法计算二项式系数：...
 * 参阅：无
 */

#include<iostream>

// solution（递归）
unsigned binomial_coefficient_1(unsigned m, unsigned n) {
    if (m == n || !m) return 1;
    return binomial_coefficient_1(m, n - 1) + binomial_coefficient_1(m - 1, n - 1);
}

// solution（迭代）
unsigned binomial_coefficient_2(unsigned m, unsigned n) {
    unsigned factorial_n = 1, factorial_m = 1, factorial_n_m = 1;
    for (unsigned i = 2; i <= n; ++i) factorial_n *= i;
    for (unsigned i = 2; i <= m; ++i) factorial_m *= i;
    for (unsigned i = 2; i <= n - m; ++i) factorial_n_m *= i;
    return factorial_n / factorial_m / factorial_n_m;
}

int main() {
    int m, n;
    std::cin >> m >> n;
    std::cout << "Solution 1: " << binomial_coefficient_1(m, n) << std::endl
              << "Solution 2: " << binomial_coefficient_2(m, n) << std::endl;
}

/**
 * << 1 1
 * >> Solution 1: 1
 * >> Solution 2: 1
 * 
 * << 5 10
 * >> Solution 1: 252
 * >> Solution 2: 252
 * 
 * << 4 12
 * >> Solution 1: 495
 * >> Solution 2: 495
 */

