/**
 * encoded in UTF-8 with BOM
 * 题目：采用基于“五元中值组取中值分割法”的线性时间选择算法，
 *      找出N个元素集合S中的第k个最小的元素，使其在线性时间内解决。
 * 参阅：
 *      语言基础补充(file://./../../reference/cpp.md#语言基础补充)
 */

#include<iostream>
#include<utility>
#include<exception>
#include<algorithm>
#include<cmath>
#include<algorithm>
#include<vector>

/**
 * 仍然使用题目给出的可排序表类SortableList。
 * 使用题目给出的模板，仅在十分简单的地方使用标准库。
 */
class SortableList {
public:
    SortableList(int mSize) {
        maxSize = mSize;
        l = new int[maxSize];
        n = 0;
    }
    
    ~SortableList() noexcept {
        delete[] l;
    }

    void Input(int count);
    void Output();
    int Select(int k);

    int* begin() noexcept {
        return l;
    }
    int* end() noexcept {
        return l + n;
    }

private:
    int Select(int k, int left, int right, int r);  // right: 最后一个元素的索引
    void InsertSort(int left, int right);

    void Swap(int i, int j);

    int Partition(int left, int right) noexcept;

private:
    int* l;
    int maxSize;
    int n;
};

void SortableList::Input(int count) {
    int index = 0;
    count = std::min(count, maxSize);
    int ct = count;
    while (ct--) {
        std::cin >> l[index++];
    }
    n = count;
}

void SortableList::Output() {
    for (int i = 0; i < n; ++i) {
        std::cout << l[i] << ' ';
    }
}

void SortableList::InsertSort(int left, int right) {
    for (int i = left + 1; i < right + 1; ++i) {
        int tmp = l[i];
        int pos = i;
        for (; pos > left; --pos) {
            if (l[pos - 1] <= tmp) break;
            else l[pos] = l[pos - 1];
        }
        l[pos] = tmp;
    }
}

void SortableList::Swap(int i, int j) {
    std::swap(l[i], l[j]);
}

int SortableList::Partition(int left, int right) noexcept {
    int i = left, j = right + 1;
    do {
        do i++; while (i <= right && l[i] < l[left]);
        do j--; while (l[j] > l[left]);
        if (i < j) Swap(i, j);
    } while (i < j);
    Swap(left, j);
    return j;
}

int SortableList::Select(int k) {
    if (n <= 0 || k > n || k <= 0) throw std::out_of_range("out of index!");
    int j = Select(k, 0, n - 1, 5);
    return l[j];
}

int SortableList::Select(int k, int left, int right, int r) {
    int n = right - left + 1;
    if (n <= r) {
        InsertSort(left, right);
        return left + k - 1;        
    }
    for (int i = 0; i < n / r; ++i) {
        InsertSort(left + i * r, left + (i + 1) * r - 1);
        Swap(left + i, left + i * r + (int)std::ceil((double)r / 2) - 1);
    }
    int j = Select((int)ceil((double)n / r / 2), left, left + (n / r) - 1, r);
    Swap(left, j);
    j = Partition(left, right);
    if (k == j - left + 1) return j;
    else if (k < j - left + 1) return Select(k, left, j - 1, r);
    else return Select(k - (j - left + 1), j + 1, right, r);
}

int main() {
    int n = 100;
    int k = 42;
    SortableList list(n); // 测试的时候使用n个数据
    
    // 输入测试样例（n个）
    list.Input(n);
    std::cout << "列表内的序列：";
    list.Output();
    std::cout << std::endl;
    
    // 使用标准库来获取预期的结果
    std::vector<int> vec(n);
    std::copy(list.begin(), list.end(), vec.begin());
    std::nth_element(vec.begin(), vec.begin() + k - 1, vec.end());  // 使用标准库进行验证
    std::cout << "预期的第" << k << "个元素为：" << *(vec.begin() + k - 1) << std::endl;

    // 验证Select()函数
    std::cout << "第" << k << "个元素为：" << list.Select(k) << std::endl;
}

/**
 * 测试用例1（..表示接上一行）：
 * << 465 -45 195 497 693 -151 -900 645 32 -766 
 * .. 400 -64 -671 -499 -517 -527 -126 695 955 295 
 * .. -48 913 459 68 907 432 286 -642 -309 455 
 * .. 163 -838 -717 340 -924 959 260 326 -938 -510 
 * .. -106 -79 -160 -8 537 762 894 -102 -992 -693 
 * .. -377 768 514 745 53 -126 -797 -673 -629 383 
 * ..  -677 464 -202 998 -160 -19 -28 -767 735 970 
 * .. 668 661 155 -994 151 -638 297 -565 943 -574 
 * .. -978 234 -353 -883 282 -125 16 -562 -62 -68 
 * .. 91 -176 -214 -254 318 -982 14 16 359 452     
 * >> 列表内的序列：465 -45 195 497 ...(省略)
 * >> 预期的第42个元素为：-102
 * >> 第42个元素为：-102
 * 
 * 测试用例2：
 * << 470 560 715 529 -729 -644 644 972 630 221 
 * .. -760 129 -825 -787 -943 124 -644 -470 -259 272 
 * .. 134 206 336 382 300 -656 40 19 938 802 
 * .. -155 -178 103 148 -665 -909 -442 -647 -567 922 
 * .. -965 962 -329 -900 -309 409 -439 -506 270 -627 
 * .. 981 783 -163 -613 -328 440 735 -944 354 418 
 * .. -555 -695 29 -648 -548 198 -888 212 -69 -715 
 * .. -887 -98 163 882 832 -966 -767 887 -825 -886 
 * .. 891 963 -388 -533 -459 -772 365 996 -402 -45 
 * .. 537 194 224 81 -462 294 801 484 -510 -680      
 * >> 列表内的序列：470 560 715 529 ...(省略)
 * >> 预期的第42个元素为：-328
 * >> 第42个元素为：-328
 */
