#pragma once
#include <opencv2/opencv.hpp>

class Ruido {
public:
    // Agrega ruido con distribucion de campana de Gauss
    static cv::Mat gaussiano(const cv::Mat& src, double media = 0.0, double desviacion = 25.0);
};