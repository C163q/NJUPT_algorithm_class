/**
 * encoded in UTF-8 with BOM
 * 题目：一般背包问题
 * 参阅：
 *      语言基础补充(file://./../../reference/cpp.md#语言基础补充)
 *      迭代器(file://./../../reference/cpp.md#迭代器)
 *      基于范围的for循环(file://./../../reference/cpp.md#基于范围的for循环)
 */

#include<algorithm>
#include<iostream>
#include<vector>
#include<utility>
#include<tuple>

/**
 * 这里给出的算法的声明和实验报告里面给出的算法的声明不一样，
 * 实验报告里面给出的是void GreedyKnapsack(float* x)，
 * 但这种声明需要在全局提前定义price和weight数组，个人感觉不合适，
 * 因此改用以下定义。
 */
std::vector<double> greedy_knapsack(const std::vector<double>& price,
                                    const std::vector<double>& weight, const double volume) {
    std::vector<std::tuple<double, double, size_t>> price_and_weight;
    price_and_weight.reserve(price.size());
    for (size_t i = 0; i < price.size(); ++i) {
        price_and_weight.emplace_back(price[i], weight[i], i);
    }
    double left = volume;
    std::sort(price_and_weight.begin(), price_and_weight.end(),
              [] (const std::tuple<double, double, size_t>& lhs, const std::tuple<double, double, size_t>& rhs) {
                return std::get<0>(lhs) / std::get<1>(lhs) > std::get<0>(rhs) / std::get<1>(rhs);
              });
    std::vector<double> solve(price.size());
    for (auto&& pw : price_and_weight) {
        if (std::get<1>(pw) > left) {
            solve[std::get<2>(pw)] = left / std::get<1>(pw);
            break;
        }
        solve[std::get<2>(pw)] = 1.0;
        left -= std::get<1>(pw);
    }
    return solve;
}

// 测试函数
void test_algorithm(const std::vector<double>& price, const std::vector<double>& weight, const double volume) {
    auto&& res = greedy_knapsack(price, weight, volume);
    std::cout << "solve: (";
    for (size_t i = 0; i < res.size(); ++i) {
        std::cout << res[i];
        if (i + 1 != res.size()) {
            std::cout << ", ";
        }
    }
    std::cout << ")\n";
    double total_price = 0;
    for (size_t i = 0; i < res.size(); ++i) {
        total_price += res[i] * price[i];
    }
    std::cout << "total price: " << total_price << std::endl;
}

int main() {
    test_algorithm({ 10, 5, 15, 7, 6, 18, 3 },
                   { 2, 3, 5, 7, 1, 4, 1 },
                   15);
}

/**
 * >> solve: (1, 0.666667, 1, 0, 1, 1, 1)
 * >> total price: 55.3333
 */
