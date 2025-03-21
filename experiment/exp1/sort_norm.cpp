/**
 * encoded in UTF-8 with BOM
 * 题目：用分治法实现一组无序序列的两路合并排序和快速排序。要求清楚合并排序合并排序及快速排序的基本原理，
 *      编程实现分别用这两种方法将输入的一组无序序列排序为有序序列后输出。
 * 参阅：
 *      语言基础补充(file://./../../reference/cpp.md#语言基础补充)
 */

#include<iostream>
#include<utility>

/**
 * 题目要求定义一个可排序表类SortableList，并在该类上实现两路合并排序算法、快速排序算法、输入和输出。
 * 使用题目给出的模板。
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
    void MergeSort();
    void QuickSort() noexcept;

private:
    void MergeSort(int left, int right);        // right: 最后一个元素的索引

    void Merge(int left, int mid, int right);   // mid: 前一个序列的最后一个元素的索引
                                                // right: 后一个序列的最后一个元素的索引

    void Swap(int i, int j) noexcept;
    void QuickSort(int left, int right) noexcept;
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

void SortableList::MergeSort() {
    MergeSort(0, n - 1);
}

void SortableList::MergeSort(int left, int right) {
    if (left < right) {
        int mid = (left + right) / 2;   // 可能会溢出，但此处不考虑这种情况
        MergeSort(left, mid);
        MergeSort(mid + 1, right);
        Merge(left, mid, right);
    }
}

void SortableList::Merge(int left, int mid, int right) {
    int* temp = new int[right - left + 1];
    int i = left, j = mid + 1, k = 0;
    while ((i <= mid) && (j <= right)) {
        if (l[i] <= l[j]) temp[k++] = l[i++];
        else temp[k++] = l[j++];
    }
    while (i <= mid) temp[k++] = l[i++];
    while (j <= right) temp[k++] = l[j++];
    for (i = 0, k = left; k <= right;) l[k++] = temp[i++];
    delete[] temp;  // 得释放内存，这在实验说明内没有给出
}

void SortableList::QuickSort() noexcept {
    QuickSort(0, n - 1);
}

void SortableList::Swap(int i, int j) noexcept {
    std::swap(l[i], l[j]);
}

int SortableList::Partition(int left, int right) noexcept{
    int i = left, j = right + 1;
    do {
        do i++; while (i <= right && l[i] < l[left]);   // 前者防止越界，这在实验说明内没有给出
        do j--; while (l[j] > l[left]);
        if (i < j) Swap(i, j);
    } while (i < j);
    Swap(left, j);
    return j;
}

void SortableList::QuickSort(int left, int right) noexcept {
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
 * 注意：实验说明内的QuickSort没有边界检查，会导致上述数据排序时出现越界访问，此处已修正。
 * 
 * 测试用例2：
 * << -57 -91 55 -2 37 -6 73 -66 4 2
 * >> 排序前的序列：-57 -91 55 -2 37 -6 73 -66 4 2
 * >> 排序后的序列：-91 -66 -57 -6 -2 2 4 37 55 73
 * << -57 83 47 -9 -20 14 21 -42 30 -82
 * >> 排序前的序列：-57 83 47 -9 -20 14 21 -42 30 -82 
 * >> 排序后的序列：-82 -57 -42 -20 -9 14 21 30 47 83
 */

