# 目录

- [目录](#目录)
- [前言](#前言)
- [语言基础补充](#语言基础补充)
  - [noexcept](#noexcept)
  - [= default](#-default)
  - [auto](#auto)
- [迭代器](#迭代器)
  - [前提知识](#前提知识)
  - [详细参考](#详细参考)
  - [相关头文件](#相关头文件)
  - [介绍](#介绍)
  - [进一步了解](#进一步了解)
- [基于范围的for循环](#基于范围的for循环)
  - [前提知识](#前提知识-1)
  - [详细参考](#详细参考-1)
  - [相关头文件](#相关头文件-1)
  - [介绍](#介绍-1)

# 前言

以下内容仅用于了解这个库里用到的代码，不能作为学习C++的参考。

这些内容不会讲得非常全面，也不能保证准确（但会尽力做到没有错误）。

**前提知识**中不会列出南邮C语言和C++语言课程当中学到的内容。

以下内容基于**C++14**。

# 语言基础补充

## noexcept

关键字。用法之一是**保证函数（方法）不会抛出异常**。若函数抛出异常，则程序立刻终止，不会向外传播。

例如声明：`void func1() noexcept;`表明函数`func1()`保证不会抛出异常。

```C++
#include<iostream>

void func2() { throw "func2"; }
void func3() noexcept { throw "func3"; }

int main() {
    try { func2(); }
    catch (const char* s) { std::cout << s << std::endl; }
    // 输出func2，因为捕获了异常。

    try { func3(); }
    catch (const char* s) { std::cout << s << std::endl; }
    // 不会被捕获，因为func3()声明为noexcept，但抛出了异常，程序直接退出。
}
```

## = default

某些特殊函数能够被编译器自动生成（例如默认构造函数），则可以使用`= default`来显式指明该函数应该由编译器自动生成。

```C++
class A {
public:
    A() = default;
};
```

如果某个类可以使用编译器生成的默认构造函数，但由于定义了其他构造函数，导致其不会自动生成，则可以使用`= default`显式指明需要由编译器自动生成。

## auto

在本仓库中，大多数`auto`的用法都是作为类型的占位符，具体类型会由编译器推导得出：
```C++
auto val1 = 1;          // int
const auto val2 = 1.0f; // const float
auto& val3 = val1;      // int&
auto val4 = &val3;      // int*
```

详细参考见[cppreference](https://zh.cppreference.com/w/cpp/language/auto)

# 迭代器

## 前提知识

- [语言基础补充](#语言基础补充)

## 详细参考

- [cppreference](https://zh.cppreference.com/w/cpp/iterator)

## 相关头文件

- `<iterator>`

## 介绍

迭代器是一种广义指针，它使得C++能够以一种统一的，类似于使用指针访问数组的方式去访问自定义（或由库定义）的数据结构。

更加具体的来说，迭代器是这样的一种类型：它通过重载运算符（例如：自增运算符`++`和解引用运算符`*`）来模拟指针的行为。但与指针不同之处在于，指针的自增运算是内存地址的增加，与所使用的数据结构没有关联，这导致对于如链表等数据结构而言，`++`并不能让指针指向下一个元素所在的内存地址。迭代器是与数据结构相关联的，这使得它能够解决上述问题，对于链表的迭代器而言，`++`运算符能够使迭代器指向下一个元素，而非指向接下来的内存地址。

迭代器具体以下特征：

- 是独立的类。
- 没有特殊的类名要求（任何符合命名规范的名称都可以）。
- 在C++17以前，一般需要继承`std::iterator`。
- 数据结构的迭代器分为迭代器`iterator`和常迭代器`const_iterator`，对应于`T*`和`const T*`，前者可以修改数据结构内的元素，后者不可以。
- 一般数据结构中使用`iterator`和`const_iterator`的类型别名来表示其对应的迭代器和常迭代器。例如单向链表`std::forward_list`的迭代器类型为`std::forward_list::iterator`。有些数据结构的迭代器和常迭代器的类型是相同的，如集合`std::set`。
- 对迭代器使用`const`一般意味着其指向的地址是不可更改的，类似于`T* const`。
- 任何迭代器都需要满足可自增（`++`）和可解引用（`*`）。
- 对于大多数迭代器而言，其运算符重载满足：
  - `++`：指向下一个元素。
  - `--`：指向上一个元素。
  - `+`：返回指向之后的第n个元素的迭代器。
  - `-(int)`：返回指向之前的第n个元素的迭代器。
  - `-(iterator)`：与另一个迭代器相差n个元素。
  - `+=`：指向之后的第n个元素。
  - `-=`：指向之前的第n个元素。
  - `*`：访问当前指向的元素。
  - `->`：解引用并访问其成员。
  - `[]`：等价于`*(it + n)`。
  - `>`：迭代器是否位于另一迭代器之后。
  - `<`：迭代器是否位于另一迭代器之前。
  - `>=`：同`(l == r || l > r)`。
  - `<=`：同`(l == r || l < r)`
  - `==`：两个迭代器是否指向同一元素。
  - `!=`：两个迭代器是否指向不同元素。

以下是一个简单的迭代器的示例：

```C++
#include<iterator>

class my_iterator : public std::iterator<std::input_iterator_tag, int> {
private:
    int* p = nullptr;
public:
    my_iterator() = default;                // 默认构造函数
    my_iterator(int* p) noexcept : p(p) {}
    my_iterator& operator++() noexcept {    // 重载自增运算符
        ++p;
        return *this;
    }
    my_iterator operator++(int) noexcept {  // 重载自增运算符
        my_iterator ret(*this);
        ++p;
        return ret;
    }
    int& operator*() const noexcept {       // 重载解引用运算符
        return *p;
    }
};
```

`C++17`前，共有5中迭代器：
- 输出迭代器
  - 需要重载`++`、`*`。一般`*`返回代理。
  - 例如`ostream_iterator`。
- 输入迭代器
  - 需要重载`++`、`*`、`->`、`!=`、`==`。
- 向前迭代器
  - 需要重载`++`、`*`、`->`、`!=`、`==`。
  - 例如`forward_list`的迭代器。
- 双向迭代器
  - 需要重载`++`、`--`、`*`、`->`、`!=`、`==`。
  - 例如`list`的迭代器和`set`的迭代器。
- 随机访问迭代器
  - 需要重载`++`、`--`、`*`、`->`、`!=`、`==`、`+=`、`+`、`-=`、`-`、`[]`、`>`、`<`、`>=`、`<=`。
  - 例如`vector`的迭代器。

其中，随机访问迭代器一定是双向迭代器，双向迭代器一定是向前迭代器，向前迭代器一定是输入迭代器。

大部分容器都有以下方法：
- `begin()`：返回指向容器开头的迭代器。
- `cbegin()`：返回指向容器开头的常迭代器。
- `end()`：返回指向容器超尾（即容器最后一个元素之后的位置，访问该位置是未定义行为）的迭代器。
- `cend()`：返回指向容器超尾的常迭代器。
- `rbegin()`：返回指向容器开头的逆向迭代器，等价于`std::make_reverse_iterator(Container.end())`。
- `crbegin()`：返回指向容器开头的逆向常迭代器。
- `rend()`：返回指向容器末尾的逆向迭代器。
- `crend()`：返回指向容器末尾的逆向迭代器。

其中，逆向迭代器仅适用于双向迭代器。

## 进一步了解

各容器所在的命名空间（通常是`std`）通常存在`begin()`、`cbegin()`、`end()`、`cend()`、`rbegin()`、`crbegin()`、`rend()`、`crend()`函数，例如使用`begin(Container)`等价于使用`Container.begin()`，这是为了兼容C风格的数组（`T[N]`），因为这种数组不存在`arr.begin()`等方法，但标准库提供了`std::begin(T(&arr)[N])`，这使得以下写法是合法的：
```C++
int arr[100] {};
auto it1 = std::cbegin(arr);  // 相当于const int* it1 = &arr[0];
std::vector<int> vec{};
auto it2 = std::cbegin(vec);  // 相当于std::vector<int>::const_iterator it2 = vec.cbegin();
```
最终任何`std`内的容器（包括C风格的数组）都可以通过使用`std::begin()`函数的方式来访问容器指向头部的迭代器。

但为了兼容任何命名空间的`begin()`等函数，可以使用以下方式（依赖`ADL`）：
```C++
#include<iterator>
#include<utility>
#include<vector>
#include<filesystem>

#define DEF_ITER_FUNC(myFunc)         \
template<typename T, std::size_t N>   \
auto myFunc(T(&arr)[N]) { return std::myFunc(arr); }

DEF_ITER_FUNC(begin)
DEF_ITER_FUNC(cbegin)
DEF_ITER_FUNC(end)
DEF_ITER_FUNC(cend)
DEF_ITER_FUNC(rbegin)
DEF_ITER_FUNC(crbegin)
DEF_ITER_FUNC(rend)
DEF_ITER_FUNC(crend)

int main() {
    int arr[10] {};
    (void)begin(arr);
    std::vector<int> v;
    (void)begin(v);
    std::filesystem::directory_iterator d;
    (void)begin(d);
}
```

# 基于范围的for循环
## 前提知识

- [迭代器](#迭代器)

## 详细参考

- [cppreference](https://zh.cppreference.com/w/cpp/language/range-for)

## 相关头文件

- 无

## 介绍

基于范围的for循环（`range-for`）能够遍历一个范围（例如容器）内的所有元素。语法如下：
```C++
for ( Type var : range ) {
    // 操作
}
// Type是var的类型
// range是一个范围
// var是范围内的元素
// 该for循环依次遍历range中的每一个元素，并赋值给var
```

一个示例：
```C++
#include<iostream>
int main() {
	  int arr[5] = {1, 2, 3, 4, 5};
    for (int val : arr) {
        std::cout << val << std::endl;
    }
}
```
其输出：
```
1
2
3
4
5
```




