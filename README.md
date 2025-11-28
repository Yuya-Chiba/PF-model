# PF-model
PFモデルを段階的に作っていくためのリポジトリ

## ディレクトリ構成
```
.
├── common
│   ├── draw.cpp
│   ├── draw.hpp
│   ├── equation.cpp
│   ├── equation.hpp
│   ├── function.cpp
│   ├── function.hpp
│   ├── lib.cpp
│   └── parameter.hpp
│
├── main.cpp
├── check_trajectory_step1.cpp
│
├── result
├── source
│   └── thickness_patterns.csv
│
└── build <= 配下ファイルはgitignore

```

## オプション: 吸引域・形態調査用の計算
`check_trajectory_option.cpp`はPFモデルと直接関係ないが、解析に使用するため記述している ※いまのところ6粒子専用
粒子座標から細胞面積(Area)とアスペクト比(AR)を計算、ファイバー太さから形態分類を行うことができる

粒子座標から面積を求める公式は、「靴紐の公式」「ガウスの面積公式」
アスペクト比は縦横比から計算
形態分類はファイバー太さをビット列にして対象の形態を探す