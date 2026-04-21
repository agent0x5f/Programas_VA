#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <algorithm>
#include <opencv2/opencv.hpp>
#include "io.h"
#include "promedio.h"

namespace fs = std::filesystem;

int main() {
    std::string inputFolder = "input";
    std::string outputPath = "output.jpg";

    if (!fs::exists(inputFolder) || !fs::is_directory(inputFolder)) {
        std::cerr << "Error: No se encontro la carpeta '" << inputFolder << "' en el directorio." << std::endl;
        return -1;
    }

    std::vector<cv::Mat> imagenes;
    int anchoReferencia = -1;
    int altoReferencia = -1;

    for (const auto& entry : fs::directory_iterator(inputFolder)) {
        if (entry.is_regular_file()) {
            std::string ext = entry.path().extension().string();
            std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

            // Acepta una mezcla de formatos JPG y PNG
            if (ext == ".jpg" || ext == ".jpeg" || ext == ".png") {
                cv::Mat img = IO::cargarImagen(entry.path().string());

                if (!img.empty()) {
                    // Si es la primera imagen, guardamos sus dimensiones como referencia
                    if (imagenes.empty()) {
                        anchoReferencia = img.cols;
                        altoReferencia = img.rows;
                        imagenes.push_back(img);
                    } else {
                        // Si es otra imagen, verificamos si necesita ser redimensionada
                        if (img.cols != anchoReferencia || img.rows != altoReferencia) {
                            cv::Mat imgRedimensionada;
                            cv::resize(img, imgRedimensionada, cv::Size(anchoReferencia, altoReferencia));
                            imagenes.push_back(imgRedimensionada);
                            std::cout << "Nota: Imagen '" << entry.path().filename().string()
                                      << "' redimensionada para coincidir con el resto." << std::endl;
                        } else {
                            imagenes.push_back(img);
                        }
                    }
                } else {
                    std::cerr << "Advertencia: El archivo '" << entry.path().filename().string()
                              << "' esta corrupto y se omitira." << std::endl;
                }
            }
        }
    }

    if (imagenes.empty()) {
        std::cerr << "Error: No se encontraron imagenes validas en '" << inputFolder << "'." << std::endl;
        return -1;
    }

    std::cout << "Se cargaron y estandarizaron " << imagenes.size() << " imagenes listas para promediar." << std::endl;
    std::cout << "Aplicando promediado de imagenes..." << std::endl;

    cv::Mat resultado = Promedio::promediar(imagenes);

    if (!resultado.empty()) {
        cv::imwrite(outputPath, resultado);
        std::cout << "Imagen promediada guardada correctamente como '" << outputPath << "'." << std::endl;
    } else {
        std::cerr << "Error interno durante el calculo del promedio." << std::endl;
        return -1;
    }

    return 0;
}