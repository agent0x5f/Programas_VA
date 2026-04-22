#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <cmath>
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

    if (argc == 1) {
        std::cout << "Usando kernel Pasa Alta de 3x3 por defecto." << std::endl;
        // Kernel Pasa Alta
        kernel = (cv::Mat_<float>(3, 3) <<
            -1, -1, -1,
            -1,  8, -1,
            -1, -1, -1);
    } else {
        int numElementos = argc - 1;
        int dimOriginal = std::round(std::sqrt(numElementos));

        if (dimOriginal * dimOriginal != numElementos) {
            std::cerr << "Error: La cantidad de valores debe formar un cuadrado perfecto (ej. 4, 9, 16, 25)." << std::endl;
            return -1;
        }

        int dimFinal = dimOriginal;

        // Si la matriz es par, le sumamos 1 a la dimensión
        if (dimOriginal % 2 == 0) {
            dimFinal = dimOriginal + 1;
            std::cout << "Matriz par detectada (" << dimOriginal << "x" << dimOriginal
                      << "). Redimensionando a " << dimFinal << "x" << dimFinal
                      << " y rellenando bordes con 1's..." << std::endl;
        } else {
            std::cout << "Construyendo kernel personalizado de " << dimFinal << "x" << dimFinal << "..." << std::endl;
        }

        // Inicializamos la matriz final llenándola toda de 1.0f
        // (así, los bordes extra ya quedan con 1's por defecto)
        kernel = cv::Mat(dimFinal, dimFinal, CV_32F, cv::Scalar(1.0f));

        // Rellenamos la matriz con los argumentos proporcionados
        for (int i = 0; i < numElementos; ++i) {
            try {
                int fila = i / dimOriginal;
                int col = i % dimOriginal;
                kernel.at<float>(fila, col) = std::stof(argv[i + 1]);
            } catch (...) {
                std::cerr << "Error: Todos los parametros del kernel deben ser numeros validos." << std::endl;
                return -1;
            }
        }
    }

    std::cout << "Aplicando convolucion Pasa Alta..." << std::endl;
  
    cv::Mat hsv;
    cv::cvtColor(img, hsv, cv::COLOR_BGR2HSV);
    std::vector<cv::Mat> canales;
    cv::split(hsv, canales);

    // Aplicar el filtro solo al canal V
    cv::Mat canalV_filtrado = Filtros::aplicarConvolucion(canales[2], kernel, true);
    // Actualizar el canal V con el resultado filtrado
    canalV_filtrado.copyTo(canales[2]);

	//a gris.
	canales[1] = cv::Mat::zeros(canales[1].size(), canales[1].type()); // S = 0

    // Volver a unir los canales
    cv::Mat hsv_resultado;
    cv::merge(canales, hsv_resultado);

    cv::Mat resultado;
    cv::cvtColor(hsv_resultado, resultado, cv::COLOR_HSV2BGR);

    if (!resultado.empty()) {
        cv::imwrite(outputPath, resultado);
        std::cout << "Imagen filtrada guardada correctamente como '" << outputPath << "'." << std::endl;
    } else {
        std::cerr << "Error interno al aplicar el filtro." << std::endl;
        return -1;
    }

    return 0;
}