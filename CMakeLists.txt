# CMakeのバージョンを設定する
cmake_minimum_required(VERSION 3.13.5)

# プロジェクト名と使用する言語を設定, 実行ファイルをtest.cppから作成
project(PF_model CXX)
add_executable(PF_model test.hpp test.cpp)

project(hello_a CXX)
add_executable(hello_a hello.cpp)

# opencvの導入
find_package(OpenCV REQUIRED)
target_include_directories(hello_a PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(hello_a PRIVATE ${OpenCV_LIBS})