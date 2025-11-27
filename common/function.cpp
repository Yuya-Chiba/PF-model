#include "function.hpp"
#include "parameter.hpp"

// 周囲6粒子の初期配置を設定する
Eigen::Array<double, num_outer_particle, 2> set_init_outer_particle() {
  // 粒子数はパラメータファイルに記載
  Eigen::Array<double, num_outer_particle, 2> particle_positions;

  for (int i = 0; i < num_outer_particle; i++) {
    particle_positions(i,0) = init_rf_length * cos(angle*i); // 6粒子だと60度ずつの設置
    particle_positions(i,1) = init_rf_length * sin(angle*i);
  }
  return particle_positions;
}

// 太さ全パターン列挙csvファイルを読み込む 解軌道計算Step1で使用
std::vector<std::array<double, 6>> read_thickness_patterns_from_csv(const std::string& filepath) {
  std::vector<std::array<double, 6>> patterns;
  std::ifstream file(filepath);
  
  if (!file.is_open()) {
    std::cerr << "Error: CSV ファイルが開けませんでした: " << filepath << std::endl;
    return patterns;
  }

  std::string line;
  while (std::getline(file, line)) {
    std::istringstream ss(line);
    std::array<double, 6> pattern;
    std::string value;
    int i = 0;

    while (std::getline(ss, value, ',') && i < 6) {
      try {
        pattern[i++] = std::stod(value);
      } catch (...) {
        std::cerr << "数値変換エラー: " << value << std::endl;
        break;
      }
    }

    if (i == 6) {
      patterns.push_back(pattern);
    }
  }

  file.close();
  return patterns;
}

// 粒子位置とファイバー太さの初期設定を読み込む
void read_position_thickness_from_csv(
  const std::string& filepath,
  std::vector<Eigen::Array<double, num_center_particle, 2>>& center_particle_positions,
  std::vector<Eigen::Array<double, num_outer_particle, 2>>& outer_particle_positions,
  std::vector<Eigen::Array<double, num_radial_fiber, 1>>& radial_fiber_thicknesses
) {
  std::ifstream file(filepath);
  if (!file.is_open()) {
    std::cerr << "Error: CSV ファイルが開けませんでした: " << filepath << std::endl;
  }

  std::string line;
  std::getline(file, line); // 1行目はヘッダーのためスキップ

  while (std::getline(file, line)) {
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
    // 中心位置
    Eigen::Array<double, num_center_particle, 2> cp_position;
    for (int i=0; i<2; i++) cp_position(0,i) = row[i+1];
    center_particle_positions.push_back(cp_position);
    // 外周位置
    Eigen::Array<double, num_outer_particle, 2> op_position;
    for (int j=0; j<6; j++) {
      for (int i=0; i<2; i++) op_position(j,i) = row[2*j+i+3];
    }
    outer_particle_positions.push_back(op_position);
    // 動径太さ
    Eigen::Array<double, num_radial_fiber, 1> rf_thickness;
    for (int j=0; j<6; j++) rf_thickness(j,0) = row[j+15];
    radial_fiber_thicknesses.push_back(rf_thickness);
  }

  file.close();
}

// 出力用csvファイルのヘッダー設定
void set_csv_header(std::ofstream& file) {
  file << "pattern,";
  // 中心粒子位置 1細胞のため1つ
  file << "center_particle_x,center_particle_y";
  // 外周粒子位置
  for (int i=0; i<num_outer_particle; i++) {
    file << ",outer_particle_" << i << "_x,outer_particle_" << i << "_y";
  }
  // 動径ファイバー太さ
  for (int i=0; i<num_radial_fiber; i++) {
    file << ",radial_fiber_" << i << "_thickness";
  }
  // 外周ファイバー太さ
  for (int i=0; i<num_outer_fiber; i++) {
    file << ",outer_fiber_" << i << "_thickness";
  }
  file << '\n';
  file.flush();
}

// 粒子位置, ファイバー太さの書き出し 1行につき1slice
void write_to_csv(
  std::ofstream& file,
  const int pattern_id,
  const Eigen::Array<double, num_center_particle, 2> center_particle_positions,
  const Eigen::Array<double, num_outer_particle, 2> outer_particle_positions,
  const Eigen::Array<double, num_radial_fiber, 1> radial_fiber_thicknesses,
  const Eigen::Array<double, num_outer_fiber, 1> outer_fiber_thicknesses
) {
  file << pattern_id << ",";
  // 中心粒子位置 1細胞のため1つ
  file << center_particle_positions(0,0) << "," << center_particle_positions(0,1);
  // 外周粒子位置
  for (int i=0; i<num_outer_particle; i++) {
    file << "," << outer_particle_positions(i,0) << "," << outer_particle_positions(i,1);
  }
  // 動径ファイバー太さ
  for (int i=0; i<num_radial_fiber; i++) {
    file << "," << radial_fiber_thicknesses(i,0);
  }
  // 外周ファイバー太さ
  for (int i=0; i<num_outer_fiber; i++) {
    file << "," << outer_fiber_thicknesses(i,0);
  }
  file << '\n';
  file.flush();
}