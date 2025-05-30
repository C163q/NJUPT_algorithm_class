
## 各文件夹存放的文件

- `experiment`: 实验。
  - 示例：`exp1`代表第一次实验。
  - 以`_norm`结尾的文件代表以应试的角度编写的代码，这些代码几乎不使用标准库，涉及内容基础，容易学习，也方便阅卷人批阅。
  - 以`_spec`结尾的文件代表**在受到题目给出的API的限制的情况下**，使用标准库，尽量写出较为规范的代码（或许）。
- `homework`: 布置的作业，一般以课程的章节分类。
  - 示例：`ch1`代表第一章的作业。`1-9.cpp`代表第一章中作业`1-9`对应的代码。
- `reference`: 资料查阅，如果不理解某些代码，可以在这里面找到解释。

## 一些有意思的文件

- [5-11.cpp](./homework/ch5/5-11.cpp): 如果你懒得去做“写出快速排序过程”的题目，只要修改这个文件中的`main()`函数中`std::vector`当中的元素，就可以自动帮你生成“快速排序过程”，非常方便，节省你的脑细胞。

## experiment文件夹

### exp1

- [sort_norm.cpp](./experiment/exp1/sort_norm.cpp): 包含**实验一**中的**两路合并排序和快速排序**的代码，基本上就是实验说明里面给出的内容，稍加修改得到的。**如果是放到实验报告里的代码，建议用这个文件里的。**
- [sort_spec.cpp](./experiment/exp1/sort_spec.cpp): 包含**实验一**中的**两路合并排序和快速排序**的代码，（限制必须）使用实验要求里面给出的API，但使用允许使用较多的标准库，尽量写出符合规范的、简洁的(?)代码。
- [nth-element_norm.cpp](./experiment/exp1/nth-element_norm.cpp): 包含**实验一**中的**寻找第k大的元素**的代码，基本上就是实验说明里面给出的内容，稍加修改得到的。**如果是放到实验报告里的代码，建议用这个文件里的。**
- [nth-element_spec.cpp](./experiment/exp1/nth-element_spec.cpp): 包含**实验一**中的**寻找第k大的元素**的代码，（限制必须）使用实验要求里面给出的API，但使用允许使用较多的标准库，尽量写出符合规范的、简洁的(?)代码。


## homework文件夹

### ch1

- [1-9.cpp](./homework/ch1/1-9.cpp): 包含**作业1-9**中**递归计算n!**的代码，`factorial()`函数就是解答。
- [1-13.cpp](./homework/ch1/1-13.cpp): 包含**作业1-13**中**递归和迭代计算二项式系数**的代码，`binomial_coefficient_1()`和`binomial_coefficient_2()`函数就是解答。

### ch5

- [5-8.cpp](./homework/ch5/5-8.cpp): 包含**作业5-8**中**三分搜索**的代码，`search()`（两个）函数就是解答。此外，`search3()`是更加符合规范的代码。
- [5-11.cpp](./homework/ch5/5-11.cpp): 包含**作业5-11**中**写出快速排序过程**的代码，由于原题不是编程题，因此实现当中不受标准库使用的限制。这个文件中的代码可以对任意待排数组都输出写出快速排序过程。
