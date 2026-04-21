#pragma once
#include <opencv2/opencv.hpp>

class Especificacion {
public:
    // Aplica la especificacion de histograma.
    // src: Imagen original a modificar.
    // ref: Imagen de referencia cuyo histograma queremos imitar.
    static cv::Mat especificar(const cv::Mat& src, const cv::Mat& ref);
};