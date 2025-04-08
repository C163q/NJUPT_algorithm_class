/**
 * encoded in UTF-8 with BOM
 * 题目：贪心思想求解会场安排问题。
 * 参阅：
 *      语言基础补充(file://./../../reference/cpp.md#语言基础补充)
 *      迭代器(file://./../../reference/cpp.md#迭代器)
 *      基于范围的for循环(file://./../../reference/cpp.md#基于范围的for循环)
 */

#include<iostream>
#include<tuple>
#include<vector>
#include<algorithm>

/**
 * struct Meetings：会议类。包括开始时间和结束时间。
 * void arrange(Meetings* m, int size)：
 *      | 算法主逻辑。但是在这里，是直接在函数内打印的，
 *      | 我不喜欢这样的做法，因此会改成带返回值的。
 */

struct meetings {
    int start;
    int finish;
};

// 由于不是C++20，不能使用operator<=>
bool operator==(const meetings& lhs, const meetings& rhs) {
    return lhs.start == rhs.start && lhs.finish == rhs.finish;
}

bool operator<(const meetings& lhs, const meetings& rhs) {
    return lhs.start < rhs.start || (lhs.start == rhs.start && lhs.finish < rhs.finish);
}

std::vector<std::tuple<size_t, const meetings&, size_t>>
    arrange(const std::vector<meetings>& m) {
    std::vector<bool> arranged(m.size());
    size_t place_id = 1;
    std::vector<std::tuple<size_t, const meetings&, size_t>> res;
    res.reserve(m.size());
    for (size_t i = 0; i < m.size(); ++i) {
        if (!arranged[i]) {
            int last_arrange_index = i;
            arranged[i] = true;
            res.emplace_back(i, m[i], place_id);
            for (size_t j = i + 1; j < m.size(); ++j) {
                if (!arranged[j] && m[j].start >= m[last_arrange_index].finish) {
                    arranged[j] = true;
                    res.emplace_back(j, m[j], place_id);
                    last_arrange_index = j;
                }
            }
            ++place_id;
        }
    }
    return res;
}

// 测试算法
void test_algorithm(const std::vector<meetings>& m) {
    std::cout << "要安排的会议如下：" << std::endl;
    for (size_t i = 0; i < m.size(); ++i) {
        std::cout << "会议" << i << "（起始时间" << m[i].start << "结束时间"
                  << m[i].finish << "）" << std::endl;
    }
    auto&& res = arrange(m);
    std::cout << "分配结果如下：" << std::endl;
    for (auto&& meeting_info : res) {
        std::cout << "会议" << std::get<0>(meeting_info) << "（起始时间"
        << std::get<1>(meeting_info).start << "结束时间" << std::get<1>(meeting_info).finish
        << "）被安排在" << std::get<2>(meeting_info) << "会场" << std::endl;
    }
    std::cout << "一共需要" << std::get<2>(res.back()) << "个会场" << std::endl;
}

int main() {
    std::vector<meetings> m1{ {1,23},{24,26},{25,35},{36,50},{27,80} };
    std::vector<meetings> m2 { {1,3},{5,8},{9,13},{6,15},{20,25},{15,30},{7,32},{18,40} };
    test_algorithm(m1);
    std::cout << "-----------------------------------------" << std::endl;
    test_algorithm(m2);
}

/**
 * >> 要安排的会议如下：
 * >> 会议0（起始时间1结束时间23）
 * >> 会议1（起始时间24结束时间26）
 * >> 会议2（起始时间25结束时间35）
 * >> 会议3（起始时间36结束时间50）
 * >> 会议4（起始时间27结束时间80）
 * >> 分配结果如下：
 * >> 会议0（起始时间1结束时间23）被安排在1会场
 * >> 会议1（起始时间24结束时间26）被安排在1会场
 * >> 会议3（起始时间36结束时间50）被安排在1会场
 * >> 会议2（起始时间25结束时间35）被安排在2会场
 * >> 会议4（起始时间27结束时间80）被安排在3会场
 * >> 一共需要3个会场
 * >> -----------------------------------------
 * >> 要安排的会议如下：
 * >> 会议0（起始时间1结束时间3）
 * >> 会议1（起始时间5结束时间8）
 * >> 会议2（起始时间9结束时间13）
 * >> 会议3（起始时间6结束时间15）
 * >> 会议4（起始时间20结束时间25）
 * >> 会议5（起始时间15结束时间30）
 * >> 会议6（起始时间7结束时间32）
 * >> 会议7（起始时间18结束时间40）
 * >> 分配结果如下：
 * >> 会议0（起始时间1结束时间3）被安排在1会场
 * >> 会议1（起始时间5结束时间8）被安排在1会场
 * >> 会议2（起始时间9结束时间13）被安排在1会场
 * >> 会议4（起始时间20结束时间25）被安排在1会场
 * >> 会议3（起始时间6结束时间15）被安排在2会场
 * >> 会议5（起始时间15结束时间30）被安排在2会场
 * >> 会议6（起始时间7结束时间32）被安排在3会场
 * >> 会议7（起始时间18结束时间40）被安排在4会场
 * >> 一共需要4个会场
 */
