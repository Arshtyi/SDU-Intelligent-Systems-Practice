// All dependencies for this template
#import "@preview/codly:1.3.0": *
#import "@preview/cetz:0.4.2"
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
    student-id: "202512111715",
    student-name: "Arshtyi",
    date: datetime.today(),
    lab-title: "实验题目",
    class: "你的班级",
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

    set document(title: lab-title, author: student-name)

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
            columns: (3.1fr, 2.7fr, 2.9fr),
            [学号： #student-id], [姓名： #student-name], [班级：#class],
        )
        #v(0em, weak: true)
        #table(
            inset: 0.5em,
            align: left + horizon,
            columns: 4fr,
            [实验题目：#lab-title],
        )
        #v(0em, weak: true)
        #table(
            inset: 0.5em,
            align: left + horizon,
            columns: (2fr, 2fr),
            [实验学时：#exp-time], [实验日期：#date.display("[year].[month].[day]")],
        )
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
    student-id: "202400130242",
    student-name: "彭靖轩",
    date: datetime.today(),
    lab-title: "二项堆ADT",
    class: "24智能",
    exp-time: "4",
)

#show figure.where(kind: "image"): it => {
    set image(width: 67%)
    it
}
#exp-block([
    = 实验环境

    - Windows 11
    - MSVC 2019
    - CMake ^3.16.0
    - QT 6.6.0
])
#exp-block([
    = 实验原理分析
    == 二项树
    二项树$B_k$是一种递归定义的有序树:
    - $B_0$包含一个节点
    - $B_k$由两棵$B_{k-1}$组成:其中一棵树的根节点成为另一棵树的左孩子

    二项树$B_k$有以下性质:
    - $B_k$ 共有 $2^k$ 个节点
    - $B_k$ 的高度为 $k$
    - $B_k$ 的根节点度数为 $k$，其子树从左到右分别为 $B_{k-1}, B_{k-2}, dots.h.c , B_0$
    == 二项堆
    二项堆$H$是二项树的集合,满足:
    + 堆序性:每棵二项树都满足最小堆性质(parent key #sym.lt.eq.slant child key)
    + 度数唯一性:对于任意非负整数$k$，堆中至多包含一棵度数为$k$的二项树

    本项目使用`CPP`实现二项堆ADT,`Node`结构体表示节点,采用Left-Child Right-Sibling表示:
    - `parent`:指向父节点
    - `child`:指向第一个子节点或者说是左孩子
    - `sibling`:指向下一个兄弟节点或者说是右兄弟
    - `degree`:度数

    堆本身由一个链表维护,链表中储存所有二项树的根节点,且按度数升序
])

#exp-block([
    = 实现分析
    == Union
    + Merge:将两个堆的根链表按度数大小归并为一个新的有序链表
    + Link:遍历得到的新链表,对于度数相同的两棵树$x$和$y$:
        - 将key更大的链接为另一棵树的子树
        - 新树的度数加1
        - 有可能需要进行多次这样的处理
    == Insert
    插入元素$x$等价于创建一个仅包含$x$的二项堆($B_0$)并将其与当前堆union
    == Extract Min
    + 首先遍历根链表找到最小值的根节点(为此,本项目也维护了一个`min_node`指针)
    + 从根链表中删除该节点
    + $x$的所有子树本身也是二项树,将他们反转顺序(因为堆的根链表升序但是子节点链表降序),构造出新堆
    + 将新堆与原堆剩余部分union
    == Decrease Key
    将节点 $x$ 的值减小后，可能会破坏堆序性.需要将 $x$ 与其父节点比较，如果 $x$ 比父节点小,则交换两者的key,直到满足堆序性或到达根节点
    == Delete
    删除节点$x$等价于:
    + 调用`decrease_key(x, -infinity)` 将 $x$ 变为堆中最小
    + 调用`extract_min()` 删除该节点
])

#exp-block([
    = 复杂度分析
    设二项堆的节点数为$n$,堆中至多有$floor.l log n floor.r + 1$棵树
    #table(
        columns: (1fr, 1fr, 3fr),
        align: center + horizon,
        table.header([操作], [时间复杂度], [说明]),
        [Make Heap], [$O(1)$], [-],
        [Find Min], [$O(1)$], [依赖于维护的`min_node`指针],
        [Union], [$O(log n)$], [归并两个堆的根链表需要$O(log n)$时间],
        [Insert], [$O(log n)$], [等价于一次Union],
        [Extract Min], [$O(log n)$], [需要遍历根链表找到最小节点,反转并进行Union],
        [Decrease Key], [$O(log n)$], [最坏情况下需要将节点上浮到根节点],
        [Delete], [$O(log n)$], [等价于一次Decrease Key和Extract Min],
    )
])

#exp-block([
    = GUI实现
    本项目使用QT实现简单的GUI
    #figure(image("fig/1.png"), caption: "分别从两个预设数据文件构建二项堆")
    #figure(image("fig/2.png"), caption: "Union到Heap1")
])

#exp-block([
    = 总结
    二项堆通过牺牲常数因子的查找速度,换取极高效的合并操作,适用于那些需要频繁合并多个优先队列的情况并且优于一般的二叉堆

    本项目实现了二项堆的基本操作,并通过QT实现了简单的可视化界面,加深了对二项堆的理解
])

#appendix([
    #code("../../practice/1/include/BinomialHeap.h", "cpp")
    #code("../../practice/1/include/HeapVisualizer.h", "cpp")
    #code("../../practice/1/include/MainWindow.h", "cpp")
    #code("../../practice/1/src/BinomialHeap.cpp", "cpp"),
    #code("../../practice/1/src/HeapVisualizer.cpp", "cpp"),
    #code("../../practice/1/src/main.cpp", "cpp"),
    #code("../../practice/1/src/MainWindow.cpp", "cpp")
    #code("../../practice/1/CMakeLists.txt", "cmake")
    编译与运行

    ```pwsh
    cmake -DCMAKE_PREFIX_PATH="C:/Software/QT/6.6.0/msvc2019_64" -S . -B build
    cmake --build build --config Release
    C:\Software\QT\6.6.0\msvc2019_64\bin\windeployqt.exe .\build\Release\BinomialHeapVisualization.exe
    .\build\Release\BinomialHeapVisualization.exe
    ```
])
