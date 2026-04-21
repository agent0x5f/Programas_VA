#include "ruido.h"
#include <vector>

cv::Mat Ruido::salYPimienta(const cv::Mat& src, float prob_sal, float prob_pimienta) {
    if (src.empty()) return src;
    cv::Mat dst = src.clone();
    
    // Matriz de ruido de 1 solo canal para decidir dónde cae el condimento
    cv::Mat ruido(src.size(), CV_8U);
    cv::randu(ruido, 0, 255); // Llenado aleatorio con valores de 0 a 255
    
    // Aplicar Sal (Píxeles a 255). Si el valor aleatorio es menor al % de probabilidad
    dst.setTo(cv::Scalar::all(255), ruido < (prob_sal * 255.0f));
    
    // Aplicar Pimienta (Píxeles a 0). Si el valor aleatorio es mayor al límite superior
    dst.setTo(cv::Scalar::all(0), ruido > (255.0f - (prob_pimienta * 255.0f)));

    return dst;
}