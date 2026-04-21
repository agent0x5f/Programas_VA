#include <iostream>
#include <filesystem>
#include <opencv2/opencv.hpp>
#include "io.h"
#include "filtros.h"

namespace fs = std::filesystem;

int main() {
    std::string inputPath = "input.jpg";
    if (!fs::exists(inputPath)) {
        inputPath = "input.png";
        if (!fs::exists(inputPath)) return -1;
    }

    // Carga de imagen original
    cv::Mat imgColor = IO::cargarImagen(inputPath);
    if (imgColor.empty()) return -1;

    // Conversión a escala de grises
    cv::Mat img;
    cv::cvtColor(imgColor, img, cv::COLOR_BGR2GRAY);

    // Definición de kernels Prewitt (3x3)
    cv::Mat kX = (cv::Mat_<float>(3, 3) <<
        -1, 0, 1,
        -1, 0, 1,
        -1, 0, 1);

    cv::Mat kY = (cv::Mat_<float>(3, 3) <<
        -1, -1, -1,
         0,  0,  0,
         1,  1,  1);

    std::cout << "Procesando Prewitt en escala de grises..." << std::endl;

    // Convolución
    cv::Mat gradX = Filtros::aplicarConvolucion(img, kX, true);
    cv::Mat gradY = Filtros::aplicarConvolucion(img, kY, true);

    // Suma
    cv::Mat resultado;
    cv::addWeighted(gradX, 0.5, gradY, 0.5, 0, resultado);

    cv::imwrite("output.jpg", resultado);
    std::cout << "Filtro Prewitt finalizado. Archivo: 'output.jpg'" << std::endl;

    return 0;
}