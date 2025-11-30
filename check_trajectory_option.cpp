#include "common/draw.hpp"
#include "common/equation.hpp"
#include "common/function.hpp"
#include "common/parameter.hpp"

// Option: 粒子位置から円形度・アスペクト比算出、ファイバー太さから形態分類

double calc_area(const Eigen::Array<double, num_outer_particle, 2>& outer_particle_positions);
double calc_aspect_ratio(const Eigen::Array<double, num_outer_particle, 2>& outer_particle_positions);
std::string classify_mode(const Eigen::Array<double, num_radial_fiber, 1>& radial_fiber_thicknesses);

int main() {

  std::ifstream input_file("../csv/result/equilibrium_solutions.csv"); // 入力用
  std::ofstream output_file("../csv/result/equilibrium_solutions_mode.csv"); // 出力用
  output_file << "pattern,area,aspect_ratio,mode\n";
  std::string line;
  std::getline(input_file, line); // 1行目はヘッダーのためスキップ
  // 1行ごとに算出
  while (std::getline(input_file, line)) {
    std::istringstream ss(line);
    std::vector<double> row;
    std::string value;
    int i = 0;

    while (std::getline(ss, value, ',')) {
      try {
        row.push_back(std::stod(value));
      } catch (...) {
        std::cerr << "数値変換エラー: " << value << std::endl;
        break;
      }
    }
    // 外周位置の取得
    Eigen::Array<double, num_outer_particle, 2> outer_particle_positions;
    for (int j=0; j<6; j++) {
      for (int i=0; i<2; i++) outer_particle_positions(j,i) = row[2*j+i+3];
    }
    // 動径太さの取得
    Eigen::Array<double, num_radial_fiber, 1> radial_fiber_thicknesses;
    for (int j=0; j<6; j++) radial_fiber_thicknesses(j,0) = row[j+15];

    double area = calc_area(outer_particle_positions);
    double aspect_ratio = calc_aspect_ratio(outer_particle_positions);
    std::string mode = classify_mode(radial_fiber_thicknesses);
    output_file << row[0] <<"," << area << "," << aspect_ratio << "," << mode << "\n";
    output_file.flush();
  }
  std::cout << "出力完了" << std::endl;
  return 0;
}


// 粒子座標から面積を計算する 座標は連続する必要がある
double calc_area(const Eigen::Array<double, num_outer_particle, 2>& outer_particle_positions) {
  double area = 0;
  for(int i=0; i<num_outer_particle; i++) {
    area += outer_particle_positions(i,0)*outer_particle_positions((i+1)%num_outer_particle,1) - outer_particle_positions((i+1)%num_outer_particle,0)*outer_particle_positions(i,1);
  }
  area = 0.5*std::abs(area);
  return area;
}

// 粒子座標からアスペクト比を計算する
double calc_aspect_ratio(const Eigen::Array<double, num_outer_particle, 2>& outer_particle_positions) {
  // 各粒子の重心をもとに中心化
  double mean_x = outer_particle_positions.col(0).mean();
  double mean_y = outer_particle_positions.col(1).mean();
  Eigen::Matrix<double, num_outer_particle, 2> centered_positions = outer_particle_positions.matrix();
  centered_positions.col(0).array() -= mean_x;
  centered_positions.col(1).array() -= mean_y;

  // 分散共分散行列の分散を計算
  Eigen::Matrix2d cov = (centered_positions.transpose() * centered_positions) / num_outer_particle;
  Eigen::SelfAdjointEigenSolver<Eigen::Matrix2d> solver(cov);
  Eigen::Vector2d eigenvalues = solver.eigenvalues();

  if (eigenvalues[0] == 0) {
    return 1.0;
  } else {
    return std::sqrt(eigenvalues[1]) / std::sqrt(eigenvalues[0]);
  }
}

// ファイバー太さから形態分類を行う 6粒子専用
std::string classify_mode(const Eigen::Array<double, num_radial_fiber, 1>& radial_fiber_thicknesses) {
  if (radial_fiber_thicknesses.size() != 6) {
    std::cout << "Error: ファイバー数が6以外です" << std::endl;
    return "error";
  }
  // ビット列に変換
  int bits = 0;
  for(int i=0; i<6; i++) {
    if (radial_fiber_thicknesses(i,0) > q_threshold) bits |= (1 << i);
  }
  // ビット列の回転/反転を行い、最小値を算出
  int min_bits = bits;
  int current_bits = bits;
  for(int i=0; i<6; i++) {
    current_bits = ((current_bits & 1) << 5) | (current_bits >> 1);
    min_bits = std::min(current_bits, min_bits);
  }
  int reversed_bits = 0;
  for(int i=0; i<6; i++) {
    if ((bits >> i) & 1) reversed_bits |= (1 << (5-i));
  }
  current_bits = reversed_bits;
  for(int i=0; i<6; i++) {
    current_bits = ((current_bits & 1) << 5) | (current_bits >> 1);
    min_bits = std::min(current_bits, min_bits);
  }
  // min_bitsから形態分類
  switch (min_bits) {
    case 0b000000: return "N";
    case 0b000001: return "M1";
    case 0b000011: return "M2";
    case 0b000111: return "M3";
    case 0b001111: return "M4";
    case 0b011111: return "M5";
    case 0b000101: return "B1";
    case 0b001001: return "B2";
    case 0b001011: return "B3";
    case 0b010111: return "B4";
    case 0b011011: return "B5";
    case 0b010101: return "T";
    case 0b111111: return "F";
    default: return "Unknown";
  }
}