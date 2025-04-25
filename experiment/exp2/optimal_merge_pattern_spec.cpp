/**
 * encoded in UTF-8 with BOM
 * 题目：采用贪心法进行两路合并最佳模式
 * 参阅：
 *      语言基础补充(file://./../../reference/cpp.md#语言基础补充)
 */

#include<iostream>
#include<queue>
#include<memory>
#include<exception>

/**
 * 题目要求实现以下几个类和函数：
 * - class PrioQueue：优先权队列，考虑到数据结构课程已经实现过优先权队列了，因此此处直接使用std::priority_queue。
 * - class BTNode：二叉树结点类。
 * - struct HNode：优先权队列中元素的类型。
 * - BTNode<T>* CreateHfmTree(T*, int)：主要逻辑。
 * 我非常不想要定义上面的这两个类和这一个函数，因为HNode存储的是指向BTNode的指针和权值，
 * 而这个权值与所指向的BTNode当中存储的值一致，既然如此，为何不直接实现一个访问器直接访问呢？
 * 因此，我决定重新组织这两个类和函数的关系，通过merge_tree类管理其中的tree_node，
 * 并提供对合并以及访问权值的支持。
 */

// 类似于实验要求里的struct HNode，但对管理的结点进行隐藏。
class merge_tree {
public:
    using element_type = size_t;

private:
    // 类似于实验要求里的class BTNode，但对外部隐藏，被merge_tree管理。
    struct tree_node {
        element_type element;   // 这里就不用模板了
        std::unique_ptr<tree_node> left{};  // 使用std::unique_ptr避免手动管理内存的风险
        std::unique_ptr<tree_node> right{};

        tree_node() noexcept = default;
        ~tree_node() noexcept = default;
        tree_node(element_type elem) noexcept : element(elem) {}
        tree_node(element_type elem, std::unique_ptr<tree_node> left_ptr,
                                     std::unique_ptr<tree_node> right_ptr)
                : element(elem), left(std::move(left_ptr)), right(std::move(right_ptr)) {}
        tree_node(const tree_node&) = delete;   // 复制构造太烦了，这里也用不到，就不写了
        tree_node(tree_node&& other) noexcept   // 移动构造比较简单
                : element(other.element), left(std::move(other.left)), right(std::move(other.right)) {}
        tree_node& operator=(const tree_node&) = delete;    // 复制赋值也很烦
        tree_node& operator=(tree_node&& other) noexcept {
            if (this == std::addressof(other)) return *this;
            element = other.element;
            left = std::move(other.left);
            right = std::move(other.right);
            return *this;
        }
    };

    void print_impl(const std::unique_ptr<tree_node>& node_ptr) const noexcept {
        if (!node_ptr) return;
        std::cout << node_ptr->element << ' ';
        print_impl(node_ptr->left);
        print_impl(node_ptr->right);
    }

private:
    std::unique_ptr<tree_node> root{};

public:
    merge_tree() noexcept = default;
    ~merge_tree() noexcept = default;
    // 构造仅包含根节点的树，此算法中需要大量使用该构造函数
    merge_tree(element_type elem) noexcept : root(std::make_unique<tree_node>(elem)) {}
    merge_tree(const merge_tree&) = delete; // 同样，实现起来太麻烦
    merge_tree(merge_tree&& other) noexcept : root(std::move(other.root)) {}
    merge_tree& operator=(const merge_tree&) = delete;
    merge_tree& operator=(merge_tree&& other) noexcept {
        if (this == std::addressof(other)) return *this;
        root = std::move(other.root);
        return *this;
    }

    element_type weight() const noexcept { return root->element; }

    void merge(merge_tree&& other) {    // 最佳合并模式里的合并
        root = std::make_unique<tree_node>(weight() + other.weight(), std::move(root), std::move(other.root));
    }

    void print() const noexcept {   // 这里就不写迭代器了，直接写一个打印函数
        const std::unique_ptr<tree_node>& now = root;
        print_impl(now);    // 先序遍历打印
    }

    bool operator>(const merge_tree& other) const noexcept {
        return weight() > other.weight();
    }

};

// 类似于实验要求里的BTNode<T>* CreateHfmTree(T*, int)，对签名进行了修改。
merge_tree optimal_merge_pattern(const std::vector<size_t>& weights /*权值数组*/ ) {
    // C++默认最大堆，因此需要使用std::greater<>
    std::priority_queue<merge_tree, std::vector<merge_tree>, std::greater<>> pq;
    for (auto&& weight : weights) {
        pq.emplace(weight);
    }
    if (pq.empty()) throw std::invalid_argument("argument weights cannot be empty.");   // 权值数组为空的情况
    while (pq.size() > 1) {
        auto left_tree(std::move(const_cast<merge_tree&>(pq.top())));   // risky，但没有想到更好的解决方案
        pq.pop();
        left_tree.merge(std::move(const_cast<merge_tree&>(pq.top())));  // 与上面同理
        pq.pop();
        pq.emplace(std::move(left_tree));
    }
    auto res(std::move(const_cast<merge_tree&>(pq.top())));
    pq.pop();   // 这段可以不写，直接返回
    return res;
}

// 测试函数
void test_algorithm(const std::vector<size_t>& weights) {
    auto res = optimal_merge_pattern(weights);
    res.print();    // 先序遍历得到的树
    std::cout << std::endl;
}

int main() {
    test_algorithm({ 3, 7, 8, 9, 15, 16, 18, 20, 23, 25, 28 });
}

/**
 * >> 172 71 33 16 17 8 9 38 18 20 101 48 23 25 53 25 10 3 7 15 28 
 */
