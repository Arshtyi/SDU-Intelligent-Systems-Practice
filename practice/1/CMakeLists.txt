cmake_minimum_required(VERSION 3.16)

project(BinomialHeapVisualization LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_executable(BinomialHeapVisualization
    src/main.cpp
    include/BinomialHeap.h
    src/BinomialHeap.cpp
    include/MainWindow.h
    src/MainWindow.cpp
    include/HeapVisualizer.h
    src/HeapVisualizer.cpp
)

target_include_directories(BinomialHeapVisualization PRIVATE include)

target_link_libraries(BinomialHeapVisualization PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)
