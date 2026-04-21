#include <iostream>
#include <opencv2/opencv.hpp>
#include "io.h"
#include "algoritmo.h"

int main() {
    std::string inputPath = "input.jpg";
    std::string outputPathColor = "output.jpg";
    std::string outputPathGris = "output2.jpg";

    // Intentamos cargar primero el .jpg
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

    std::cout << "Generando el histograma a color..." << std::endl;
    cv::Mat lienzoColor = Algoritmo::generarHistograma(img, false, 800, 600);

    if (!lienzoColor.empty()) {
        cv::imwrite(outputPathColor, lienzoColor);
        std::cout << "Histograma a color guardado como '" << outputPathColor << "'." << std::endl;
    } else {
        std::cerr << "Error en la generacion del histograma a color." << std::endl;
    }

    std::cout << "Generando el histograma en escala de grises..." << std::endl;
    cv::Mat lienzoGris = Algoritmo::generarHistograma(img, true, 800, 600);

    if (!lienzoGris.empty()) {
        cv::imwrite(outputPathGris, lienzoGris);
        std::cout << "Histograma en escala de grises guardado como '" << outputPathGris << "'." << std::endl;
    } else {
        std::cerr << "Error en la generacion del histograma en escala de grises." << std::endl;
    }

    return 0;
}