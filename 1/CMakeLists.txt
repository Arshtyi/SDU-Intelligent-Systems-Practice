cmake_minimum_required(VERSION 3.16)

project(BinomialHeapVisualization LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add Qt installation path to CMAKE_PREFIX_PATH
list(APPEND CMAKE_PREFIX_PATH "C:/Software/QT/6.6.0/msvc2019_64")

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

if(WIN32)
    # Try to find windeployqt in the hinted path
    find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "C:/Software/QT/6.6.0/msvc2019_64/bin")

    if(WINDEPLOYQT_EXECUTABLE)
        # Run windeployqt after build to copy necessary DLLs to the build directory
        add_custom_command(TARGET BinomialHeapVisualization POST_BUILD
            COMMAND "${WINDEPLOYQT_EXECUTABLE}" --no-translations --compiler-runtime "$<TARGET_FILE:BinomialHeapVisualization>"
            COMMENT "Deploying Qt dependencies using windeployqt..."
        )
    else()
        message(WARNING "windeployqt.exe not found. You may need to add Qt bin directory to PATH or copy DLLs manually.")
    endif()
endif()
