#include "ruido.h"

cv::Mat Ruido::gaussiano(const cv::Mat& src, double media, double desviacion) {
    if (src.empty()) return src;

    // Creamos la matriz de ruido con el mismo tamano y numero de canales que la original
    cv::Mat ruido(src.size(), CV_32FC(src.channels()));
    
    // Generamos el ruido gaussiano usando la funcion random de OpenCV
    cv::randn(ruido, media, desviacion);

    // Para evitar desbordamientos al sumar, pasamos la imagen original a formato de punto flotante
    cv::Mat src_flotante, dst_flotante;
    src.convertTo(src_flotante, CV_32F);
    dst_flotante = src_flotante + ruido;

    // Regresamos la imagen a su formato original de 8 bits
    cv::Mat dst;
    // cv::Mat::convertTo evita el desbordamiento por ambos lados internamente
    dst_flotante.convertTo(dst, src.type());

    return dst;
}