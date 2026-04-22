#pragma once
#include <opencv2/opencv.hpp>

class Filtros {
public:
    static cv::Mat aplicarConvolucion(const cv::Mat& src, const cv::Mat& kernel, bool absoluto);
    
    // Nuevos filtros
    static cv::Mat aplicarMinimo(const cv::Mat& src, int dimension);
    static cv::Mat aplicarMaximo(const cv::Mat& src, int dimension);
    static cv::Mat aplicarMediana(const cv::Mat& src, int dimension);
};