/**
 * encoded in UTF-8 with BOM
 * 题目：给定正整数n（其有效位数<=240），删除其中任意k个数字后，使得剩下的数字按原次序组成的新的正整数最大或是最小。
 * 参阅：
 *      语言基础补充(file://./../../reference/cpp.md#语言基础补充)
 */

#include<iostream>
#include<string>
#include<exception>
#include<random>
#include<time.h>

/**
 * 题目要求实现以下两个方法：
 * int MinNum(string number, int k);删除k个数字后，使得剩下的数字最小。
 * int MaxNum(string number, int k);删除k个数字后，使得剩下的数字最大。
 * 考虑到删除后的数字不一定能够用int容纳下来，因此修改为返回string。
 */

std::string MinNum(std::string number, int k) {
    if (number.size() < k) throw std::invalid_argument("k cannot be greater than number.length()");
    int i = 0;
    while (k > 0) {
        while (i < number.size() - 1 && number[i] <= number[i + 1]) {
            ++i;
        }
        if (i + 1 < number.size()) {
            number.erase(i, 1);
            --k;
            --i;
            if (i < 0) {
                i = 0;
            }
        }
        else break;
    }
    if (k > 0) {
        number.erase(number.size() - k, k);
    }
    int leading_zeros = 0;
    while (leading_zeros < number.size() - 1 && number[leading_zeros] == '0') {
        ++leading_zeros;
    }
    number.erase(0, leading_zeros);
    return number;
}

std::string MaxNum(std::string number, int k) {
    if (number.size() < k) throw std::invalid_argument("k cannot be greater than number.length()");
    int i = 0;
    while (k > 0) {
        while (i < number.size() - 1 && number[i] >= number[i + 1]) {
            ++i;
        }
        if (i + 1 < number.size()) {
            number.erase(i, 1);
            --k;
            --i;
            if (i < 0) {
                i = 0;
            }
        }
        else break;
    }
    if (k > 0) {
        number.erase(number.size() - k, k);
    }
    return number;
}

std::default_random_engine re(static_cast<std::default_random_engine::result_type>(time(nullptr)));
// 生成随机的数字字符串
std::string yield_random_number() {
    std::uniform_int_distribution<char> digit('0', '9');
    std::uniform_int_distribution<int> ud(10, 240);
    (void) ud(re);
    int total_size = ud(re);
    std::string number;
    number.reserve(total_size);
    for (int i = 0; i < total_size; ++i) {
        if (!i) {
            char first_digit = 0;
            do {
                first_digit = digit(re);
            } while (first_digit == '0');
            number.push_back(first_digit);
        }
        else number.push_back(digit(re));
    }
    return number;
}

// 测试函数
void test_algorithm() {
    std::cout << "original number: ";
    std::string num = yield_random_number();
    std::cout << num << std::endl;
    int k = std::uniform_int_distribution<int>(2, num.size() - 5)(re);
    std::cout << "remove " << k << " digits" << std::endl;
    std::cout << "minimum number: " << MinNum(num, k) << std::endl;
    std::cout << "maximum number: " << MaxNum(num, k) << std::endl;
}

int main() {
    test_algorithm();
}

/**
 * 可能的输出：
 * >> original number: 831660222208226036167229618526639298268037604764441185436991109143182021488853881624581
 * >> remove 68 digits
 * >> minimum number: 143881624581
 * >> maximum number: 9999998888881624581
 */
