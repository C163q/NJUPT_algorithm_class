/**
 * encoded in UTF-8 with BOM
 * 题目：用表格的形式写出快速排序的执行过程。
 * 参阅：无
 */

#include<iostream>
#include<algorithm>
#include<iterator>
#include<utility>
#include<set>
#include<iomanip>
#include<vector>

// 由于这道题不要求写代码，因此不作标准库的使用限制。
// 以下程序用于自动生成快速排序的过程的序列。

template<typename RanIt>
RanIt partition(RanIt left, RanIt right) {  // 就是书上普通的partition
    auto begin = left;
    auto end = right;
    do {
        do ++begin; while (begin < right && *begin < *left);
        do --end; while (*end > *left);
        if (begin < end) std::swap(*begin, *end);
    } while (begin < end);
    std::swap(*end, *left);
    return end;
}

template<typename RanIt>
void div_print(RanIt left, RanIt right, std::set<RanIt>& divide) {
    std::cout.setf(std::ios::right);
    auto it = divide.begin();
    auto ptr = left;
    bool left_bracket = false;
    for (; ptr < right; ++ptr) {
        if (!left_bracket) {    // 左括号还没有或者前面的左右括号都匹配了
            if (*it == ptr) {   // 是分隔元素，因此不需要加左括号
                std::cout << "  " << std::setw(6) << *ptr;
                ++it;   // 下一个分隔元素
            }
            else {
                std::cout << " (" << std::setw(6) << *ptr;
                left_bracket = true;
            }
        }
        else {  // 有未配对的左括号
            if (*it == ptr) {   // 是分隔元素，因此前一个元素要加右括号
                std::cout << ") " << std::setw(6) << *ptr;
                left_bracket = false;
                ++it;
            }
            else {
                std::cout << "  " << std::setw(6) << *ptr;
            }
        }
    }
    if (left_bracket) std::cout << ")"; // 括号未闭合
    else std::cout << " ";
}

template<typename RanIt>
int quick_sort_with_print(RanIt left, RanIt right,  // right: 超尾
                          int level,                // level: 执行partition的次数
                          const std::pair<RanIt, RanIt>& begin_end, // begin_end: 整个序列的开始和超尾，用于输出整个序列
                          std::set<RanIt>& divide) {    // divide: 记录partition的分隔元素
    auto begin = left;
    auto end = right;
    if (begin < std::prev(end)) {
        auto div = partition(begin, end);
        divide.insert(div);
        std::cout << std::setw(3) << level << " ";              // 输出partition次数
        div_print(begin_end.first, begin_end.second, divide);   // 输出元素partition的情况
        std::cout << "  inf" << std::endl;      // 考试要求最后必须要加∞，即使这里没有所谓的卫元素
        level = quick_sort_with_print(left, div, level + 1, begin_end, divide);
        level = quick_sort_with_print(std::next(div), right, level + 1, begin_end, divide);
    }
    else {
        divide.insert(begin);   // 这里插入元素主要是因为当存在独立元素的时候，仍然会生成括号，该语句能抑制括号产生
        return level - 1;   // 此处，由于没有参与partition，而传参时用了level+1，因此此处要减1
    }
    return level;
}

template<typename RanIt>
void quick_sort_print(RanIt left, RanIt right) {
    std::set<RanIt> divide{};
    std::cout.setf(std::ios::right);
    std::cout << "ini ";    // 初始序列，这是书上要求写出的
    for (auto it = left; it < right; ++it) {
        std::cout << std::setw(8) << *it;
    }
    std::cout << "   inf" << std::endl; // 考试要求最后必须要加∞，即使这里没有所谓的卫元素
    quick_sort_with_print(left, right, 1, std::pair<RanIt, RanIt>{ left, right }, divide);
    std::cout << "fin ";    // 最终序列，这是书上要求的
    for (auto it = left; it < right; ++it) {
        std::cout << std::setw(8) << *it;
    }
    std::cout << "   inf" << std::endl;
}

int main() {
    // 测试样例1
    std::vector<int> vec1{ 467, 436, 588, 826, 628, 179, 461, 345, 890, 6 };
    quick_sort_print(vec1.begin(), vec1.end());
    std::cout << std::endl;
    
    // 测试用例2
    std::vector<int> vec2{ 1, 1, 1, 1, 1 };
    quick_sort_print(vec2.begin(), vec2.end());
    std::cout << std::endl;

    // 测试用例3
    std::vector<int> vec3{ 5, 5, 8, 3, 4, 3, 2 };
    quick_sort_print(vec3.begin(), vec3.end());
    std::cout << std::endl;
}


