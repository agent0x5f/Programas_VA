#include <iostream>
#include <filesystem>
#include <string>
#include <opencv2/opencv.hpp>
#include "io.h"
#include "ruido.h"

namespace fs = std::filesystem;

int main(int argc, char** argv) {
    std::string inputPath = "input.jpg";
    std::string outputPath = "output.jpg";

    // Verificación del archivo de entrada
    if (!fs::exists(inputPath)) {
        if (fs::exists("input.png")) {
            inputPath = "input.png";
        } else {
            std::cerr << "Error: No se encontro 'input.jpg' ni 'input.png' en el directorio." << std::endl;
            return -1;
        }
    }

    cv::Mat img = IO::cargarImagen(inputPath);
    if (img.empty()) {
        std::cerr << "Error: El archivo '" << inputPath << "' esta corrupto o no se puede leer." << std::endl;
        return -1;
    }

    //Configuración de las probabilidades
    float probSal = 0.10f;     // 10% por defecto
    float probPimienta = 0.10f; // 10% por defecto

    if (argc == 1) {
        std::cout << "Usando valores por defecto: 10% Sal y 10% Pimienta." << std::endl;
    } else if (argc == 3) {
        try {
            probSal = std::stof(argv[1]);
            probPimienta = std::stof(argv[2]);
            std::cout << "Usando valores personalizados: "
                      << (probSal * 100) << "% Sal y "
                      << (probPimienta * 100) << "% Pimienta." << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Error: Los parametros deben ser numeros decimales validos (ej. 0.15 0.05)." << std::endl;
            return -1;
        }
    } else {
        std::cerr << "Error: Debes proporcionar exactamente 2 valores o ninguno." << std::endl;
        std::cerr << "Ejemplo: ./RuidoSalPimienta 0.10 0.10" << std::endl;
        return -1;
    }

    std::cout << "Aplicando ruido..." << std::endl;

    // Ejecutar el algoritmo
    cv::Mat resultado = Ruido::salYPimienta(img, probSal, probPimienta);

    if (!resultado.empty()) {
        cv::imwrite(outputPath, resultado);
        std::cout << "Imagen con ruido guardada correctamente como '" << outputPath << "'." << std::endl;
    } else {
        std::cerr << "Error interno al aplicar el ruido." << std::endl;
        return -1;
    }

    return 0;
}