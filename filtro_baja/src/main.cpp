#include <iostream>
#include <filesystem>
#include <string>
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
    int dimension = 3; // Valor por defecto

    if (argc == 1) {
        std::cout << "Usando kernel Pasa Baja (Promedio) de 3x3 por defecto." << std::endl;
    } else if (argc == 2) {
        try {
            dimension = std::stoi(argv[1]);

            // Validamos que sea un número impar mayor a 1 (ej. 3, 5, 7...)
            if (dimension < 3 || dimension % 2 == 0) {
                std::cerr << "Error: La dimension debe ser un numero impar mayor o igual a 3 (ej. 3, 5, 7)." << std::endl;
                return -1;
            }
            std::cout << "Construyendo kernel Pasa Baja de " << dimension << "x" << dimension << "..." << std::endl;
        } catch (...) {
            std::cerr << "Error: Debes ingresar un numero entero valido para la dimension." << std::endl;
            return -1;
        }
    } else {
        std::cerr << "Error: Solo debes pasar un parametro (el tamano de la matriz)." << std::endl;
        std::cerr << "Ejemplo: ./FiltroPasaBaja 5" << std::endl;
        return -1;
    }

    // Calculamos el valor de cada celda: 1 / (N * N)
    float valorCelda = 1.0f / (dimension * dimension);

    // OpenCV nos permite crear y rellenar toda la matriz en una sola linea
    kernel = cv::Mat(dimension, dimension, CV_32F, cv::Scalar(valorCelda));

    std::cout << "Aplicando convolucion Pasa Baja..." << std::endl;

    // Pasamos 'false' ya que el pasa baja no genera valores negativos
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