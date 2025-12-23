cmake_minimum_required(VERSION 3.16)

project(BusRouteOptimization LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

include_directories(include)

set(SOURCES
    src/main.cpp
    src/BusSystem.cpp
    src/MainWindow.cpp
)

set(HEADERS
    include/BusSystem.h
    include/MainWindow.h
)

add_executable(BusRouteOptimization ${SOURCES} ${HEADERS})

target_link_libraries(BusRouteOptimization PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)

configure_file(in.txt ${CMAKE_CURRENT_BINARY_DIR}/in.txt COPYONLY)
