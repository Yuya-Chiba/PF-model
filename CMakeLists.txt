# CMakeのバージョンを設定する
cmake_minimum_required(VERSION 3.22.1)
project(PF_model CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ライブラリ導入(opencv/Eigen)
find_package(OpenCV REQUIRED)
find_package (Eigen3 3.4 REQUIRED NO_MODULE)
include_directories(${OpenCV_INCLUDE_DIRS} ${EIGEN3_INCLUDE_DIR})

# 複数のプロジェクトで使用する共通処理をまとめる
add_library(Common common/lib.cpp common/draw.cpp common/equation.cpp common/function.cpp common/parameter.hpp)

# プロジェクト名と使用する言語を設定, 実行ファイルを作成
# 1細胞PFモデル
add_executable(PF_model main.cpp)
target_link_libraries(PF_model Common ${OpenCV_LIBS} Eigen3::Eigen)

# 成長方程式までを導入して解軌道を調べる
# step1: 太さ固定で初期値を決定する
add_executable(Check_init_position check_trajectory_step1.cpp)
target_link_libraries(Check_init_position Common ${OpenCV_LIBS} Eigen3::Eigen)
# step2: 初期値から平衡解までの軌跡を調べる
add_executable(Check_solution_trajectory check_trajectory_step2.cpp)
target_link_libraries(Check_solution_trajectory Common ${OpenCV_LIBS} Eigen3::Eigen)