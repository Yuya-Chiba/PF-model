#ifndef DRAW_HPP
#define DRAW_HPP

#include <opencv2/opencv.hpp>

class Drawer {
private:
    cv::Mat canvas;
    int width, height;

public:
    Drawer(int width = 800, int height = 600);
    void clear(); // 毎フレーム初期化用
    void draw_particle(float x, float y, float radius, const cv::Scalar& color);
    void draw_fiber(float x1, float y1, float x2, float y2, float thickness, const cv::Scalar& color);
    void show_param(float x1, float y1, float size, std::string str);
    void show(const std::string& window_name = "PF-model", int delay = 30);
};

#endif