//
// Created by azari on 24.09.2021.
//

#include "filters.h"
#include "cmath"
#include "QDebug"

int fit(int min, int max, int val) {
    if (val < min) {
        val = min;
    } else if (val > max) {
        val = max;
    }
    return val;
}


QImage Filters::sobelFilter(const QImage &inputImg) {
    int divX[3][3] = {{-1, 0, 1},
                      {-2, 0, 2},
                      {-1, 0, 1}};
    int divY[3][3] = {{-1, -2, -1},
                      {0,  0,  0,},
                      {1,  2,  1}};
    QImage outImg = inputImg.copy();
    int xLen = inputImg.width() - 2;
    int yLen = inputImg.height() - 2;
    int xOut = 1;
    for (int i = 0; i < xLen; ++i) {
        int yOut = 1;
        for (int j = 0; j < yLen; ++j) {
            double sumX = 0;
            double sumY = 0;
            for (int kernelI = 0; kernelI < 3; ++kernelI) {
                for (int kernelJ = 0; kernelJ < 3; ++kernelJ) {
                    QColor pixel = inputImg.pixelColor(i + kernelI, j + kernelJ);
                    double grayScale = pixel.red() * 0.299 + pixel.green() * 0.587 + pixel.blue() * 0.114;
                    sumX += (grayScale * divX[kernelI][kernelJ]);
                    sumY += (grayScale * divY[kernelI][kernelJ]);
                }
            }
            int imgValue = int(sqrt(pow(sumX, 2) + pow(sumY, 2)));
            imgValue = fit(0, 255, imgValue);
            outImg.setPixel(xOut, yOut, QColor(imgValue, imgValue, imgValue).rgb());
            yOut++;
        }
        xOut++;
    }
    return outImg;
}

double gauss(double sigma, int x, int y) {
    double gaussParam = 1 / (2 * M_PI * pow(sigma, 2));
    double expArgue = -(pow(x, 2) + pow(y, 2)) / (2 * pow(sigma, 2));
    return gaussParam * exp(expArgue);
}

std::tuple<QImage, std::vector<double>> Filters::sobelWithGradients(const QImage &inputImg) {
    int divX[3][3] = {{-1, 0, 1},
                      {-2, 0, 2},
                      {-1, 0, 1}};
    int divY[3][3] = {{-1, -2, -1},
                      {0,  0,  0,},
                      {1,  2,  1}};
    QImage outImg = inputImg.copy();
    int xLen = inputImg.width() - 2;
    int yLen = inputImg.height() - 2;
    int xOut = 1;
    std::vector<double> gradAngleVector(0);
    for (int i = 0; i < xLen; ++i) {
        int yOut = 1;
        for (int j = 0; j < yLen; ++j) {
            double sumX = 0;
            double sumY = 0;
            for (int kernelI = 0; kernelI < 3; ++kernelI) {
                for (int kernelJ = 0; kernelJ < 3; ++kernelJ) {
                    QColor pixel = inputImg.pixelColor(i + kernelI, j + kernelJ);
                    double grayScale = pixel.red() * 0.299 + pixel.green() * 0.587 + pixel.blue() * 0.114;
                    sumX += (grayScale * divX[kernelI][kernelJ]);
                    sumY += (grayScale * divY[kernelI][kernelJ]);

                }
            }
            double gradAngle = std::atan2(sumY, sumX) * (M_PI / 180) + 180;///get angle in [0,360]
            gradAngleVector.emplace_back(gradAngle);
            int imgValue = int(sqrt(pow(sumX, 2) + pow(sumY, 2)));
            imgValue = fit(0, 255, imgValue);
            outImg.setPixel(xOut, yOut, QColor(imgValue, imgValue, imgValue).rgb());
            yOut++;
        }
        xOut++;
    }
    return {outImg, gradAngleVector};


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
    int xLen = inputImg.width() - kernelSize + 1;
    int yLen = inputImg.height() - kernelSize + 1;
    int yOut = 1;
    for (int j = 0; j < yLen; ++j) {
        int xOut = 1;
        for (int i = 0; i < xLen; ++i) {
            double r_sum = 0;
            double g_sum = 0;
            double b_sum = 0;
            for (int kernelJ = 0; kernelJ < kernelSize; ++kernelJ) {
                for (int kernelI = 0; kernelI < kernelSize; ++kernelI) {
                    QColor pixel = inputImg.pixelColor(i + kernelI, j + kernelJ);
                    r_sum += pixel.red() * gaussKernel[kernelI][kernelJ];
                    g_sum += pixel.green() * gaussKernel[kernelI][kernelJ];
                    b_sum += pixel.blue() * gaussKernel[kernelI][kernelJ];
                }
            }
            int rValue = fit(0, 255, int(r_sum));
            int gValue = fit(0, 255, int(g_sum));
            int bValue = fit(0, 255, int(b_sum));
            outImg.setPixel(xOut, yOut,
                            QColor(rValue, gValue, bValue).rgb());
            xOut++;
        }
        yOut++;
    }


    return outImg;

}


double gaborFunc(double xSh, double ySh, double lambda, double gama, double phi, double sigma) {
    return exp(-(pow(xSh, 2) + gama * pow(ySh, 2)) / (2 * pow(sigma, 2))) * cos(2 * M_PI * xSh / lambda + phi);
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
    int xLen = inputImg.width() - kernelSize + 1;
    int yLen = inputImg.height() - kernelSize + 1;
    int yOut = 1;
    for (int j = 0; j < yLen; ++j) {
        int xOut = 1;
        for (int i = 0; i < xLen; ++i) {
            ////double convSum = 0;
            double rSum = 0;
            double gSum = 0;
            double bSum = 0;
            for (int kernel_j = 0; kernel_j < kernelSize; ++kernel_j) {
                for (int kernel_i = 0; kernel_i < kernelSize; ++kernel_i) {
                    QColor pixel = inputImg.pixelColor(i + kernel_i, j + kernel_j);
                    ////double grayScale = pixel.red() * 0.299 + pixel.green() * 0.587 + pixel.blue() * 0.114;
                    ////convSum += grayScale * gaborKernel[kernel_i][kernel_j];
                    rSum += pixel.red() * gaborKernel[kernel_i][kernel_j];
                    gSum += pixel.green() * gaborKernel[kernel_i][kernel_j];
                    bSum += pixel.blue() * gaborKernel[kernel_i][kernel_j];
                }
            }
            ////auto value = fit(0, 255, int(convSum));
            auto rValue = fit(0, 255, int(rSum));
            auto gValue = fit(0, 255, int(gSum));
            auto bValue = fit(0, 255, int(bSum));

            outImg.setPixel(xOut, yOut,
                            QColor(rValue, gValue, bValue).rgb());
            xOut++;
        }
        yOut++;
    }


    return outImg;
}



