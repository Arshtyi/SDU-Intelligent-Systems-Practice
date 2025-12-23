// All dependencies for this template
#import "@preview/codly:1.3.0": *
#import "@preview/cetz:0.4.2"
#import "@preview/subpar:0.2.2"
#import "@preview/numbly:0.1.0": numbly

// Almost all settings.
// Ref:https://github.com/Dregen-Yor/sdu-exp-report

#let font = (
    main: "Source Han Serif SC",
    mono: "IBM Plex Mono",
    cjk: "Noto Serif CJK SC",
)

#let cjk-markers = regex("[“”‘’．，。、？！：；（）｛｝［］〔〕〖〗《》〈〉「」【】『』─—＿·…\u{30FC}]+")

#let 字号 = (
    初号: 42pt,
    小初: 36pt,
    一号: 26pt,
    小一: 24pt,
    二号: 22pt,
    小二: 18pt,
    三号: 16pt,
    小三: 15pt,
    四号: 14pt,
    中四: 13pt,
    小四: 12pt,
    五号: 10.5pt,
    小五: 9pt,
    六号: 7.5pt,
    小六: 6.5pt,
    七号: 5.5pt,
    小七: 5pt,
)

#let report(
    institute: "计算机科学与技术",
    course: "人工智能引论",
    student-id1: "202512111715",
    student-name1: "Arshtyi",
    class1: "你的班级",
    student-id2: "202512111715",
    student-name2: "Arshtyi",
    class2: "你的班级",
    student-id3: "202512111715",
    student-name3: "Arshtyi",
    class3: "你的班级",
    date: datetime.today(),
    lab-title: "实验题目",
    exp-time: "实验时间",
    body,
) = {
    set text(
        font: ("Source Han Serif SC", "Fira Sans"),
        size: 10.5pt,
        lang: "zh",
        region: "cn",
        // leading: 1.6
    )

    set page(
        paper: "a4",
        margin: (top: 2.6cm, bottom: 2.3cm, inside: 2cm, outside: 2cm),
        footer: [
            #set align(center)
            #set text(9pt)
            #context {
                counter(page).display("- 1 -")
            }
        ],
    )

    set document(title: lab-title, author: student-name1)

    set heading(
        numbering: numbly(
            "{1:一、} ",
            "{2:1}.",
            "({3:1})", // here, we only want the 3rd level
        ),
    )
    set par(justify: true)
    show math.equation.where(block: true): it => block(width: 100%, align(center, it))

    set raw(tab-size: 4)
    show raw: set text(font: (font.mono, font.cjk))
    // Display inline code in a small box
    // that retains the correct baseline.
    show raw.where(block: false): box.with(fill: luma(240), inset: (x: 3pt, y: 0pt), outset: (y: 3pt), radius: 2pt)
    show raw: it => {
        show ".": "." + sym.zws
        show "=": "=" + sym.zws
        show ";": ";" + sym.zws
        it
    }
    let style-number(number) = text(gray)[#number]
    show raw.where(block: true): it => {
        align(center)[
            #block(
                fill: luma(240),
                inset: 10pt,
                radius: 4pt,
                width: 100%,
            )[
                #place(top + right, dy: -15pt)[
                    #set text(size: 9pt, fill: white, style: "italic")
                    #block(
                        fill: gray,
                        outset: 4pt,
                        radius: 4pt,
                        // width: 100%,
                        context {
                            it.lang
                        },
                    )
                ]
                #set par(justify: false, linebreaks: "simple")
                #grid(
                    columns: (1em, 1fr),
                    align: (right, left),
                    column-gutter: 0.7em,
                    row-gutter: 0.6em,
                    // stroke: 1pt,
                    ..it.lines.enumerate().map(((i, line)) => (style-number(i + 1), line)).flatten(),
                )

            ]]
    }

    show link: it => {
        set text(fill: blue)
        underline(it)
    }

    set list(indent: 6pt)
    set enum(indent: 6pt)
    set enum(
        numbering: numbly(
            "{1:1})",
            "{2:a}.",
        ),
        full: true,
    )

    counter(page).update(1)
    [
        #show heading: it => {
            set align(center)
            set text(size: 字号.小二, weight: "bold")
            it
        }
        #set text(tracking: 0.1em)
        #heading(numbering: none, depth: 1)[山东大学 #underline(extent: 2pt, [#institute]) 学院\ #underline(
                extent: 2pt,
                [#course],
            ) 课程实验报告]
    ]

    show heading: set block(spacing: 1.5em)
    // show heading: set block(above: 1.4em, below: 1em)

    show heading.where(depth: 1): it => {
        show h.where(amount: 0.3em): none
        set text(size: 字号.小四)
        it
    }

    show heading: it => {
        set text(size: 字号.小四)
        it
    }

    set text(size: 字号.小四)
    set par(first-line-indent: 2em)
    let fakepar = context {
        box()
        v(-measure(block() + block()).height)
    }
    show math.equation.where(block: true): it => it + fakepar // 公式后缩进
    show heading: it => it + fakepar // 标题后缩进
    show figure: it => it + fakepar // 图表后缩进
    show enum: it => {
        // it.numbering + fakepar
        it
        // for item in it.children {
        //   context {
        //     counter(it.numbering).display()
        //   }
        //   [
        //     #item.body
        //   ]
        // }

        fakepar
    }
    // show enum.item: it => {
    //   repr(it)
    //   it
    // }
    show list: it => {
        it
        fakepar
    }
    show grid: it => it + fakepar // 列表后缩进
    show table: it => it + fakepar // 表格后缩进
    show raw.where(block: true): it => it + fakepar

    [
        #set par(justify: true)
        #set text(size: 字号.小四)
        #table(
            align: left + horizon,
            inset: 0.5em,
            columns: (3fr, 2.5fr, 3fr),
            [学号： #student-id1], [姓名： #student-name1], [班级：#class1],
        )
        #v(0em, weak: true)
        #table(
            align: left + horizon,
            inset: 0.5em,
            columns: (3fr, 2.5fr, 3fr),
            [学号： #student-id2], [姓名： #student-name2], [班级：#class2],
        )
        #v(0em, weak: true)
        #table(
            align: left + horizon,
            inset: 0.5em,
            columns: (3fr, 2.5fr, 3fr),
            [学号： #student-id3], [姓名： #student-name3], [班级：#class3],
        )
        #v(0em, weak: true)
        #table(
            inset: 0.5em,
            align: left + horizon,
            columns: 4fr,
            [实验题目：#lab-title],
        )
        //     #v(0em, weak: true)
        //     #table(
        //         inset: 0.5em,
        //         align: left + horizon,
        //         columns: (2fr, 2fr),
        //         [实验学时：#exp-time], [实验日期：#date.display("[year].[month].[day]")],
        //     )
    ]
    v(0em, weak: true)
    show heading.where(depth: 1): it => {
        show h.where(amount: 0.3em): none
        set text(size: 字号.小四)
        [
            #block(
                width: 100%,
                inset: 0em,
                stroke: none,
                breakable: true,
                it,
            )
        ]
    }
    body
}
#let exp-block(content) = {
    v(0em, weak: true)
    set par(first-line-indent: (amount: 2em, all: true)) // Use this to make sure all paragraphs are indented.Ref:https://github.com/typst/typst/pull/5768
    block(
        width: 100%,
        inset: 1em,
        stroke: 1pt,
        breakable: true,
        content + v(1em),
    )
}

