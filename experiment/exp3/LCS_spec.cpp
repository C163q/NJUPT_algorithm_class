/**
 * encoded in UTF-8 with BOM
 * 题目：给定两个字符序列X={x1,x2,……,xm}和Y={y1,y2,……,yn}，要求找出A和B的一个最长公共子序列。
 * 参阅：
 *      语言基础补充(file://./../../reference/cpp.md#语言基础补充)
 */

#include<iostream>
#include<string>
#include<vector>
#include<utility>
#include<queue>

/**
 * 实验要求中要求定义LCS类，用以计算最长公共子序列长度和最长公共子序列。
 * 实验要求中是这么描述LCS类的成员的：
 * LCS构造函数：用以让用户传入字符串，求出这两个字符串的最长公共子序列。
 * LCSLength()方法：用以求出最长公共子序列的长度，并返回该长度。
 * CLCS()方法：用以输出具体的子序列。
 * -----------------------------------------------------------
 * 个人认为既然在求解LCSLength()时，已经能够求出具体的子序列了，为何要把两者的逻辑分开？
 * 因此，此处我虽然实现了这两个方法，但吧这两个方法都作为私有成员，并由calculate()方法调用。
 * 该方法直接返回最后的子序列std::deque，至于子序列长度，使用std::deque的size()方法即可。
 */
class LCS {
public:
    using dp_map_t = std::vector<std::vector<int>>; // 这是动态规划要用的二维数组
    using LCS_return_t = std::deque<char>;  // 子序列的结果保存在一个std::deque中
public:
    // 构造函数，需要传入的是待求最大公共子序列的两个字符串
    LCS(const std::string& x, const std::string& y) : x(x), y(y) {}
    std::pair<const std::string&, const std::string&> get_seq() const noexcept {
        return { x, y };    // 如果要获取原序列的话，就用这个函数访问。
    }
    const LCS_return_t& calculate() {   // 这个方法就是简单的调用了LCSLength()和CLCS()
        if (calculator.empty()) {   // 如果还未曾求解过LCS的值，才调用
            LCSLength();
            CLCS();
        }
        return result;  // 否则直接返回存储在类中的结果
    }

private:
    void LCSLength() {  // 动态规划的核心代码
        // 二维数组的初始化，这段代码只会调用一次
        calculator.resize(x.size() + 1);
        for (auto&& vec : calculator) {
            vec.resize(y.size() + 1);
        }
        path.resize(x.size() + 1);
        for (auto&& vec : path) {
            vec.resize(y.size() + 1);
        }
        // 默认初始化后数组内全为0
        for (size_t i = 1; i <= x.size(); ++i) {
            for (size_t j = 1; j <= y.size(); ++j) {
                if (x[i - 1] == y[j - 1]) { // 由c[i-1][j-1]得到c[i][j]
                    calculator[i][j] = calculator[i - 1][j - 1] + 1;
                    path[i][j] = 1;
                }
                else if (calculator[i][j - 1] <= calculator[i - 1][j]) {    // 由c[i-1][j]得到c[i][j]
                    calculator[i][j] = calculator[i - 1][j];
                    path[i][j] = 2;
                }
                else {  // 由c[i][j-1]得到c[i][j]
                    calculator[i][j] = calculator[i][j - 1];
                    path[i][j] = 3;
                }
            }
        }
        // 此处没有像实验要求里面一样返回最优解，直接通过最终的返回序列也可以求出长度。
    }

    void CLCS() {
        size_t i = x.size();
        size_t j = y.size();
        while (i && j) {
            if (path[i][j] == 1) {  // 如果满足该条件，表示是最有解中的一个元素
                result.push_front(x[i - 1]);    // 由于是逆序查找的，所以向前插入
                --i;
                --j;
            }
            else if (path[i][j] == 2) { // 向上查找
                --i;
            }
            else if (path[i][j] == 3) { // 向左查找
                --j;
            }
            else {  // 这段代码不会被执行到
                break;
            }
        }
    }

private:
    std::string x;  // 两个待求解的字符串
    std::string y;
    dp_map_t calculator{};
    dp_map_t path{};
    LCS_return_t result{};  // 最优解
};

// 测试函数
void test_algorithm(const std::string& x, const std::string& y) {
    LCS test_val(x, y);
    auto&& res = test_val.calculate();
    std::cout << "序列 X=(";
    for (char c : x) {
        std::cout << c << ", ";
    }
    std::cout << "\b\b) 和 Y=(";
    for (char c : y) {
        std::cout << c << ", ";
    }
    std::cout << "\b\b) 的最长公共子序列为：\n(";
    for (char c : res) {
        std::cout << c << ", ";
    }
    std::cout << "\b\b)" << std::endl;
    std::cout << "长度为：" << res.size() << std::endl;
}

int main() {
    test_algorithm("abcbdab", "bdcaba");
}

/**
 * >> 序列 X=(a, b, c, b, d, a, b) 和 Y=(b, d, c, a, b, a) 的最长公共子序列为：
 * >> (b, c, b, a) 
 * >> 长度为：4
 */
