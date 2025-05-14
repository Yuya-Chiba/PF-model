#ifndef DRAW_HPP
#define DRAW_HPP

#include <opencv2/opencv.hpp>
#include "particle.hpp"

class Drawer {
private:
    cv::Mat canvas;
    int width, height;

public:
    Drawer(int width = 800, int height = 600);
    void clear(); // 毎フレーム初期化用
    void draw_particle(Particle& p, const cv::Scalar& color);
    void draw_fiber(double x1, double y1, double x2, double y2, double thickness, const cv::Scalar& color);
    void show_param(double x1, double y1, double size, std::string str);
    void show(const std::string& window_name = "PF-model", int delay = 30);
};

#endif