#include <iostream>
#include <filesystem>
#include <opencv2/opencv.hpp>
#include "io.h"
#include "ecualizacion.h"

int main() {
    std::string inputPath = "input.jpg";
    std::string outputPath = "output.jpg";

    if (!std::filesystem::exists(inputPath)) {
        if (std::filesystem::exists("input.png")) {
            inputPath = "input.png";
        } else {
            std::cerr << "Error: No se encontro 'input.jpg' ni 'input.png' en el directorio." << std::endl;
            return -1;
        }
    }

    cv::Mat img = IO::cargarImagen(inputPath);

    if (img.empty()) {
        std::cerr << "Error: El archivo '" << inputPath << "' existe pero esta corrupto o no se puede leer." << std::endl;
        return -1;
    }

    std::cout << "Imagen '" << inputPath << "' cargada exitosamente." << std::endl;
    std::cout << "Aplicando ecualizacion de histograma..." << std::endl;

    cv::Mat resultado = Ecualizacion::ecualizar(img);

    if (!resultado.empty()) {
        cv::imwrite(outputPath, resultado);
        std::cout << "Imagen ecualizada guardada correctamente como '" << outputPath << "'." << std::endl;
    } else {
        std::cerr << "Error interno al aplicar la ecualizacion." << std::endl;
        return -1;
    }

    return 0;
}