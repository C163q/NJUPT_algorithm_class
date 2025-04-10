/**
 * encoded in UTF-8 with BOM
 * 题目：使用Prim算法和Kruskal算法构造G的一棵最小生成树。
 * 参阅：
 *      语言基础补充(file://./../../reference/cpp.md#语言基础补充)
 */

#include<iostream>
#include<vector>
#include<forward_list>
#include<utility>
#include<unordered_map>
#include<memory>
#include<stack>
#include<queue>
#include<functional>

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
 * 本文件实现kruskal算法，prim算法在另一个文件中。
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

// 并查集类
class union_find_set {
public:
    using element_type = size_t;    // 为了方便，就不写模板了

private:
    struct set_node {
        element_type m_data;
        set_node* father = nullptr; // 指向自己的父结点，nullptr表明是根结点
    public:
        set_node() noexcept = default;  // 一般不使用
        ~set_node() noexcept = default;
        explicit set_node(element_type element) noexcept : m_data(element) {}
        bool operator==(const set_node& other) const {
            return m_data == other.m_data;
        }
    };

    struct node_hash {
        size_t operator()(const set_node& node) const noexcept {
            return std::hash<element_type>()(node.m_data);
        }
    };

private:
    std::unordered_map<element_type, std::unique_ptr<set_node>> element_map;
    std::unordered_map<set_node, size_t, node_hash> size_map;

private:
    set_node& find_head(set_node& node) {
        std::stack<set_node*> path;
        auto ptr = &node;
        while (ptr->father) {
            path.push(ptr);
            ptr = ptr->father;
        }
        while (!path.empty()) {
            path.top()->father = ptr;
            path.pop();
        }
        return *ptr;
    }

    bool same_set_uncheck(set_node& lhs, set_node& rhs) {
        set_node& lhs_head = find_head(lhs);
        set_node& rhs_head = find_head(rhs);
        return lhs_head == rhs_head;
    }

public:
    union_find_set() noexcept = default;    // 一般不使用
    ~union_find_set() noexcept = default;
    template<typename InputIt>
    explicit union_find_set(InputIt begin_it, InputIt end_it) {
        for (; begin_it != end_it; ++begin_it) {
            auto&& val = *begin_it;
            auto&& ptr = std::make_unique<set_node>(val);
            size_map.emplace(*ptr, 1);
            element_map.emplace(val, std::move(ptr));
        }
    }

    bool same_set(const element_type& lhs, const element_type& rhs) {
        auto lhs_it = element_map.find(lhs);
        auto rhs_it = element_map.find(rhs);
        if (lhs_it == element_map.end() || rhs_it == element_map.end()) {   // 根本不存在查找结点
            return false;
        }
        return same_set_uncheck(*(lhs_it->second), *(rhs_it->second));
    }

    void union_element(const element_type& lhs, const element_type& rhs) {
        auto lhs_it = element_map.find(lhs);
        auto rhs_it = element_map.find(rhs);
        if (lhs_it == element_map.end() || rhs_it == element_map.end()) {   // 根本不存在查找结点
            return;
        }
        set_node& lhs_head = find_head(*(lhs_it->second));
        set_node& rhs_head = find_head(*(rhs_it->second));
        if (lhs_head == rhs_head) return;
        size_t lhs_size = size_map.find(lhs_head)->second;
        size_t rhs_size = size_map.find(rhs_head)->second;
        set_node* bigger = nullptr;
        set_node* smaller = nullptr;
        if (lhs_size > rhs_size) {
            bigger = &lhs_head;
            smaller = &rhs_head;
        }
        else {
            bigger = &rhs_head;
            smaller = &lhs_head;
        }
        smaller->father = bigger;
        size_map[*bigger] = lhs_size + rhs_size;
        size_map.erase(*smaller);
    }
};

using graph_edge = std::tuple<size_t, size_t, graph::weight_type>;

std::vector<graph_edge> kruskal(const graph& g) {
    size_t n = g.size();
    auto edge_length_greater = [] (const graph_edge& lhs, const graph_edge& rhs) {
        return std::get<2>(lhs) > std::get<2>(rhs);
    };
    std::priority_queue<graph_edge, std::vector<graph_edge>,
                        decltype(edge_length_greater)> pq(edge_length_greater);
    for (size_t index = 0; index < n; ++index) {
        auto&& connect = g.get_connection(index);
        for (auto&& edge : connect) {
            pq.emplace(index, edge.second, edge.first);
        }
    }
    std::vector<size_t> vec;
    vec.reserve(n);
    for (size_t i = 0; i < n; ++i) {
        vec.emplace_back(i);
    }
    union_find_set s(vec.begin(), vec.end());
    size_t k = 0;
    std::vector<graph_edge> res;
    while (k < n - 1 && !pq.empty()) {
        auto&& x = pq.top();
        if (!s.same_set(std::get<0>(x), std::get<1>(x))) {
            res.push_back(std::move(x));
            s.union_element(std::get<0>(x), std::get<1>(x));
            ++k;
        }
        pq.pop();
    }
    return res;
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
    auto&& res = kruskal(g);
    std::cout << "连接的边为：" << std::endl;
    for (const auto& edge : res) {
        std::cout << "(" << std::get<0>(edge) << ", " << std::get<1>(edge)
                  << ")，权值为：" << std::get<2>(edge) << std::endl; 
    }
}

int main() {
    test_algorithm();
}

/**
 * >> 连接的边为：
 * >> (0, 2)，权值为：1
 * >> (3, 5)，权值为：2
 * >> (1, 4)，权值为：3
 * >> (2, 5)，权值为：4
 * >> (2, 1)，权值为：5
 */
