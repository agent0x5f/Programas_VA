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

    //Verificacion del archivo de entrada
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

    //Configuracion de la media y desviacion
    double media = 0.0;
    double desviacion = 25.0;

    if (argc == 1) {
        std::cout << "Usando valores por defecto: Media = 0.0, Desviacion = 25.0" << std::endl;
    } else if (argc == 3) {
        try {
            media = std::stod(argv[1]);
            desviacion = std::stod(argv[2]);
            std::cout << "Usando valores personalizados: Media = "
                      << media << ", Desviacion = "
                      << desviacion << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Error: Los parametros deben ser numeros validos (ej. 10.0 50.0)." << std::endl;
            return -1;
        }
    } else {
        std::cerr << "Error: Debes proporcionar exactamente 2 valores o ninguno." << std::endl;
        std::cerr << "Ejemplo: ./RuidoGaussiano 0.0 25.0" << std::endl;
        return -1;
    }

    std::cout << "Aplicando ruido Gaussiano..." << std::endl;

    //Ejecutar el algoritmo
    cv::Mat resultado = Ruido::gaussiano(img, media, desviacion);

    if (!resultado.empty()) {
        cv::imwrite(outputPath, resultado);
        std::cout << "Imagen con ruido guardada correctamente como '" << outputPath << "'." << std::endl;
    } else {
        std::cerr << "Error interno al aplicar el ruido." << std::endl;
        return -1;
    }

    return 0;
}