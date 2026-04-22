#include <iostream>
#include <filesystem>
#include <string>
#include <opencv2/opencv.hpp>
#include "io.h"
#include "filtros.h"

namespace fs = std::filesystem;

int main(int argc, char** argv) {
    std::string inputPath = "input.jpg";

    if (!fs::exists(inputPath)) {
        if (fs::exists("input.png")) inputPath = "input.png";
        else {
            std::cerr << "Error: No se encontro 'input.jpg' ni 'input.png'." << std::endl;
            return -1;
        }
    }

    cv::Mat img = IO::cargarImagen(inputPath);
    if (img.empty()) return -1;

    int dimension = 3; // Valor por defecto

    if (argc == 1) {
        std::cout << "Usando matriz de 3x3 por defecto." << std::endl;
    } else if (argc == 2) {
        try {
            dimension = std::stoi(argv[1]);

            // Validamos que sea un número impar mayor a 1
            if (dimension < 3 || dimension % 2 == 0) {
                std::cerr << "Error: La dimension debe ser un numero impar mayor o igual a 3 (ej. 3, 5, 7)." << std::endl;
                return -1;
            }
            std::cout << "Usando matriz de " << dimension << "x" << dimension << "..." << std::endl;
        } catch (...) {
            std::cerr << "Error: Debes ingresar un numero entero valido para la dimension." << std::endl;
            return -1;
        }
    } else {
        std::cerr << "Error: Solo debes pasar un parametro (el tamano de la matriz)." << std::endl;
        std::cerr << "Ejemplo: ./Programa 5" << std::endl;
        return -1;
    }

    std::cout << "Aplicando filtros (Mínimo, Máximo y Mediana)..." << std::endl;

    // Filtro Mínimo
    cv::Mat resultadoMin = Filtros::aplicarMinimo(img, dimension);
    if (!resultadoMin.empty()) cv::imwrite("o_min.jpg", resultadoMin);

    // Filtro Máximo
    cv::Mat resultadoMax = Filtros::aplicarMaximo(img, dimension);
    if (!resultadoMax.empty()) cv::imwrite("o_max.jpg", resultadoMax);

    // Filtro Mediana
    cv::Mat resultadoMed = Filtros::aplicarMediana(img, dimension);
    if (!resultadoMed.empty()) cv::imwrite("o_med.jpg", resultadoMed);

    std::cout << "Se generaron correctamente: 'o_min.jpg', 'o_max.jpg' y 'o_med.jpg'." << std::endl;

    return 0;
}