#let code(path, lang) = {
    raw(read(path), lang: lang, block: true)
}// Read code from file

#let appendix(body) = {
    exp-block([
        #align(center)[
            #heading(numbering: none, depth: 1)[#text(size: 15pt)[附录]]
        ]
        #body
    ])
}

#show: report.with(
    institute: "计算机科学与技术",
    course: "智能系统综合实践",
    student-id1: "202400130242",
    student-name1: "彭靖轩",
    class1: "24智能",
    student-name2: "孙晨涵",
    student-id2: "202400130112",
    class2: "24数据",
    student-name3: "崔家瑞",
    student-id3: "202400202035",
    class3: "24数据",
    date: datetime.today(),
    lab-title: "Bus Route Optimization",
    exp-time: "4",
)

#show figure.where(kind: "image"): it => {
    set image(width: 67%)
    it
}

#exp-block([
    = 实验环境
    - Windows 11
    - CMake ^3.16.0
    - MSVC 2019
    - QT 6.6.0
])

#exp-block([
    = 实验原理分析
    == 算法介绍
    Djikstra算法是一种用于计算图中单源最短路径的经典算法.它适用于边权非负的有向图和无向图.该算法通过贪心策略,逐步扩展已知最短路径的节点集合,直到找到从起点到所有其他节点的最短路径.
    == 边权计算
    本项目中,我们使用Djikstra算法来优化公交线路.其关键在于边权$W$的计算不同:
    + 最小花费
        $
            W = cases(
                0 & "if" "cur"_("route")="last"_("route"),
                "Route.Cost" & "if" "cur"_("route")eq.not"last"_("route"),
            )
        $
    + 最少时间(不含等待)
    $
        W=("Distance"paren.l u,v paren.r)/("Route.Speed")
    $
    + 最少时间(含等待)
    $
        W=("Distance"paren.l u,v paren.r)/("Route.Speed")+cases(
            0 & "if" "cur"_("route")="last"_("route"),
            "Route.Frequency" & "if" "cur"_("route")eq.not"last"_("route"),
        )
    $
    == 模型建立
    === 数据
    从`in.txt`从读取合法的输入数据并处理为:
    - `Stop`:存储站点名称、坐标和一个唯一确定的ID
    - `Route`:储存线路ID、经过的站点序列、票价、速度和发车频率
    === 模型构建
    构建一个State-Space-Graph:
    - `State`:(当前站点$u$,到达该站点的线路$"last"_"route"$),这容易区分同路线行驶和换乘
    - 转移:从站点$u$通过线路$"cur"_"route"$到邻居站点$v$的转移:
    $
        cases(
            "连续乘坐,无换乘成本" & "if" "cur"_("route")="last"_("route"),
            "换乘,增加换乘成本(票价或等待时间)" & "if" "cur"_("route")eq.not"last"_("route"),
        )
    $
    == GUI
    === 可视化框架
    - 使用 `QGraphicsScene` 管理所有图元
    - 使用 `QGraphicsView` 进行渲染.继承并自定义 `ZoomableGraphicsView`以支持缩放和拖拽,重写`wheelEvent` 以实现鼠标滚轮缩放
    === 交互设计
    - 启动默认载入`in.txt`
    - 用户选择起点和终点站后,选择优化目标,点击查询,图中将显示最优路径(红色)并在顶栏显示路径信息
    === 界面展示
    @full 给出了程序的图形界面截图
    #subpar.grid(
        figure(image("fig/1.png")), <a>,

        figure(image("fig/2.png")), <b>,

        figure((image("fig/3.png"))), <c>,

        columns: (1fr, 1fr, 1fr),
        caption: [图形界面],
        label: <full>,
    )
])

