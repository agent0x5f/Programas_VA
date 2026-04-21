#include "ecualizacion.h"
#include <opencv2/imgproc.hpp>
#include <vector>

cv::Mat Ecualizacion::ecualizar(const cv::Mat& src) {
    if (src.empty()) return src;

    // Convertir de BGR a HSV
    cv::Mat res;
    cv::cvtColor(src, res, cv::COLOR_BGR2HSV);

    // Separar los 3 canales
    std::vector<cv::Mat> canales;
    cv::split(res, canales);

    // Iniciamos la ecualización del histograma en la luminancia
    cv::Mat canalY = canales[2];
    int N = canalY.rows * canalY.cols; // Total de píxeles en la imagen

    // Calcular las frecuencias absolutas (n_k)
    int n[256] = {0};
    for (int i = 0; i < canalY.rows; i++) {
        const uchar* fila = canalY.ptr<uchar>(i);
        for (int j = 0; j < canalY.cols; j++) {
            n[fila[j]]++;
        }
    }

    // Probabilidad, Suma Acumulada y Mapeo a (L-1)
    cv::Mat lut(1, 256, CV_8U);
    uchar* p = lut.ptr();
    double suma_probabilidades = 0.0;

    for (int k = 0; k < 256; k++) {
        // Probabilidad del nivel actual (n_k / N)
        double prob = (double)n[k] / (double)N;
        // Suma acumulada de probabilidades
        suma_probabilidades += prob;
        // Transformación: (L-1) * suma_acumulada, donde L-1 = 255
        p[k] = cv::saturate_cast<uchar>(cvRound(255.0 * suma_probabilidades));
    }
    
    // Aplicar la tabla de transformación al canal Y original
    cv::LUT(canalY, lut, canales[2]);

    // Volver a fusionar los canales
    cv::Mat res_ecualizado;
    cv::merge(canales, res_ecualizado);

    // Convertir de regreso al espacio de color BGR original
    cv::Mat dst;
    cv::cvtColor(res_ecualizado, dst, cv::COLOR_HSV2BGR);

    return dst;
}