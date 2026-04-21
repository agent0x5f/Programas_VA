#pragma once
#include <opencv2/opencv.hpp>

class Ruido {
public:
    // Agrega ruido con distribucion Gamma
    static cv::Mat gamma(const cv::Mat& src, double forma = 2.0, double escala = 15.0);
};