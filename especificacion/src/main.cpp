#include <iostream>
#include <filesystem>
#include <opencv2/opencv.hpp>
#include "io.h"
#include "especificacion.h"

int main() {
    std::string input1Path = "input1.jpg";
    std::string input2Path = "input2.jpg";
    std::string outputPath = "output.jpg";

    // Verificamos que exista input1 (origen)
    if (!std::filesystem::exists(input1Path)) {
        if (std::filesystem::exists("input1.png")) {
            input1Path = "input1.png";
        } else {
            std::cerr << "Error: No se encontro 'input1.jpg' ni 'input1.png' (imagen a modificar)." << std::endl;
            return -1;
        }
    }

    // Verificamos que exista input2 (referencia)
    if (!std::filesystem::exists(input2Path)) {
        if (std::filesystem::exists("input2.png")) {
            input2Path = "input2.png";
        } else {
            std::cerr << "Error: No se encontro 'input2.jpg' ni 'input2.png' (imagen de referencia)." << std::endl;
            return -1;
        }
    }

    // Cargamos ambas imagenes
    cv::Mat src = IO::cargarImagen(input1Path);
    cv::Mat ref = IO::cargarImagen(input2Path);

    if (src.empty() || ref.empty()) {
        std::cerr << "Error: Alguna de las imagenes existe en el disco pero esta corrupta o no se pudo leer." << std::endl;
        return -1;
    }

    std::cout << "Imagen original: '" << input1Path << "' cargada." << std::endl;
    std::cout << "Imagen de referencia: '" << input2Path << "' cargada." << std::endl;
    std::cout << "Aplicando especificacion de histograma..." << std::endl;

    // Ejecutamos el algoritmo
    cv::Mat resultado = Especificacion::especificar(src, ref);

    if (!resultado.empty()) {
        cv::imwrite(outputPath, resultado);
        std::cout << "Imagen modificada guardada correctamente como '" << outputPath << "'." << std::endl;
    } else {
        std::cerr << "Error interno al aplicar la especificacion." << std::endl;
        return -1;
    }

    return 0;
}