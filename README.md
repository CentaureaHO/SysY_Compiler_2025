# SysyRV64

## 项目说明

小组成员：

1. 2210878 唐显达
2. 2213040 王禹衡

```html
sysyrv64/           # 根目录
├── include/        # 头文件
│   ├── ast/        # 涉及语法解析中各节点的定义
│   ├── common/     # 多场景都可能使用的类的定义
│   ├── llvm_ir/    # 涉及llvm ir代码生成的类的定义
│   └── parser/     # 词法/语法分析类的定义
├── lib/            # 包含SysY语言运行库的定义与链接库
├── parser/         # 包含编译器对词法/语法的定义
│   ├── lexer.l     # flex文件，用于生成词法分析器
│   └── yacc.y      # bison文件，用于生成语法分析器
├── src/            # 源文件，形式同include/
├── testcase/       # 测试源文件与对应输入/输出
├── main.cpp        # 程序主文件
└── Makefile        # 构建脚本
```

## 构建方法

使用Makefile进行项目构建，使用  

```shell
make
```

即可编译，可执行文件保存在相对路径 [bin/SysYc](bin/SysYc)。

## 测试用例

测试用例以及测试脚本来自仓库 [NKU-Compilers2024-RV64GC](https://github.com/yuhuifishash/NKU-Compilers2024-RV64GC)
主程序读取的参数格式与该仓库保持一致，可以直接使用测试脚本

对于代码生成，直接使用

```shell
python3 grade.py 3 0
python3 grade.py 3 1
```

进行测试即可
