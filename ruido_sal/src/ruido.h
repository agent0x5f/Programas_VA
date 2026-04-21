#pragma once
#include <opencv2/opencv.hpp>

class Ruido {
public:
    // Agrega píxeles completamente blancos (sal) y completamente negros (pimienta)
    static cv::Mat salYPimienta(const cv::Mat& src, float prob_sal = 0.10f, float prob_pimienta = 0.10f);
};