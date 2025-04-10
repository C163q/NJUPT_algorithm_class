/**
 * encoded in UTF-8 with BOM
 * 题目：使用Prim算法和Kruskal算法构造G的一棵最小生成树。
 * 参阅：
 *      语言基础补充(file://./../../reference/cpp.md#语言基础补充)
 */

#include<iostream>
#include<queue>
#include<vector>
#include<forward_list>
#include<limits>
#include<tuple>
#include<utility>

/**
 * 题目要求实现以下类和函数：
 * class Graph：用邻接表法表示的图。
 * void Graph<T>::Prim(int k, int* nearest, T* lowcost)：Prim算法构造的最小生成树。
 *      | 注意到这里没有返回值，调用该函数得到的信息通过nearest和lowcost这两个参数得到。
 *      | 个人不太喜欢这种做法，因此会修改该函数定义。
 * class UFSet：并查集类，用于Kruskal算法。
 * void Graph<T>::Kruskal(PrioQueue<eNode<T>>& pq)：Kruskal算法构造的最小生成树。
 *      | 这个函数依然没有返回值，而是通过PrioQueue<eNode<T>>&得到的。
 *      | 这个做法和上面的做法又不一样，让人摸不着头脑，因此仍然会修改该函数定义。
 */

/**
 * 本文件实现prim算法，kruskal算法在另一个文件中。
 */

// 用邻接表法表示的（无向）图。原本题目给出的做法是手动管理内存的，这里使用标准库。
class graph {
public:
    using weight_type = size_t;
    using data_type = std::pair<weight_type, size_t>;

private:
    std::vector<std::forward_list<data_type>> data{};

    void illegal_index(size_t index) const {
        if (index >= data.size()) throw std::out_of_range("Illegal index.");
    }
public:
    graph() = default;  // 不用这个
    ~graph() = default;
    explicit graph(size_t size) : data(size) {}
    graph(const graph&) = delete;   // 图方便，不允许复制构造
    graph& operator=(const graph&) = delete;

    void connect(size_t from, size_t to, weight_type w) {
        illegal_index(from);
        illegal_index(to);
        data[from].emplace_front(w, to);
        data[to].emplace_front(w, from);
    }

    const std::forward_list<data_type>& get_connection(size_t index) const {
        illegal_index(index);
        return data[index];
    }

    size_t size() const noexcept {
        return data.size();
    }
};

// 题目中提示的题解方法使用特殊值作为没有连接时的数值，这种做法有效但不太好。
constexpr size_t not_connect = std::numeric_limits<size_t>::max();

// prim()使用的是函数，而不是题目里面定义的作为graph的方法。
std::pair<std::vector<size_t>, std::vector<size_t>> prim(const graph& g, size_t k = 0) {
    if (k >= g.size()) throw std::out_of_range("Illegal index.");
    std::vector<bool> mark(g.size());
    std::vector<size_t> nearest(g.size(), not_connect);
    std::vector<size_t> lowcost(g.size(), not_connect);

    mark[k] = true;
    nearest[k] = k;
    lowcost[k] = 0;
    for (size_t i = 1; i < g.size(); ++i) {
        for (auto&& weight_and_connect : g.get_connection(k)) {
            size_t connect = weight_and_connect.second;
            if (!mark[connect] && (lowcost[connect] > weight_and_connect.first)) {
                lowcost[connect] = weight_and_connect.first;
                nearest[connect] = k;
            }
        }
        size_t min = not_connect;
        for (size_t j = 0; j < g.size(); ++j) {
            if (!mark[j] && (lowcost[j] < min)) {
                min = lowcost[j];
                k = j;
            }
        }
        mark[k] = true;
    }
    return std::make_pair<std::vector<size_t>, std::vector<size_t>>(
                                std::move(nearest), std::move(lowcost));
}

std::ostream& operator<<(std::ostream& os, const std::vector<size_t>& vec) {   // 临时的
    std::cout << "[";
    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << vec[i];
        if (i + 1 != vec.size()) {
            std::cout << ", ";
        }
    }
    std::cout << "]";
    return os;
}

// 测试函数
void test_algorithm() {
    // 书上P107的图
    graph g(6);
    g.connect(0, 1, 6);
    g.connect(0, 2, 1);
    g.connect(0, 3, 5);
    g.connect(1, 2, 5);
    g.connect(1, 4, 3);
    g.connect(2, 3, 5);
    g.connect(2, 4, 6);
    g.connect(2, 5, 4);
    g.connect(3, 5, 2);
    g.connect(4, 5, 6);
    // 由于结构化绑定auto [nearest, lowcost] = prim(g);的写法是C++17的，因此只能退而求其次。
    std::vector<size_t> nearest, lowcost;
    std::tie(nearest, lowcost) = prim(g);
    std::cout << "nearest: " << nearest << std::endl
              << "lowcost: " << lowcost << std::endl;
}

int main() {
    test_algorithm();
}

/**
 * >> nearest: [0, 2, 0, 5, 1, 2]
 * >> lowcost: [0, 5, 1, 2, 3, 4]
 */
