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

    // Cargamos la imagen
    cv::Mat imgColor = IO::cargarImagen(inputPath);
    if (imgColor.empty()) return -1;

    // Convertir a grises
    cv::Mat img;
    cv::cvtColor(imgColor, img, cv::COLOR_BGR2GRAY);

    // Definimos los kernels de Roberts puros (2x2)
    // pero los metemos en una caja de 3x3 rellena de CEROS
    cv::Mat kX = (cv::Mat_<float>(3, 3) <<
         1,  0,  0,
         0, -1,  0,
         0,  0,  0);

    cv::Mat kY = (cv::Mat_<float>(3, 3) <<
         0,  1,  0,
        -1,  0,  0,
         0,  0,  0);

    std::cout << "Aplicando Roberts en escala de grises con relleno neutro..." << std::endl;

    // Aplicamos las convoluciones
    // Usamos absoluto = true para ver los bordes blancos sobre fondo negro
    cv::Mat gradX = Filtros::aplicarConvolucion(img, kX, true);
    cv::Mat gradY = Filtros::aplicarConvolucion(img, kY, true);

    //  Combinamos
    cv::Mat resultado;
    cv::addWeighted(gradX, 0.5, gradY, 0.5, 0, resultado);

    cv::imwrite("output.jpg", resultado);
    std::cout << "Proceso finalizado. Revisa 'output.jpg'." << std::endl;

    return 0;
}