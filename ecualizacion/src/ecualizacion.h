#pragma once
#include <opencv2/core.hpp>

class Ecualizacion {
public:
    // Aplica la ecualización del histograma a una imagen a color
    static cv::Mat ecualizar(const cv::Mat& src);
};