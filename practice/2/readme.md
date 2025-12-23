# Bus Route Optimization

## Requirements

主要为对 Dijkstra 算法的优化

### 符号规定

某城市的第$i$条公交路线记作$r_i$(正整数),其路线上的第$j$个站点名称及其坐标记作$s_{ij}(x_{ij} y_{ij})$（站名是一个仅包含大小写英文字母和阿拉伯数字的有限字符串，坐标均为正整数）,价格为$c$,此线路的平均来车时间为$f$,车速恒定$v$（均为正整数）.

### 约定

-   假设车辆不受路途中的所有可能影响
-   假设同一线路任意两个相邻站点之间的路线是直达的
-   同一线路每站点票价相同
-   车辆速度恒定
-   没有其他可能的外部因素

### Input

下方若干行按如上形式给出公交线路,其中第$i$行为:

$
r_i s_{i1} x_{i1} y_{i1} s_{i2} x_{i2} y_{i2} \cdots s_{ij} x_{ij} y_{ij} c f v
$

### Requirements

1. 根据上述数据,建立图
2. 查询给定任意两站点之间花费最少的路径
3. 查询给定任意两站点之间最省时间(不考虑中间等车时间)的路径
4. 查询给定任意两站点之间最省时间(考虑中间等车时间)的路径
5. GUI

### Tips

-   根据不同的优化目标选择不同的优化策略
-   抛弃 CLI

### Output

输出类似如下形式

$
线路r_1:s_{11}->s_{12}->\cdots->s_{1j}换乘r_2:s_{21}->s_{22}->\cdots->s_{2j}->\cdots换乘r_i:s_{i1}->s_{i2}->\cdots->s_{ij}总cost
$

其中,cost 视该查询针对类型而定,可能是花费或用时

## Environment

-   Windows 11
-   CMake ^3.16.0
-   MSVC 2019
-   QT 6.6.0

## Usage

```pwsh
cmake -DCMAKE_PREFIX_PATH="C:/Software/QT/6.6.0/msvc2019_64" -S . -B build
cmake --build build --config Release
C:\Software\QT\6.6.0\msvc2019_64\bin\windeployqt.exe .\build\Release\BusRouteOptimization.exe
.\build\Release\BusRouteOptimization.exe
```
