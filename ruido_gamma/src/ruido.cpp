#include "ruido.h"
#include <random>
#include <vector>

cv::Mat Ruido::gamma(const cv::Mat& src, double forma, double escala) {
    if (src.empty()) return src;

    int total_elementos = src.rows * src.cols * src.channels();
    std::vector<float> vector_ruido(total_elementos);

    // Usar el motor de numeros aleatorios de C++
    std::default_random_engine generador;
    std::gamma_distribution<float> distribucion_gamma(forma, escala);

    for(int i = 0; i < total_elementos; ++i) {
        vector_ruido[i] = distribucion_gamma(generador);
    }

    // Convertimos el std::vector a una cv::Mat de OpenCV
    cv::Mat ruido(src.rows, src.cols, CV_32FC(src.channels()), vector_ruido.data());
    cv::Mat src_flotante, dst_flotante;
    src.convertTo(src_flotante, CV_32F);

    // Para ver la gamma la sumamos
    dst_flotante = src_flotante + ruido;
    cv::Mat dst;
    dst_flotante.convertTo(dst, src.type());

    return dst;
}