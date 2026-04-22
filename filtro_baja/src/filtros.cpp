#include "filtros.h"
#include <opencv2/opencv.hpp>
#include <cmath>
#include <vector>
#include <algorithm>

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

cv::Mat Filtros::aplicarMinimo(const cv::Mat& src, int dimension) {
    if (src.empty() || dimension % 2 == 0) return src;
    cv::Mat dst = src.clone();
    int offset = dimension / 2;
    int canales = src.channels();
    int ancho = src.cols;
    uchar* datosSrc = src.data;
    uchar* datosDst = dst.data;

    for (int y = offset; y < src.rows - offset; ++y) {
        for (int x = offset; x < ancho - offset; ++x) {
            if (canales == 3) {
                uchar minB = 255, minG = 255, minR = 255;
                for (int ky = -offset; ky <= offset; ++ky) {
                    for (int kx = -offset; kx <= offset; ++kx) {
                        int indiceVecino = ((y + ky) * ancho + (x + kx)) * 3;
                        PixelRGB* pixel = (PixelRGB*)(datosSrc + indiceVecino);
                        if (pixel->b < minB) minB = pixel->b;
                        if (pixel->g < minG) minG = pixel->g;
                        if (pixel->r < minR) minR = pixel->r;
                    }
                }
                int indiceDestino = (y * ancho + x) * 3;
                PixelRGB* pixelDst = (PixelRGB*)(datosDst + indiceDestino);
                pixelDst->b = minB;
                pixelDst->g = minG;
                pixelDst->r = minR;
            } else if (canales == 1) {
                uchar minGris = 255;
                for (int ky = -offset; ky <= offset; ++ky) {
                    for (int kx = -offset; kx <= offset; ++kx) {
                        int indiceVecino = ((y + ky) * ancho + (x + kx));
                        if (datosSrc[indiceVecino] < minGris) minGris = datosSrc[indiceVecino];
                    }
                }
                datosDst[y * ancho + x] = minGris;
            }
        }
    }
    return dst;
}

cv::Mat Filtros::aplicarMaximo(const cv::Mat& src, int dimension) {
    if (src.empty() || dimension % 2 == 0) return src;
    cv::Mat dst = src.clone();
    int offset = dimension / 2;
    int canales = src.channels();
    int ancho = src.cols;
    uchar* datosSrc = src.data;
    uchar* datosDst = dst.data;

    for (int y = offset; y < src.rows - offset; ++y) {
        for (int x = offset; x < ancho - offset; ++x) {
            if (canales == 3) {
                uchar maxB = 0, maxG = 0, maxR = 0;
                for (int ky = -offset; ky <= offset; ++ky) {
                    for (int kx = -offset; kx <= offset; ++kx) {
                        int indiceVecino = ((y + ky) * ancho + (x + kx)) * 3;
                        PixelRGB* pixel = (PixelRGB*)(datosSrc + indiceVecino);
                        if (pixel->b > maxB) maxB = pixel->b;
                        if (pixel->g > maxG) maxG = pixel->g;
                        if (pixel->r > maxR) maxR = pixel->r;
                    }
                }
                int indiceDestino = (y * ancho + x) * 3;
                PixelRGB* pixelDst = (PixelRGB*)(datosDst + indiceDestino);
                pixelDst->b = maxB;
                pixelDst->g = maxG;
                pixelDst->r = maxR;
            } else if (canales == 1) {
                uchar maxGris = 0;
                for (int ky = -offset; ky <= offset; ++ky) {
                    for (int kx = -offset; kx <= offset; ++kx) {
                        int indiceVecino = ((y + ky) * ancho + (x + kx));
                        if (datosSrc[indiceVecino] > maxGris) maxGris = datosSrc[indiceVecino];
                    }
                }
                datosDst[y * ancho + x] = maxGris;
            }
        }
    }
    return dst;
}

cv::Mat Filtros::aplicarMediana(const cv::Mat& src, int dimension) {
    if (src.empty() || dimension % 2 == 0) return src;
    cv::Mat dst = src.clone();
    int offset = dimension / 2;
    int canales = src.channels();
    int ancho = src.cols;
    uchar* datosSrc = src.data;
    uchar* datosDst = dst.data;
    int totalElementos = dimension * dimension;

    for (int y = offset; y < src.rows - offset; ++y) {
        for (int x = offset; x < ancho - offset; ++x) {
            if (canales == 3) {
                std::vector<uchar> vecB, vecG, vecR;
                vecB.reserve(totalElementos);
                vecG.reserve(totalElementos);
                vecR.reserve(totalElementos);

                for (int ky = -offset; ky <= offset; ++ky) {
                    for (int kx = -offset; kx <= offset; ++kx) {
                        int indiceVecino = ((y + ky) * ancho + (x + kx)) * 3;
                        PixelRGB* pixel = (PixelRGB*)(datosSrc + indiceVecino);
                        vecB.push_back(pixel->b);
                        vecG.push_back(pixel->g);
                        vecR.push_back(pixel->r);
                    }
                }
                std::sort(vecB.begin(), vecB.end());
                std::sort(vecG.begin(), vecG.end());
                std::sort(vecR.begin(), vecR.end());

                int indiceDestino = (y * ancho + x) * 3;
                PixelRGB* pixelDst = (PixelRGB*)(datosDst + indiceDestino);
                pixelDst->b = vecB[totalElementos / 2];
                pixelDst->g = vecG[totalElementos / 2];
                pixelDst->r = vecR[totalElementos / 2];
            } else if (canales == 1) {
                std::vector<uchar> vecGris;
                vecGris.reserve(totalElementos);
                for (int ky = -offset; ky <= offset; ++ky) {
                    for (int kx = -offset; kx <= offset; ++kx) {
                        int indiceVecino = ((y + ky) * ancho + (x + kx));
                        vecGris.push_back(datosSrc[indiceVecino]);
                    }
                }
                std::sort(vecGris.begin(), vecGris.end());
                datosDst[y * ancho + x] = vecGris[totalElementos / 2];
            }
        }
    }
    return dst;
}