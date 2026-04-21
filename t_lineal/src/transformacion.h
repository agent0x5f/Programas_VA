#pragma once
#include <opencv2/opencv.hpp>
#include <vector>

class Transformacion {
public:
    // Aplica una transformación lineal a pedazos basada en n puntos de inflexión.
    static cv::Mat linealAPedazos(const cv::Mat& src, const std::vector<cv::Point>& puntos);
};