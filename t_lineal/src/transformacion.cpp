#include "transformacion.h"
#include <algorithm>

cv::Mat Transformacion::linealAPedazos(const cv::Mat& src, const std::vector<cv::Point>& puntos) {
    if (src.empty()) return src;

    cv::Mat lut(1, 256, CV_8U);
    uchar* p = lut.ptr();

    // Construir la lista completa de nodos de control
    std::vector<cv::Point> nodos;
    nodos.push_back(cv::Point(0, 0)); // Nodo inicial obligatorio
    for (const auto& pt : puntos) {
        nodos.push_back(pt); // Puntos definidos por el usuario
    }
    nodos.push_back(cv::Point(255, 255)); // Nodo final obligatorio

    //Calcular el valor para cada intensidad (0 a 255)
    for (int i = 0; i < 256; ++i) {

        // Buscar en qué tramo cae la intensidad actual 'i'
        for (size_t j = 0; j < nodos.size() - 1; ++j) {
            if (i >= nodos[j].x && i <= nodos[j+1].x) {

                int dx = nodos[j+1].x - nodos[j].x;
                int dy = nodos[j+1].y - nodos[j].y;

                if (dx == 0) {
                    p[i] = cv::saturate_cast<uchar>(nodos[j].y);
                } else {
                    // Ecuación de la recta: y = y1 + (dy/dx)*(x - x1)
                    p[i] = cv::saturate_cast<uchar>(nodos[j].y + (dy * (i - nodos[j].x)) / dx);
                }
                break; // Una vez calculado este píxel, pasamos al siguiente 'i'
            }
        }
    }

    cv::Mat dst;
    cv::LUT(src, lut, dst);
    return dst;
}