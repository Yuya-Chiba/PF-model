# CMakeのバージョンを設定する
cmake_minimum_required(VERSION 3.13.5)

# プロジェクト名と使用する言語を設定, 実行ファイルをtest.cppから作成
project(test_2particle CXX)
add_executable(test_2particle vector.cpp particle.cpp fiber.cpp draw.cpp function.cpp main_2particle.cpp)
project(PF_model CXX)
add_executable(PF_model vector.cpp particle.cpp fiber.cpp draw.cpp function.cpp main.cpp)

# opencvの導入
find_package(OpenCV REQUIRED)
target_include_directories(test_2particle PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(test_2particle PRIVATE ${OpenCV_LIBS})
target_include_directories(PF_model PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(PF_model PRIVATE ${OpenCV_LIBS})