author: 某23级学生

# 南邮2025年算法课实验

该仓库（预计）存放2025年南邮算法课实验的所有代码，供大家使用。

## 文件结构

- `experiment`: 实验。
- `homework`: 布置的作业，一般以课程的章节分类。
  - `ch1`代表第一章的作业。`1-9.cpp`代表第一章中作业`1-9`对应的代码。
- `reference`: 资料查阅，如果不理解某些代码，可以在这里面找到解释。

## 本仓库代码怪谈

**考虑到算法课使用C++语言，但南邮的C++课存在以下问题：**

- 对C++的讲解停留在表面，几乎未涉及到标准库。
- 教材内容古老，仍然依据C++98标准编写教材。

**本仓库将假设你：**

- <del>是南邮的。</del>
- 学习过南邮的C++课程（或者掌握了同等程度的知识）。
- 使用Visual Studio 2022（或者只要支持C++14就行）。

**本仓库保证：**

- 遵循C++14标准。也就是说，在VS2022中，不需要调整任何与标准相关的设置就可以使用本仓库的代码。
- 不使用第三方库。也就是说，不需要下载其他库就可以使用本仓库的代码。
- 如果一个题目使用了多个`.cpp`文件，则会额外提供CMake文件。如果使用VS2022，则可以忽略这一条且可以忽略仓库中的`CMakeLists.txt`文件。
- 代码中涉及到的超出南邮C++课程内容范围的知识，都会写在[cpp.md文件](./reference/cpp.md)中列出，且在每个源代码文件中指出该代码中具体使用了哪些超纲知识。

## 免责声明？

本仓库的代码不能保证完全没有错误，但所有代码都会经过测试后上传。

本仓库主要用于帮助完成算法课的实验，但不能作为你学习C++的途径，因为这里面对知识的讲解并不严谨，也不全面，甚至可能有错。我对C++的掌握程度也十分有限。

## 如何使用本仓库

要使用本仓库的话就只要下载下来就可以了。

如果你拥有`git`的话，可以打开命令行，跳转到你想下载的目录下面，然后输入如下命令：

```SHELL
git clone https://github.com/C163q/NJUPT_algorithm_class.git
```

如果没有的话，可以点击右上角绿色的`code`按钮，然后点击`Download ZIP`以下载。