#exp-block([
    = 实现细节
    == 算法流程
    + 初始化距离矩阵 `dist[numStops][numRoutes + 1]` 为无穷大
    + 将起点状态 `(startNode, -1)` 压入优先队列,距离设为 0
    + 当队列不为空
        - 取出当前代价最小的状态 `(u, lastR)`
        - 遍历经过站点 `u` 的所有路线 `r_new`
        - 在路线 `r_new` 上找到 `u` 的相邻站点 `v`
        - 计算从 `(u, lastR)` 到 `(v, r_new)` 的边权 `w`
        - 如果 `dist[u][lastR] + w < dist[v][r_new]`,则更新距离并压入队列
    + 最终在终点 `endNode` 的所有可能到达状态中取最小值
])
#exp-block([
    = 复杂度分析
    设
    - $n$:站点数量
    - $m$:路线数量
    - $k$:每个站点平均经过的路线数
    那么
    + 状态数:节点数$n paren.l m+1 paren.r$,实际上每个站点仅与经过它的$k$条路线相关,因此有效状态数$V approx n k$
    + 边数:算法经过`u`的所有路线,对于每条路线检查前后邻居,因此每个状态约$k$条出边.总边数$E approx V dot k approx n k^2$
    + 总的复杂度
    $
        O(E log V) = O(n k^2 log(n k))
    $
    考虑到实际情况中$k$通常较小且近似为常数,因此复杂度接近于$O(n log n)$
])
#appendix([
    #code("../../practice/2/include/BusSystem.h", "cpp")
    #code("../../practice/2/include/MainWindow.h", "cpp")
    #code("../../practice/2/src/BusSystem.cpp", "cpp")
    #code("../../practice/2/src/MainWindow.cpp", "cpp")
    #code("../../practice/2/src/main.cpp", "cpp")
    #code("../../practice/2/CMakeLists.txt", "cmake")
    ```pwsh
    cmake -DCMAKE_PREFIX_PATH="C:/Software/QT/6.6.0/msvc2019_64" -S . -B build
    cmake --build build --config Release
    C:\Software\QT\6.6.0\msvc2019_64\bin\windeployqt.exe .\build\Release\BusRouteOptimization.exe
    .\build\Release\BusRouteOptimization.exe
    ```
])
