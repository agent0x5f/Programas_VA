#pragma once
#include <opencv2/opencv.hpp>

class Filtros {
public:
    static cv::Mat aplicarConvolucion(const cv::Mat& src, const cv::Mat& kernel, bool absoluto);
};