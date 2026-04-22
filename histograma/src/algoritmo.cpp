#include "algoritmo.h"
#include <algorithm>

// calculamos las frecuencias
DatosHistograma Algoritmo::calcularFrecuencias(const cv::Mat& img, bool enGrises)  {
    DatosHistograma datos;
    datos.is_gray = enGrises;
    int histSize = 256;
    float range[] = { 0, 256 };
    const float* histRange[] = { range };

    if (enGrises) {
        cv::Mat img_gray;
        if (img.channels() == 3) cv::cvtColor(img, img_gray, cv::COLOR_BGR2GRAY);
        else img_gray = img;

        cv::calcHist(&img_gray, 1, 0, cv::Mat(), datos.gray_hist, 1, &histSize, histRange, true, false);
        cv::minMaxLoc(datos.gray_hist, nullptr, &datos.max_freq);
    } else {
        cv::Mat img_color = img;
        if (img.channels() == 1) cv::cvtColor(img, img_color, cv::COLOR_GRAY2BGR);

        std::vector<cv::Mat> bgr_planes;
        cv::split(img_color, bgr_planes);

        cv::calcHist(&bgr_planes[0], 1, 0, cv::Mat(), datos.b_hist, 1, &histSize, histRange, true, false);
        cv::calcHist(&bgr_planes[1], 1, 0, cv::Mat(), datos.g_hist, 1, &histSize, histRange, true, false);
        cv::calcHist(&bgr_planes[2], 1, 0, cv::Mat(), datos.r_hist, 1, &histSize, histRange, true, false);

        double b_max, g_max, r_max;
        cv::minMaxLoc(datos.b_hist, nullptr, &b_max);
        cv::minMaxLoc(datos.g_hist, nullptr, &g_max);
        cv::minMaxLoc(datos.r_hist, nullptr, &r_max);
        datos.max_freq = std::max({b_max, g_max, r_max});
    }
    return datos;
}

// Interfaz y graficado con OpenCV
cv::Mat Algoritmo::dibujarLienzoHistograma(const DatosHistograma& datos, int width, int height) {
    int margin = 50;
    cv::Mat histImage(height, width, CV_8UC3, cv::Scalar(255,255,255)); //creamos donde dibujar

    int draw_h = height - (2 * margin);
    int draw_w = width - (2 * margin);
    int histSize = 256;
    int bin_w = cvRound((double)draw_w / histSize);

    // Dibujar Ejes
    cv::line(histImage, cv::Point(margin, margin), cv::Point(margin, height - margin), cv::Scalar(0,0,0), 2);
    cv::line(histImage, cv::Point(margin, height - margin), cv::Point(width - margin, height - margin), cv::Scalar(0,0,0), 2);

    // Eje Y (Frecuencia Absoluta)
    for (int i = 0; i <= 4; i++) {
        int y = height - margin - (i * draw_h / 4);
        cv::line(histImage, cv::Point(margin - 5, y), cv::Point(margin, y), cv::Scalar(0,0,0), 1);

        int valor_real = cvRound((datos.max_freq * i) / 4.0);
        cv::putText(histImage, std::to_string(valor_real), cv::Point(5, y + 5),cv::FONT_HERSHEY_SIMPLEX, 0.35, cv::Scalar(0,0,0), 1);
    }
    cv::putText(histImage, "Frecuencia", cv::Point(margin - 40, margin - 15),cv::FONT_HERSHEY_SIMPLEX, 0.4, cv::Scalar(0,0,0), 1);

    // Eje X
    for (int i = 0; i <= 255; i += 64) {
        int x = margin + (i * bin_w);
        cv::line(histImage, cv::Point(x, height - margin), cv::Point(x, height - margin + 5), cv::Scalar(200, 200, 200), 1);
        cv::putText(histImage, std::to_string(i), cv::Point(x - 10, height - margin + 25),cv::FONT_HERSHEY_SIMPLEX, 0.4, cv::Scalar(0,0,0), 1);
    }
    cv::putText(histImage, "Intensidad de Pixel", cv::Point(width / 2 - 50, height - 10),cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0,0,0), 1);

    if (datos.is_gray) {
        cv::Mat hist_draw, log_hist;
        // Aplicamos logaritmo sumando 1 para evitar log(0)
        cv::log(datos.gray_hist + 1, log_hist); 
        cv::normalize(log_hist, hist_draw, 0, draw_h, cv::NORM_MINMAX);
        
        for (int i = 0; i < histSize; i++) {
            int x = margin + (i * bin_w);
            int y_base = height - margin;
            int y = y_base - cvRound(hist_draw.at<float>(i));
            int ancho_barra = std::max(1, bin_w - 1);
            if (y < y_base) {
                cv::rectangle(histImage, cv::Point(x, y_base), cv::Point(x + ancho_barra, y), cv::Scalar(80, 80, 80), cv::FILLED);
            }
        }
    } else {
        cv::Mat b_draw, g_draw, r_draw;
        cv::Mat b_log, g_log, r_log;
        
        // Aplicamos logaritmo a cada canal sumando 1
        cv::log(datos.b_hist + 1, b_log);
        cv::log(datos.g_hist + 1, g_log);
        cv::log(datos.r_hist + 1, r_log);

        cv::normalize(b_log, b_draw, 0, draw_h, cv::NORM_MINMAX);
        cv::normalize(g_log, g_draw, 0, draw_h, cv::NORM_MINMAX);
        cv::normalize(r_log, r_draw, 0, draw_h, cv::NORM_MINMAX);

        for (int i = 0; i < histSize; i++) {
            int x = margin + (i * bin_w);
            int y_base = height - margin;

            int b_y = y_base - cvRound(b_draw.at<float>(i));
            int g_y = y_base - cvRound(g_draw.at<float>(i));
            int r_y = y_base - cvRound(r_draw.at<float>(i));

            std::vector<std::pair<int, cv::Scalar>> barras = {
                {b_y, cv::Scalar(255, 0, 0)}, {g_y, cv::Scalar(0, 150, 0)}, {r_y, cv::Scalar(0, 0, 255)}
            };

            std::sort(barras.begin(), barras.end(), [](const auto& a, const auto& b) { return a.first < b.first; });

            int ancho_barra = std::max(1, bin_w - 1);
            for (const auto& barra : barras) {
                if (barra.first < y_base) {
                    cv::rectangle(histImage, cv::Point(x, y_base), cv::Point(x + ancho_barra, barra.first), barra.second, cv::FILLED);
                }
            }
        }
    }
    return histImage;
}

cv::Mat Algoritmo::generarHistograma(const cv::Mat& img_bgr, bool enGrises, int width, int height) {
    if (img_bgr.empty()) return img_bgr;
    DatosHistograma datos = calcularFrecuencias(img_bgr, enGrises);
    return dibujarLienzoHistograma(datos, width, height);
}