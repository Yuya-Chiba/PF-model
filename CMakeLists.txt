# CMakeのバージョンを設定する
cmake_minimum_required(VERSION 3.13.5)

# プロジェクト名と使用する言語を設定, 実行ファイルをtest.cppから作成 使用しないのでコメントアウト
# project(test_2particle CXX)
# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
# add_executable(test_2particle vector.cpp particle.cpp fiber.cpp force.cpp draw.cpp function.cpp main_2particle.cpp)

# 1細胞PFモデル
project(PF_model CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_executable(PF_model vector.cpp particle.cpp fiber.cpp force.cpp draw.cpp function.cpp parameter.hpp main.cpp)

# 成長方程式までを導入して解軌道を調べる
project(CheckSolutionTrajectory CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_executable(CheckSolutionTrajectory vector.cpp particle.cpp fiber.cpp force.cpp draw.cpp function.cpp parameter.hpp check_solution_trajectory.cpp)

# opencvの導入
find_package(OpenCV REQUIRED)
# target_include_directories(test_2particle PRIVATE ${OpenCV_INCLUDE_DIRS})
# target_link_libraries(test_2particle PRIVATE ${OpenCV_LIBS})
target_include_directories(PF_model PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(PF_model PRIVATE ${OpenCV_LIBS})
target_include_directories(CheckSolutionTrajectory PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(CheckSolutionTrajectory PRIVATE ${OpenCV_LIBS})