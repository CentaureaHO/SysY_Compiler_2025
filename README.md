# SysyRV64

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
