#include "io.h"

cv::Mat IO::cargarImagen(const std::string& ruta) {
    // Carga la imagen a color por defecto
    return cv::imread(ruta, cv::IMREAD_COLOR);
}