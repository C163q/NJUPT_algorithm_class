/**
 * encoded in UTF-8 with BOM
 * 题目：使用备忘录方法改写最优计算次序算法。
 * 参阅：
 *      语言基础补充(file://./../../reference/cpp.md#语言基础补充)
 */

#include<iostream>
#include<vector>
#include<utility>
#include<queue>
#include<array>
#include<functional>

/**
 * 矩阵连乘当中实验要求里面给出了两个函数：
 * MatrixChain()：用于求解矩阵连乘方法的最优解，同时给出计算次序的二维数组
 * traceback()：利用得到的计算次序的二维数组，给出计算矩阵连乘的算式
 */

// 动态规划需要使用的二维数组
using dp_map_t = std::vector<std::vector<int>>;

// 为了更方便的打印二维数组，给出了简单的std::ostream重载
std::ostream& operator<<(std::ostream& os, const dp_map_t& dp_map) {
    for (auto&& vec : dp_map) {
        os << "[";
        for (auto&& val : vec) {
            os << val << ", ";
        }
        os << "\b\b]" << std::endl;
    }
    return os;
}

int matrix_chain_impl(size_t i, size_t j, dp_map_t& calculate_map,
    dp_map_t& div_map, const std::vector<int>& dimension) {
    if (i >= j) {
        // 对角线上表示矩阵本身，断开位置就是自身的位置
        div_map[i][j] = i;
        return 0;
    }
    if (calculate_map[i][j]) {
        return calculate_map[i][j];
    }
    using namespace std::placeholders;
    // std::bind()对Args使用std::decay_t<Args>，必须使用std::ref()包装
    auto func_self = std::bind(matrix_chain_impl, _1, _2, std::ref(calculate_map),
        std::ref(div_map), std::ref(dimension));
    int min_val = func_self(i + 1, j) + dimension[i] * dimension[i + 1] * dimension[j + 1];
    size_t div = 0;
    for (size_t k = 1; k < j - i; ++k) {
        // 通过获取取各个断点时计算次数的最小值……
        int val = func_self(i, i + k) + func_self(i + k + 1, j) +
                  dimension[i] * dimension[i + k + 1] * dimension[j + 1];
        if (val < min_val) {
            min_val = val;
            div = k;
        }
    }
    // 从而得到计算m[i][j]的最小值，并记录断开位置为i + div
    div_map[i][j] = i + div;
    calculate_map[i][j] = min_val;
    return calculate_map[i][j];
}

// 计算矩阵连乘次数的函数
std::array<dp_map_t, 2> matrix_chain(const std::vector<int>& dimension) {
    if (dimension.size() <= 1) return {};   // 矩阵的个数需要大于0，如果是0或者1，无法表示一个完整的矩阵！
    // m -> calculate_map
    // s -> div_map
    // 两个二维数组的初始化，并置零
    dp_map_t calculate_map(dimension.size() - 1);
    for (auto&& vec : calculate_map) {
        vec.resize(calculate_map.size());
    }
    dp_map_t div_map(dimension.size() - 1);
    for (auto&& vec : div_map) {
        vec.resize(div_map.size());
    }
    matrix_chain_impl(0, calculate_map.size() - 1, calculate_map, div_map, dimension);
    return { calculate_map, div_map };  // 返回这两个二维数组
}

// 打印计算顺序（递归）
void traceback_impl(const dp_map_t& div_map, size_t x, size_t y) {
    if (x == y) {
        std::cout << " A" << x << ' ';  // 矩阵利用A1这种格式打印
        return;
    }
    int div = div_map[x][y];
    std::cout << "(";
    traceback_impl(div_map, x, div);
    traceback_impl(div_map, div + 1, y);
    std::cout << ")";
}

// 打印计算顺序
void traceback(const dp_map_t& div_map) {
    traceback_impl(div_map, 0, div_map.size() - 1); // 一开始要打印的范围应该是所有的矩阵
    std::cout << std::endl;
}

// 测试函数
void test_algorithm(const std::vector<int>& dim) {
    auto&& res = matrix_chain(dim);
    std::cout << "乘法次数表：" << std::endl << res[0];
    std::cout << "最少需要应用" << res[0][0][res[0].size() - 1] << "次乘法。" << std::endl;
    std::cout << "方法：" << std::endl;
    traceback(res[1]);
}

int main() {
    // 实例1：A1维数：50×10，A2维数：10×40，A3维数：40×30，A4维数：30×5
    test_algorithm({ 50, 10, 40, 30, 5 });
    // 实例2：A1维数：30×35，A2维数：35×15，A3维数：15×5，A4维数：5×10；A5维数：10×20，A6维数：20×25
    test_algorithm({ 30, 35, 15, 5, 10, 20, 25 });
}

/**
 * >> 乘法次数表：
 * >> [0, 20000, 27000, 10500]
 * >> [0, 0, 12000, 8000]
 * >> [0, 0, 0, 6000]
 * >> [0, 0, 0, 0]
 * >> 最少需要应用10500次乘法。
 * >> 方法：
 * >> ( A0 ( A1 ( A2  A3 )))
 * >> 乘法次数表：
 * >> [0, 15750, 7875, 9375, 11875, 15125]
 * >> [0, 0, 2625, 4375, 7125, 10500]
 * >> [0, 0, 0, 750, 2500, 5375]
 * >> [0, 0, 0, 0, 1000, 3500]
 * >> [0, 0, 0, 0, 0, 5000]
 * >> [0, 0, 0, 0, 0, 0]
 * >> 最少需要应用15125次乘法。
 * >> 方法：
 * >> (( A0 ( A1  A2 ))(( A3  A4 ) A5 ))
 */
