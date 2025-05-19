# CMakeのバージョンを設定する
cmake_minimum_required(VERSION 3.13.5)

# プロジェクト名と使用する言語を設定, 実行ファイルをtest.cppから作成
project(PF_model CXX)
# add_executable(PF_model main.cpp draw.cpp particle.cpp) 以前のプロジェクト
add_executable(PF_model main.cpp draw.cpp particle.cpp fiber.cpp)

# opencvの導入
find_package(OpenCV REQUIRED)
target_include_directories(PF_model PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(PF_model PRIVATE ${OpenCV_LIBS})