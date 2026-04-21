#include "filtros.h"
#include <opencv2/opencv.hpp>
#include <cmath>

struct PixelRGB {
    uchar b;
    uchar g;
    uchar r;
};

cv::Mat Filtros::aplicarConvolucion(const cv::Mat& src, const cv::Mat& kernel, bool absoluto) {
    if (src.empty() || kernel.empty()) return src;
    if (kernel.rows % 2 == 0 || kernel.cols % 2 == 0) return src; // El kernel debe ser impar

    cv::Mat kernelF;
    kernel.convertTo(kernelF, CV_32F);
    cv::Mat dst = src.clone();

    int offset_y = kernelF.rows / 2;
    int offset_x = kernelF.cols / 2;
    int canales = src.channels();
    int ancho = src.cols;
    uchar* datosSrc = src.data;
    uchar* datosDst = dst.data;

    for (int y = offset_y; y < src.rows - offset_y; ++y) {
        for (int x = offset_x; x < ancho - offset_x; ++x) {

            if (canales == 3) {
                float sumaB = 0.0f, sumaG = 0.0f, sumaR = 0.0f;

                for (int ky = -offset_y; ky <= offset_y; ++ky) {
                    for (int kx = -offset_x; kx <= offset_x; ++kx) {
                        int indiceVecino = ((y + ky) * ancho + (x + kx)) * 3;
                        PixelRGB* pixel = (PixelRGB*)(datosSrc + indiceVecino);
                        float peso = kernelF.at<float>(ky + offset_y, kx + offset_x);

                        sumaB += pixel->b * peso;
                        sumaG += pixel->g * peso;
                        sumaR += pixel->r * peso;
                    }
                }
                if (absoluto) {
                    sumaB = std::abs(sumaB);
                    sumaG = std::abs(sumaG);
                    sumaR = std::abs(sumaR);
                }
                int indiceDestino = (y * ancho + x) * 3;
                PixelRGB* pixelDst = (PixelRGB*)(datosDst + indiceDestino);

                pixelDst->b = cv::saturate_cast<uchar>(std::round(sumaB));
                pixelDst->g = cv::saturate_cast<uchar>(std::round(sumaG));
                pixelDst->r = cv::saturate_cast<uchar>(std::round(sumaR));

            } else if (canales == 1) {
                float sumaGris = 0.0f;

                for (int ky = -offset_y; ky <= offset_y; ++ky) {
                    for (int kx = -offset_x; kx <= offset_x; ++kx) {
                        int indiceVecino = ((y + ky) * ancho + (x + kx)) * 1;
                        float peso = kernelF.at<float>(ky + offset_y, kx + offset_x);
                        sumaGris += datosSrc[indiceVecino] * peso;
                    }
                }
                if (absoluto) sumaGris = std::abs(sumaGris);
                
                int indiceDestino = (y * ancho + x) * 1;
                datosDst[indiceDestino] = cv::saturate_cast<uchar>(std::round(sumaGris));
            }
        }
    }
    return dst;
}