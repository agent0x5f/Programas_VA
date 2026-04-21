#pragma once
#include <opencv2/opencv.hpp>
#include <vector>

class Promedio {
public:
    // Promedia una serie de imagenes para reducir el ruido.
    // Recibe un vector que contiene las imagenes cargadas en memoria.
    // Regresa la imagen resultante del promedio.
    static cv::Mat promediar(const std::vector<cv::Mat>& imagenes);
};