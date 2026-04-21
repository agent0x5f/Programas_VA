#pragma once
#include <opencv2/opencv.hpp>
#include <vector>

struct DatosHistograma {
    cv::Mat b_hist, g_hist, r_hist; //rgb
    cv::Mat gray_hist; //grises
    double max_freq;
    bool is_gray;
};

class Algoritmo {
public:
    static cv::Mat generarHistograma(const cv::Mat& img_bgr, bool enGrises = false, int width = 600, int height = 450);

private:
    static DatosHistograma calcularFrecuencias(const cv::Mat& img_bgr,bool enGrises);
    static cv::Mat dibujarLienzoHistograma(const DatosHistograma& datos, int width, int height);
};