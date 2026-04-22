#include <iostream>
#include <filesystem>
#include <string>
#include <opencv2/opencv.hpp>
#include "io.h"
#include "ruido.h"

namespace fs = std::filesystem;

int main(int argc, char** argv) {
    std::string inputPath = "input.jpg";

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

    // Configuracion de la media y desviacion
    double media = 0.0;
    double desviacion = 75.0; 

    if (argc == 1) {
        std::cout << "Usando valores por defecto: Media = " << media << ", Desviacion = " << desviacion << std::endl;
    } else if (argc == 3) {
        try {
            media = std::stod(argv[1]);
            desviacion = std::stod(argv[2]);
            std::cout << "Usando valores personalizados: Media = "
                      << media << ", Desviacion = "
                      << desviacion << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Error: Los parametros deben ser numeros validos (ej. 10.0 75.0)." << std::endl;
            return -1;
        }
    } else {
        std::cerr << "Error: Debes proporcionar exactamente 2 valores o ninguno." << std::endl;
        std::cerr << "Ejemplo: ./RuidoGaussiano 0.0 75.0" << std::endl;
        return -1;
    }

    std::cout << "Aplicando ruido Gaussiano y generando 50 imagenes..." << std::endl;

    // Inicializar el generador de numeros aleatorios (RNG) de OpenCV usando el reloj del sistema.
    // Esto asegura que cada llamada a cv::randn y cada ejecucion del programa tenga ruido diferente.
    cv::theRNG().state = cv::getTickCount();

    // Bucle para generar 50 imagenes diferentes
    for (int i = 1; i <= 50; ++i) {
        // Ejecutar el algoritmo
        cv::Mat resultado = Ruido::gaussiano(img, media, desviacion);

        if (!resultado.empty()) {
            // Crear un nombre de archivo unico para cada iteracion
            std::string outputPath = "output_" + std::to_string(i) + ".jpg";
            cv::imwrite(outputPath, resultado);
            std::cout << "Imagen con ruido guardada correctamente como '" << outputPath << "'." << std::endl;
        } else {
            std::cerr << "Error interno al aplicar el ruido en la imagen " << i << "." << std::endl;
        }
    }

    std::cout << "\nProceso finalizado: 50 imagenes generadas con exito." << std::endl;

    return 0;
}