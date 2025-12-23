# Binomial Heap ADT

## Requirements

1.  设计并实现 Binomial Heap ADT,包括：
    -   make_heap ():初始化空堆
    -   find_min():返回一个指向最小关键字元素的指针
    -   union(rhs):将 rhs 堆合并到当前堆,rhs 置空
    -   insert(x):插入元素 x
    -   extract_min():从堆中删除最小关键字元素
    -   decrease_key (x,k):将元素 x 的关键字赋值 k
    -   delete(x):删除元素 x
2.  复杂度分析
3.  GUI

## Tips

-   预留文件接口(`1_in.txt`和`2_in.txt`,全部视为 insert)用于读取预设数据集
-   仅支持 GUI 操作,抛弃了 CLI

## Environment

-   Windows 11
-   CMake ^3.16.0
-   MSVC 2019
-   QT 6.6.0

## Usage

```pwsh
cmake -DCMAKE_PREFIX_PATH="C:/Software/QT/6.6.0/msvc2019_64" -S . -B build
cmake --build build --config Release
C:\Software\QT\6.6.0\msvc2019_64\bin\windeployqt.exe .\build\Release\BinomialHeapVisualization.exe
.\build\Release\BinomialHeapVisualization.exe
```
