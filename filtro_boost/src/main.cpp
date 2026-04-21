#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <cmath>
#include <opencv2/opencv.hpp>
#include "io.h"
#include "filtros.h"

namespace fs = std::filesystem;

int main(int argc, char** argv) {
    std::string inputPath = "input.jpg";
    std::string outputPath = "output.jpg";

    if (!fs::exists(inputPath)) {
        if (fs::exists("input.png")) inputPath = "input.png";
        else {
            std::cerr << "Error: No se encontro 'input.jpg' ni 'input.png'." << std::endl;
            return -1;
        }
    }

    cv::Mat img = IO::cargarImagen(inputPath);
    if (img.empty()) return -1;

    cv::Mat kernel;

    if (argc == 1) {
        std::cout << "Usando kernel High-Boost (Enfoque) de 3x3 por defecto (A=1)." << std::endl;
        // Kernel de High-Boost: El centro compensa a los vecinos (suma = 1)
        kernel = (cv::Mat_<float>(3, 3) <<
            -1, -1, -1,
            -1,  9, -1,
            -1, -1, -1);
    } else {
        int numElementos = argc - 1;
        int dimOriginal = std::round(std::sqrt(numElementos));

        if (dimOriginal * dimOriginal != numElementos) {
            std::cerr << "Error: La cantidad de valores debe formar un cuadrado perfecto." << std::endl;
            return -1;
        }

        int dimFinal = dimOriginal;

        // Si la matriz es par, le sumamos 1 a la dimensión
        if (dimOriginal % 2 == 0) {
            dimFinal = dimOriginal + 1;
            std::cout << "Matriz par detectada (" << dimOriginal << "x" << dimOriginal
                      << "). Redimensionando a " << dimFinal << "x" << dimFinal
                      << " y rellenando bordes con 1's..." << std::endl;
        } else {
            std::cout << "Construyendo kernel High-Boost personalizado de " << dimFinal << "x" << dimFinal << "..." << std::endl;
        }

        // Inicializamos con 1.0f para rellenar los bordes extra
        kernel = cv::Mat(dimFinal, dimFinal, CV_32F, cv::Scalar(1.0f));

        for (int i = 0; i < numElementos; ++i) {
            try {
                int fila = i / dimOriginal;
                int col = i % dimOriginal;
                kernel.at<float>(fila, col) = std::stof(argv[i + 1]);
            } catch (...) {
                std::cerr << "Error: Todos los parametros del kernel deben ser numeros validos." << std::endl;
                return -1;
            }
        }
    }

    std::cout << "Aplicando convolucion High-Boost..." << std::endl;

    cv::Mat resultado = Filtros::aplicarConvolucion(img, kernel, false);

    if (!resultado.empty()) {
        cv::imwrite(outputPath, resultado);
        std::cout << "Imagen filtrada guardada correctamente como '" << outputPath << "'." << std::endl;
    } else {
        std::cerr << "Error interno al aplicar el filtro." << std::endl;
        return -1;
    }

    return 0;
}