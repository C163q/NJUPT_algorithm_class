/**
 * encoded in UTF-8 with BOM
 * 题目：编写三分搜索算法的C++程序实现。
 * 参阅：无
 */

#include<iostream>
#include<algorithm>
#include<iterator>

// solution
int search(int arr[], int left, int right, int x) {
    if (left > right) return -1;
    int mid1 = left + (right - left) / 3;
    int mid2 = right - (right - left) / 3;
    
    if (x == arr[mid1]) return mid1;
    if (x == arr[mid2]) return mid2;

    if (x < arr[mid1]) return search(arr, left, mid1 - 1, x);
    else if (x < arr[mid2]) return search(arr, mid1 + 1, mid2 - 1, x);
    else return search(arr, mid2 + 1, right, x);
}

int search(int arr[], int n, int x) {
    return search(arr, 0, n - 1, x);
}


// solution2
template<typename RanIt>
RanIt search3(RanIt left, RanIt right, const typename std::iterator_traits<RanIt>::value_type& value) {
    if (left >= right) return right;
    auto begin = left;
    auto end = right;
    for (auto mid1 = begin + (end - begin) / 3, mid2 = end - (end - begin) / 3; begin < end; 
              mid1 = begin + (end - begin) / 3, mid2 = end - (end - begin) / 3) {
        if (value < *mid1) {
            end = mid1;
            continue;
        }
        if (value == *mid1) {
            return mid1;
        }
        if (value < *mid2) {
            begin = std::next(mid1);
            end = mid2;
            continue;
        }
        if (value == *mid2) {
            return mid2;
        }
        begin = std::next(mid2);
    }
    return right;
}



int main() {
    constexpr int size = 100;
    int arr[size] {};
    std::copy_n(std::istream_iterator<int>(std::cin), size, arr);
    std::sort(arr, arr + size);
    int to_search = 0;
    std::cin >> to_search;

    int res = search(arr, size, to_search);
    auto res2 = search3(arr, arr + size, to_search);

    std::cout.setf(std::ios::boolalpha);
    std::cout << "search(): " << std::endl;
    if (res == -1) std::cout << "Not found." << std::endl;
    else std::cout << "Found at index " << res << ".  Result: " << (arr[res] == to_search) << std::endl;

    std::cout << std::endl << "search3(): " << std::endl;
    if (res2 == arr + size) std::cout << "Not found." << std::endl;
    else std::cout << "Found at " << res2 << ".  Result: " << (*res2 == to_search) << std::endl;
}

/**
 * << -994 179 403 -696 244 -31 386 -46 -193 52 
 *    305 -685 -578 240 -77 -475 746 -816 716 162 
 *    -685 441 707 -677 937 509 315 -860 518 -762 
 *    309 464 509 -748 -215 458 -842 -309 952 986 
 *    -145 -403 -595 -952 -385 -887 -642 974 -669 -795 
 *    -107 738 -758 -305 -406 -648 588 -495 -88 -194 
 *    -953 647 395 -89 -406 -746 -984 458 -447 184 
 *    924 -65 -984 -115 -411 829 866 162 -481 103 
 *    619 -775 87 882 -732 965 -521 -521 -600 -470 
 *    -240 -249 833 -852 419 -481 -722 489 240 408     
 * << -215
 * >> search(): 
 * >> Found at index 45.  Result: true
 * >> 
 * >> search3():
 * >> Found at 0x5ffdc4.  Result: true
 * 
 * << -994 179 403 -696 244 -31 386 -46 -193 52 
 *    305 -685 -578 240 -77 -475 746 -816 716 162 
 *    -685 441 707 -677 937 509 315 -860 518 -762 
 *    309 464 509 -748 -215 458 -842 -309 952 986 
 *    -145 -403 -595 -952 -385 -887 -642 974 -669 -795 
 *    -107 738 -758 -305 -406 -648 588 -495 -88 -194 
 *    -953 647 395 -89 -406 -746 -984 458 -447 184 
 *    924 -65 -984 -115 -411 829 866 162 -481 103 
 *    619 -775 87 882 -732 965 -521 -521 -600 -470 
 *    -240 -249 833 -852 419 -481 -722 489 240 408     
 * << 0
 * >> search(): 
 * >> Not found.
 * >> 
 * >> search3():
 * >> Not found.
 */


