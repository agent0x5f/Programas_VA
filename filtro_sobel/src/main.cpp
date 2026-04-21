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

    // Conversión a escala de grises para un gradiente limpio
    cv::Mat img;
    cv::cvtColor(imgColor, img, cv::COLOR_BGR2GRAY);

    // Definición de kernels Sobel (3x3)
    cv::Mat kX = (cv::Mat_<float>(3, 3) <<
        -1, 0, 1,
        -2, 0, 2,
        -1, 0, 1);

    cv::Mat kY = (cv::Mat_<float>(3, 3) <<
        -1, -2, -1,
         0,  0,  0,
         1,  2,  1);

    std::cout << "Procesando Sobel en escala de grises..." << std::endl;

    // Convolución con valor absoluto para detectar cambios en ambas direcciones
    cv::Mat gradX = Filtros::aplicarConvolucion(img, kX, true);
    cv::Mat gradY = Filtros::aplicarConvolucion(img, kY, true);

    // Fusión de gradientes
    cv::Mat resultado;
    cv::addWeighted(gradX, 0.5, gradY, 0.5, 0, resultado);

    cv::imwrite("output.jpg", resultado);
    std::cout << "Filtro Sobel finalizado. Archivo: 'output.jpg'" << std::endl;

    return 0;
}