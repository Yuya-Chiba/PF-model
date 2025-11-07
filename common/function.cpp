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

// 太さ全パターン列挙csvファイルを読み込む 解軌道計算で使用
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