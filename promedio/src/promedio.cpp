#include "promedio.h"

cv::Mat Promedio::promediar(const std::vector<cv::Mat>& imagenes) {
    if (imagenes.empty()) return cv::Mat();
    if (imagenes.size() == 1) return imagenes[0].clone();

    // Obtener las dimensiones de la primera imagen como referencia
    int filas = imagenes[0].rows;
    int columnas = imagenes[0].cols;
    int tipo = imagenes[0].type();

    // Validar que todas las imagenes tengan el mismo tamano y numero de canales
    for (size_t i = 1; i < imagenes.size(); i++) {
        if (imagenes[i].rows != filas || imagenes[i].cols != columnas || imagenes[i].type() != tipo) {
            return cv::Mat(); // Retornamos matriz vacia como codigo de error
        }
    }

    // Crear una matriz acumuladora de 32-bits para evitar desbordes
    // Usamos 3 canales si es a color, 1 canal si es a escala de grises
    cv::Mat acumulador = cv::Mat::zeros(filas, columnas, (imagenes[0].channels() == 3) ? CV_32FC3 : CV_32FC1);

    // Sumar todas las imagenes en el acumulador
    for (const auto& img : imagenes) {
        cv::Mat img_flotante;
        // Convertir la imagen actual de 8 bits a 32 bits de punto flotante
        img.convertTo(img_flotante, acumulador.type());
        acumulador += img_flotante;
    }

    // Dividir entre cantidad de imagenes
    acumulador /= (double)imagenes.size();
    
    // Convertir de regreso al formato original de 8 bits
    cv::Mat dst;
    acumulador.convertTo(dst, tipo);

    return dst;
}