# 北航软件学院编译原理

北京航空航天大学2019年秋季编译原理大作业

代码用C++17编写，用了一些新特性。

switch...case语句可能会有bug，其余功能都已通过测试。

因抄袭本代码造成一切后果责任自负。

## 编译方式

在这个README.md所在的目录下：
```
mkdir build
cd build
cmake ..
make
```
运行的说明：
```
Usage:
  ./c0 [options] input [-o file]
or 
  ./c0 [-h]
Options:
  -t        输出词法分析结果
  -s        将输入的 c0 源代码翻译为文本汇编文件
  -c        将输入的 c0 源代码翻译为二进制目标文件
  -h        显示关于编译器使用的帮助
  -o file   输出到指定的文件 file

不提供任何参数时，默认为 -h
提供 input 不提供 -o file 时，默认为 -o out
```

## 完成的实验内容

扩展C0的全部

## 对文法/语义规则进行的改写

不存在改写

## 对未定义行为进行的处理

UB:字面量溢出为自然溢出不报错；
UB:关系表达式不具有语义值；
UB:未初始化的非const变量，默认值为0；
UB:函数中的控制流，如果存在没有返回语句的分支，如果函数返回类型不为void，则默认返回0；
UB:scan和print是目标机有关的内容，当I/O流出现问题时，它们的表现是未定义；
UB:如果default标签不是switch中的最后一个标签，直接报错。

## 目录结构
```
- 根目录
| - 3rd_party/
  | - argparse/ # 272b19b
  | - catch2/
    | - catch2/
      | - catch.hpp
    | - CMakeLists.txt
  | - fmt/ # 1dfa259
| - error/
  | - error.h
| - instruction/
  | - instruction.h
| - tests/
  | - simple_vm.hpp
  | - test_analyser.cpp
  | - test_main.cpp
  | - test_tokenizer.cpp
| - tokenizer/
  | - token.h
  | - tokenizer.cpp
  | - tokenizer.h
  | - utils.hpp
| - analyser/
  | - analyser.cpp
  | - analyser.h
| - .gitignore
| - .gitmodules
| - CMakeLists.txt
| - fmts.hpp
| - main.cpp
| - README.cpp
```