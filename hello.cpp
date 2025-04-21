#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

int main(int argc, char** argv) {
    cv::Mat img = cv::Mat::zeros(400, 512, CV_8UC3);

    cv::line(img, cv::Point(100, 100), cv::Point(400, 105), cv::Scalar(0,0,200), 3, cv::LINE_4);
    cv::line(img, cv::Point(100, 200), cv::Point(400, 205), cv::Scalar(0,200,0), 5, cv::LINE_8);
    cv::line(img, cv::Point(100, 300), cv::Point(400, 305), cv::Scalar(200,0,0), 10, cv::LINE_AA);

    cv::namedWindow("drawing", cv::WINDOW_AUTOSIZE | cv::WINDOW_FREERATIO);
    cv::imshow("drawing", img);
    cv::waitKey(0);
    return 0;
}
