#include "especificacion.h"
#include <opencv2/imgproc.hpp>
#include <vector>
#include <cmath>

cv::Mat Especificacion::especificar(const cv::Mat& src, const cv::Mat& ref) {
    if (src.empty() || ref.empty()) return src;

    // Convertir ambas imagenes de BGR a HSV
    cv::Mat a_src, b_ref;
    cv::cvtColor(src, a_src, cv::COLOR_BGR2HSV);
    cv::cvtColor(ref, b_ref, cv::COLOR_BGR2HSV);

    // Separar canales y tomar la Luminancia (Canal Y)
    std::vector<cv::Mat> canales_src, canales_ref;
    cv::split(a_src, canales_src);
    cv::split(b_ref, canales_ref);

    cv::Mat y_src = canales_src[2];
    cv::Mat y_ref = canales_ref[2];

    // Calcular histogramas de los canales Y
    int histSize = 256;
    float range[] = { 0, 256 };
    const float* histRange[] = { range };

    cv::Mat hist_src, hist_ref;
    cv::calcHist(&y_src, 1, 0, cv::Mat(), hist_src, 1, &histSize, histRange, true, false);
    cv::calcHist(&y_ref, 1, 0, cv::Mat(), hist_ref, 1, &histSize, histRange, true, false);

    // Calcular probabilidades acumuladas (Suma acumulada / Total de pixeles)
    float total_src = y_src.rows * y_src.cols;
    float total_ref = y_ref.rows * y_ref.cols;

    std::vector<float> cdf_src(256, 0.0f);
    std::vector<float> cdf_ref(256, 0.0f);

    cdf_src[0] = hist_src.at<float>(0) / total_src;
    cdf_ref[0] = hist_ref.at<float>(0) / total_ref;

    for (int i = 1; i < 256; i++) {
        cdf_src[i] = cdf_src[i - 1] + (hist_src.at<float>(i) / total_src);
        cdf_ref[i] = cdf_ref[i - 1] + (hist_ref.at<float>(i) / total_ref);
    }

    // Construir la tabla cruzando los valores
    cv::Mat lut(1, 256, CV_8U);
    uchar* p = lut.ptr();

    for (int i = 0; i < 256; i++) {
        // Para cada nivel en la imagen original, buscamos el nivel j en la referencia
        // cuya probabilidad acumulada sea la mas cercana matematicamente.
        int j_closest = 0;
        float min_diff = 2.0f; // La max diferencia posible entre porcentajes es 1.0

        for (int j = 0; j < 256; j++) {
            float diff = std::abs(cdf_src[i] - cdf_ref[j]);
            if (diff < min_diff) {
                min_diff = diff;
                j_closest = j;
            }
        }
        // Guardamos el mapeo
        p[i] = cv::saturate_cast<uchar>(j_closest);
    }

    // Aplicar la tabla al canal Y origen
    cv::LUT(canales_src[2], lut, canales_src[2]);

    // Regresar a BGR
    cv::Mat dst;
    cv::merge(canales_src, dst);
    cv::cvtColor(dst, dst, cv::COLOR_HSV2BGR);

    return dst;
}