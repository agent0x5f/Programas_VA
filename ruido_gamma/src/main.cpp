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

    // Verificacion del archivo de entrada
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

    // Configuracion de la forma y escala
    double forma = 2.0;
    double escala = 15.0;

    if (argc == 1) {
        std::cout << "Usando valores por defecto: Forma = 2.0, Escala = 15.0" << std::endl;
    } else if (argc == 3) {
        try {
            forma = std::stod(argv[1]);
            escala = std::stod(argv[2]);
            std::cout << "Usando valores personalizados: Forma = "
                      << forma << ", Escala = "
                      << escala << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Error: Los parametros deben ser numeros validos (ej. 3.0 20.0)." << std::endl;
            return -1;
        }
    } else {
        std::cerr << "Error: Debes proporcionar exactamente 2 valores o ninguno." << std::endl;
        std::cerr << "Ejemplo: ./RuidoGamma 2.0 15.0" << std::endl;
        return -1;
    }

    std::cout << "Aplicando ruido Gamma..." << std::endl;

    // Ejecutar el algoritmo
    cv::Mat resultado = Ruido::gamma(img, forma, escala);

    if (!resultado.empty()) {
        cv::imwrite(outputPath, resultado);
        std::cout << "Imagen con ruido guardada correctamente como '" << outputPath << "'." << std::endl;
    } else {
        std::cerr << "Error interno al aplicar el ruido." << std::endl;
        return -1;
    }

    return 0;
}