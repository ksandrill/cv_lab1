//
// Created by azari on 24.09.2021.
//

#include "filters.h"
#include "cmath"
#include "QDebug"

QImage Filters::sobelFilter(const QImage &inputImg, int threshold) {
    int divX[3][3] = {{-1, 0, 1},
                      {-2, 0, 2},
                      {-1, 0, 1}};
    int divY[3][3] = {{-1, -2, -1},
                      {0,  0,  0,},
                      {1,  2,  1}};
    QImage outImg = inputImg.copy();
    int x_len = inputImg.width() - 2;
    int y_len = inputImg.height() - 2;
    int y_out = 1;
    for (int j = 0; j < y_len; ++j) {
        int x_out = 1;
        for (int i = 0; i < x_len; ++i) {
            double sum_x = 0;
            double sum_y = 0;
            for (int kernel_j = 0; kernel_j < 3; ++kernel_j) {
                for (int kernel_i = 0; kernel_i < 3; ++kernel_i) {
                    QColor pixel = inputImg.pixelColor(i + kernel_i, j + kernel_j);
                    double grayScale = pixel.red() * 0.299 + pixel.green() * 0.587 + pixel.blue() * 0.114;
                    sum_x += (grayScale * divX[kernel_i][kernel_j]);
                    sum_y += (grayScale * divY[kernel_i][kernel_j]);
                }
            }
            int img_value = int(sqrt(pow(sum_x, 2) + pow(sum_y, 2)));
            img_value = img_value > threshold ? 255 : 0;
            outImg.setPixel(x_out, y_out, QColor(img_value, img_value, img_value).rgb());
            x_out++;
        }
        y_out++;
    }
    return outImg;
}

double gauss(double sigma, int x, int y) {
    double gaussParam = 1 / (2 * M_PI * pow(sigma, 2));
    double exp_argue = -(pow(x, 2) + pow(y, 2)) / (2 * pow(sigma, 2));
    return gaussParam * exp(exp_argue);
}


QImage Filters::gaussFilter(const QImage &inputImg, double sigma, int kernelSize) {
    QImage outImg = inputImg.copy();
    double gaussKernel[kernelSize][kernelSize];
    int center = kernelSize / 2;
    double sum = 0.0; /// normalization
    for (auto i = 0; i < kernelSize; ++i) {
        int xPos = i - center;
        for (auto j = 0; j < kernelSize; ++j) {
            int yPos = j - center;
            gaussKernel[i][j] = gauss(sigma, xPos, yPos);
            sum += gaussKernel[i][j];
        }
    }
    for (auto i = 0; i < kernelSize; ++i) {
        for (auto j = 0; j < kernelSize; ++j) {
            gaussKernel[i][j] /= sum;
        }
    }
    int x_len = inputImg.width() - kernelSize + 1;
    int y_len = inputImg.height() - kernelSize + 1;
    int y_out = 1;
    for (int j = 0; j < y_len; ++j) {
        int x_out = 1;
        for (int i = 0; i < x_len; ++i) {
            double r_sum = 0;
            double g_sum = 0;
            double b_sum = 0;
            for (int kernel_j = 0; kernel_j < kernelSize; ++kernel_j) {
                for (int kernel_i = 0; kernel_i < kernelSize; ++kernel_i) {
                    QColor pixel = inputImg.pixelColor(i + kernel_i, j + kernel_j);
                    r_sum += pixel.red() * gaussKernel[kernel_i][kernel_j];
                    g_sum += pixel.green() * gaussKernel[kernel_i][kernel_j];
                    b_sum += pixel.blue() * gaussKernel[kernel_i][kernel_j];
                }
            }
            outImg.setPixel(x_out, y_out,
                            QColor(int(r_sum), int(g_sum), int(b_sum)).rgb());
            x_out++;
        }
        y_out++;
    }


    return outImg;

}


double gaborFunc(double xSh, double ySh, double lambda, double gama, double phi, double sigma) {
    return exp(-(pow(xSh, 2) + gama * pow(ySh, 2)) / (2 * pow(sigma, 2))) * cos(2 * M_PI * xSh / lambda + phi);
}

int fit(int min, int max, int val) {
    if (val < min) {
        val = min;
    } else if (val > max) {
        val = max;
    }
    return val;
}

QImage
Filters::gaborFilter(const QImage &inputImg, double lambda, double theta, double phi, double gama, int kernelSize) {
    QImage outImg = inputImg.copy();
    double gaborKernel[kernelSize][kernelSize];
    int center = kernelSize / 2;
    double sigma = 0.56 * lambda;
    double radTheta = theta * M_PI / 180;
    double sum = 0;
    for (auto i = 0; i < kernelSize; ++i) {
        int xPos = i - center;
        for (auto j = 0; j < kernelSize; ++j) {
            int yPos = j - center;
            double xSh = xPos * cos(radTheta) + yPos * sin(radTheta);
            double ySh = -xPos * sin(radTheta) + yPos * cos(radTheta);
            gaborKernel[i][j] = gaborFunc(xSh, ySh, lambda, gama, phi, sigma);
            sum += gaborKernel[i][j];
        }
    }
    for (auto i = 0; i < kernelSize; ++i) {
        for (auto j = 0; j < kernelSize; ++j) {
            gaborKernel[i][j] /= sum;
        }
    }
    int x_len = inputImg.width() - kernelSize + 1;
    int y_len = inputImg.height() - kernelSize + 1;
    int y_out = 1;
    for (int j = 0; j < y_len; ++j) {
        int x_out = 1;
        for (int i = 0; i < x_len; ++i) {
            ////double convSum = 0;
            double r_sum = 0;
            double g_sum = 0;
            double b_sum = 0;
            for (int kernel_j = 0; kernel_j < kernelSize; ++kernel_j) {
                for (int kernel_i = 0; kernel_i < kernelSize; ++kernel_i) {
                    QColor pixel = inputImg.pixelColor(i + kernel_i, j + kernel_j);
                    ////double grayScale = pixel.red() * 0.299 + pixel.green() * 0.587 + pixel.blue() * 0.114;
                    ////convSum += grayScale * gaborKernel[kernel_i][kernel_j];
                    r_sum += pixel.red() * gaborKernel[kernel_i][kernel_j];
                    g_sum += pixel.green() * gaborKernel[kernel_i][kernel_j];
                    b_sum += pixel.blue() * gaborKernel[kernel_i][kernel_j];
                }
            }
            ////auto value = fit(0, 255, int(convSum));
            auto r_value = fit(0, 255, int(r_sum));
            auto g_value = fit(0, 255, int(g_sum));
            auto b_value = fit(0, 255, int(b_sum));

            outImg.setPixel(x_out, y_out,
                            QColor(r_value, g_value, b_value).rgb());
            x_out++;
        }
        y_out++;
    }


    return outImg;
}


