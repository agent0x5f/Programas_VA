#include <iostream>
#include <vector>
#include <string>
#include <opencv2/opencv.hpp>
#include "io.h"
#include "transformacion.h"

int main(int argc, char** argv) {
    std::string inputPath = "input.jpg";
    std::string outputPath = "output.jpg";

    // Intentamos cargar la imagen
    cv::Mat img = IO::cargarImagen(inputPath);
    if (img.empty()) {
        inputPath = "input.png";
        img = IO::cargarImagen(inputPath);
    }

    if (img.empty()) {
        std::cerr << "Error: No se pudo encontrar 'input.jpg' ni 'input.png' en el directorio." << std::endl;
        return -1;
    }

    std::cout << "Imagen '" << inputPath << "' cargada exitosamente." << std::endl;

    std::vector<cv::Point> puntos;

    // Procesamiento de los argumentos de la terminal
    if (argc == 1) {
        // Sin argumentos: Usamos los valores por defecto requeridos
        puntos.push_back(cv::Point(80, 30));
        puntos.push_back(cv::Point(180, 230));
        std::cout << "Usando 2 puntos por defecto: (80, 30) y (180, 230)" << std::endl;
    } else {
        // Con argumentos: Validar que se pasaron en pares correctos (X e Y)
        // argc incluye el nombre del programa, por lo que (argc - 1) son los argumentos reales
        if ((argc - 1) % 2 != 0) {
            std::cerr << "Error: Debes ingresar pares de coordenadas X Y completos." << std::endl;
            std::cerr << "Ejemplo de uso: ./TransformacionLineal 80 30 180 230" << std::endl;
            return -1;
        }

        for (int i = 1; i < argc; i += 2) {
            try {
                int x = std::stoi(argv[i]);
                int y = std::stoi(argv[i+1]);
                puntos.push_back(cv::Point(x, y));
            } catch (const std::exception& e) {
                std::cerr << "Error: Las coordenadas deben ser numeros enteros validos." << std::endl;
                return -1;
            }
        }
        std::cout << "Usando " << puntos.size() << " puntos proporcionados desde la terminal." << std::endl;
    }

    std::cout << "Aplicando transformacion lineal a pedazos..." << std::endl;
    cv::Mat resultado = Transformacion::linealAPedazos(img, puntos);

    if (!resultado.empty()) {
        cv::imwrite(outputPath, resultado);
        std::cout << "Imagen modificada guardada correctamente como '" << outputPath << "'." << std::endl;
    } else {
        std::cerr << "Error interno al aplicar la transformacion." << std::endl;
        return -1;
    }

    return 0;
}