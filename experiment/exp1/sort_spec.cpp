/**
 * encoded in UTF-8 with BOM
 * 题目：用分治法实现一组无序序列的两路合并排序和快速排序。要求清楚合并排序合并排序及快速排序的基本原理，
 *      编程实现分别用这两种方法将输入的一组无序序列排序为有序序列后输出。
 * 参阅：
 *      语言基础补充(file://./../../reference/cpp.md#语言基础补充)
 *      迭代器(file://./../../reference/cpp.md#迭代器)
 */

#include<iostream>
#include<utility>
#include<algorithm>
#include<iterator>
#include<vector>

/**
 * 题目要求定义一个可排序表类SortableList，并在该类上实现两路合并排序算法、快速排序算法、输入和输出。
 * 限制必须按照题目给出的接口的含义实现，但该文件中会多次使用标准库来实现。
 */
class SortableList {    // 题目给出的数据结构及其接口
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
    void MergeSort();
    void QuickSort();
    
    // vvv 自定义的接口
    int* begin() { return l; }
    int* end() { return l + n; }

private:
    void MergeSort(int left, int right);        // right: 最后一个元素的索引

    void Merge(int left, int mid, int right);   // mid: 前一个序列的最后一个元素的索引
                                                // right: 后一个序列的最后一个元素的索引

    void Swap(int i, int j) noexcept;
    void QuickSort(int left, int right);
    int Partition(int left, int right);

private:
    int* l;
    int maxSize;
    int n;
};

void SortableList::Input(int count) {
    count = std::max(0, std::min(maxSize, count));
    // std::clamp()是C++17更新的，此处只能使用这种方法替代。
    std::copy_n(std::istream_iterator<int>(std::cin), count, begin());
    n = count;
}

void SortableList::Output() {
    std::copy(begin(), end(), std::ostream_iterator<int>(std::cout, " "));
}

void SortableList::MergeSort() {
    // 标准库std::stable_sort()不全是归并排序，因此不能直接使用。
    MergeSort(0, n - 1);
}

void SortableList::MergeSort(int left, int right) { // 接口要求right为最后一个元素的索引
    if (left < right) {
        // std::midpoint()是C++20加入的，此处不能使用
        int mid = left + (right - left) / 2;    // right > left
        MergeSort(left, mid);
        MergeSort(mid + 1, right);
        Merge(left, mid, right);
    }
}

void SortableList::Merge(int left, int mid, int right) {
    // 接口要求mid为前一个序列的最后一个元素的索引; right为后一个序列的最后一个元素的索引。
    std::vector<int> tmp(right - left + 1);
    // 使用vector自动管理内存，也防止std::merge()抛出异常后未释放内存，也可以使用std::unique_ptr<int[]>
    std::merge(begin() + left, begin() + mid + 1, begin() + mid + 1, begin() + right + 1, tmp.begin());
    std::copy(tmp.begin(), tmp.end(), begin() + left);
}

void SortableList::QuickSort() {    // 由于使用了std::partition()，因此该函数不是noexcept的。
    // 标准库std::sort()不全是快速排序，因此不能直接使用。
    QuickSort(0, n - 1);
}

void SortableList::Swap(int i, int j) noexcept {    // 这个函数其实不用使用
    std::swap(l[i], l[j]);
}

int SortableList::Partition(int left, int right) {
    int base_val = *(begin() + left);
    // 使用第一个元素来partition()，这是教材的标准做法。
    auto it = std::partition(begin() + left + 1, begin() + right + 1,
                             [base_val] (int val) { return val <= base_val; });
    std::swap(*--it, *(begin() + left));
    return static_cast<int>(std::distance(begin(), it));
    // Partition()要求返回int，因此使用这种方式转换。
}

void SortableList::QuickSort(int left, int right) {
    if (left < right) {
        int j = Partition(left, right);
        QuickSort(left, j - 1);
        QuickSort(j + 1, right);
    }
}


int main() {
    SortableList list(10);  // 测试的时候就只用10个数据了
    
    // 测试MergeSort
    list.Input(10);
    std::cout << "排序前的序列：";
    list.Output();
    list.MergeSort();
    std::cout << std::endl << "排序后的序列：";
    list.Output();
    std::cout << std::endl;

    // 测试QuickSort
    list.Input(10);
    std::cout << "排序前的序列：";
    list.Output();
    list.QuickSort();
    std::cout << std::endl << "排序后的序列：";
    list.Output();
    std::cout << std::endl;
}

/**
 * 测试用例1（第一次输入是测试MergeSort的，第二次输入是测试QuickSort的）：
 * << 20 40 57 66 69 77 10 87 60 18
 * >> 排序前的序列：20 40 57 66 69 77 10 87 60 18
 * >> 排序后的序列：10 18 20 40 57 60 66 69 77 87
 * << 10 1 2 3 4 5 6 7 8 9
 * >> 排序前的序列：10 1 2 3 4 5 6 7 8 9
 * >> 排序后的序列：1 2 3 4 5 6 7 8 9 10
 * 
 * 测试用例2：
 * << -57 -91 55 -2 37 -6 73 -66 4 2
 * >> 排序前的序列：-57 -91 55 -2 37 -6 73 -66 4 2
 * >> 排序后的序列：-91 -66 -57 -6 -2 2 4 37 55 73
 * << -57 83 47 -9 -20 14 21 -42 30 -82
 * >> 排序前的序列：-57 83 47 -9 -20 14 21 -42 30 -82 
 * >> 排序后的序列：-82 -57 -42 -20 -9 14 21 30 47 83
 */

