#pragma once
#include <opencv2/opencv.hpp>
#include <string>

namespace IO {
    // Carga la imagen desde el disco
    cv::Mat cargarImagen(const std::string& ruta);
}