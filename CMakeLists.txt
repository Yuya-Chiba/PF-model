# CMakeのバージョンを設定する
cmake_minimum_required(VERSION 3.22.1)
project(PF_model CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ライブラリ導入(opencv/Eigen)
find_package(OpenCV REQUIRED)
find_package (Eigen3 3.4 REQUIRED NO_MODULE)
include_directories(${OpenCV_INCLUDE_DIRS} ${EIGEN3_INCLUDE_DIR})

# 共通ライブラリ
add_library(common lib.cpp draw.cpp function.cpp parameter.hpp)

# プロジェクト名と使用する言語を設定, 実行ファイルを作成
# 1細胞PFモデル
add_executable(PF_model main.cpp)
target_link_libraries(PF_model common ${OpenCV_LIBS} Eigen3::Eigen)

# 成長方程式までを導入して解軌道を調べる
#add_executable(check_solution_trajectory.cpp)