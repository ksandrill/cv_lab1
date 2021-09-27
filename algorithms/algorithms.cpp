//
// Created by azari on 25.09.2021.
//

#include <algorithm>
#include <iostream>
#include "algorithms.h"
#include "cmath"
#include "qdebug.h"
#include "filters.h"

#define PI_GRAD 180.0

int Algorithms::getOtsuThreshold(const std::vector<int> &grayScaleVector) {
    const auto &[min_it, max_it] = std::minmax_element(std::cbegin(grayScaleVector), std::cend(grayScaleVector));
    auto[min, max] = std::tie(*min_it, *max_it);
    auto histVector = std::vector<int>(max - min, 0);
    if (histVector.empty()) {
        return 0;
    }
    /////qDebug() << grayScaleVector[0] << " " << min << " " << max;
    for (auto value: grayScaleVector) {
        histVector[value - min] += 1;
    }
    int m = 0; /// a1 * hist_pos1 + ... hist_pos - value from sample,light intensity in 0..255, a - height of bean
    int n = 0;/// a1+a2....
    auto histSize = histVector.size();
    for (auto i = 0; i < histSize; ++i) {
        m += histVector[i] * i;
        n += histVector[i];
    }
    double maxSigma = -1;
    int threshold = 0;
    int m1 = 0;
    int n1 = 0;
    for (auto i = 0; i < histSize; ++i) {
        m1 += histVector[i] * i;
        n1 += histVector[i];
        double prob1 = double(n1) / n; /// prob2 = 1  - prob1
        double a = double(m1) / n1 - double(m - m1) / (n - n1); /// mean for class 1 -  mean for class 2
        double sigma = prob1 * (1 - prob1) * std::pow(a, 2);
        if (sigma > maxSigma) {
            maxSigma = sigma;
            threshold = i;
        }

    }
    threshold += min;
    return threshold;

}

QImage Algorithms::OtsuBinarization(const QImage &inputImage) {
    auto width = inputImage.width();
    auto height = inputImage.height();
    std::vector<int> grayScaleVector;
    QImage outputImage = inputImage.copy();
    for (auto i = 0; i < width; ++i) {
        for (auto j = 0; j < height; ++j) {
            auto redVal = inputImage.pixelColor(i, j).red();
            auto greenVal = inputImage.pixelColor(i, j).green();
            auto blueVal = inputImage.pixelColor(i, j).blue();
            auto grayscaleVal = int(redVal * 0.299 + greenVal * 0.587 + blueVal * 0.114);
            outputImage.setPixel(i, j, QColor(grayscaleVal, grayscaleVal, grayscaleVal).rgb());
            grayScaleVector.emplace_back(grayscaleVal);
        }
    }
    auto threshold = getOtsuThreshold(grayScaleVector);
    for (auto i = 0; i < width; ++i) {
        for (auto j = 0; j < height; ++j) {
            auto pixelIntens = outputImage.pixelColor(i, j).red();
            if (pixelIntens < threshold) {
                pixelIntens = 0;
            } else {
                pixelIntens = 255;
            }
            outputImage.setPixel(i, j, QColor(pixelIntens, pixelIntens, pixelIntens).rgb());
        }
    }
    return outputImage;
}

QImage nonMaxSuppression(const QImage &inputImage, const std::vector<double> &gradAngleVector) {
    auto outImage = inputImage.copy(1, 1, inputImage.width() - 2, inputImage.height() - 2);
    auto tmpImage = outImage.copy();
    auto width = outImage.width();
    auto xSize = width - 1;
    auto ySize = outImage.height() - 1;
    for (auto x = 1; x < xSize; ++x) {
        for (auto y = 1; y < ySize; ++y) {
            int beforePixelValue;
            int afterPixelValue;
            int pixelValue = tmpImage.pixelColor(x, y).value();
            double angle = gradAngleVector[x * width + y];
            if (0 <= angle and angle < PI_GRAD / 8 or 15 * PI_GRAD / 8 <= angle and angle <= 2 * PI_GRAD / 8) {
                beforePixelValue = tmpImage.pixelColor(x, y - 1).value();
                afterPixelValue = tmpImage.pixelColor(x, y + 1).value();
            } else if (PI_GRAD <= angle and angle < 3 * PI_GRAD / 8 or 9 * PI_GRAD / 8 <= angle and
                       angle < 11 * PI_GRAD / 8) {
                beforePixelValue = tmpImage.pixelColor(x + 1, y - 1).value();
                afterPixelValue = tmpImage.pixelColor(x - 1, y + 1).value();
            } else if (3 * PI_GRAD / 8 <= angle and
                       angle < 5 * PI_GRAD / 8 or 11 * PI_GRAD / 8 <= angle < 13 * PI_GRAD / 8) {
                beforePixelValue = tmpImage.pixelColor(x - 1, y).value();
                afterPixelValue = tmpImage.pixelColor(x + 1, y).value();
            } else {
                beforePixelValue = tmpImage.pixelColor(x - 1, y - 1).value();
                afterPixelValue = tmpImage.pixelColor(x + 1, y + 1).value();

            }
            if (pixelValue > beforePixelValue and pixelValue > afterPixelValue) {
                outImage.setPixel(x, y, QColor(tmpImage.pixelColor(x, y).rgb()).rgb());
            } else {
                outImage.setPixel(x, y, QColor(0, 0, 0).rgb());


            }


        }
    }

    return outImage;

}

QImage doubleThresholding(const QImage &inputImage, int lowThreshold, int highThreshold) {
    auto outImage = inputImage.copy();
    auto xSize = inputImage.width();
    auto ySize = inputImage.height();
    for (auto i = 0; i < xSize; ++i) {
        for (auto j = 0; j < ySize; ++j) {
            int value = inputImage.pixelColor(i, j).value();
            if (value >= highThreshold) {
                value = 255;
            } else if (value < lowThreshold) {
                value = 0;
            }
            outImage.setPixel(i, j, QColor(value, value, value).rgb());
        }
    }
    return outImage;
}

QImage hysteresis(const QImage &inputImage) {
    auto xSize = inputImage.width() - 1;
    auto ySize = inputImage.height() - 1;
    auto topToBottomImage = inputImage.copy();
    for (auto x = 1; x < xSize; ++x) {
        for (auto y = 1; y < ySize; ++y) {
            int curValue = inputImage.pixelColor(x, y).value();
            if (curValue != 255) {//// it's not strong pixel
                if (inputImage.pixelColor(x, y - 1).value() == 255 or
                    inputImage.pixelColor(x, y + 1).value() == 255 or
                    inputImage.pixelColor(x + 1, y).value() == 255 or
                    inputImage.pixelColor(x - 1, y).value() == 255 or
                    inputImage.pixelColor(x - 1, y - 1).value() == 255 or
                    inputImage.pixelColor(x + 1, y + 1).value() == 255 or
                    inputImage.pixelColor(x + 1, y - 1).value() == 255 or
                    inputImage.pixelColor(x - 1, y + 1).value() == 255) {
                    topToBottomImage.setPixel(x, y, QColor(255, 255, 255).rgb());
                } else {
                    topToBottomImage.setPixel(x, y, QColor(0, 0, 0).rgb());
                }
            }
        }
    }
    auto bottomToTopImg = inputImage.copy();
    for (auto x = bottomToTopImg.width() - 2; x > 0; --x) {
        for (auto y = bottomToTopImg.height() - 2; y > 0; --y) {
            int curValue = inputImage.pixelColor(x, y).value();
            if (curValue != 255) {//// it's not strong pixel
                if (inputImage.pixelColor(x, y - 1).value() == 255 or
                    inputImage.pixelColor(x, y + 1).value() == 255 or
                    inputImage.pixelColor(x + 1, y).value() == 255 or
                    inputImage.pixelColor(x - 1, y).value() == 255 or
                    inputImage.pixelColor(x - 1, y - 1).value() == 255 or
                    inputImage.pixelColor(x + 1, y + 1).value() == 255 or
                    inputImage.pixelColor(x + 1, y - 1).value() == 255 or
                    inputImage.pixelColor(x - 1, y + 1).value() == 255) {
                    bottomToTopImg.setPixel(x, y, QColor(255, 255, 255).rgb());
                } else {
                    bottomToTopImg.setPixel(x, y, QColor(0, 0, 0).rgb());
                }
            }
        }
    }
    auto rightToLeftImage = inputImage.copy();
    for (auto x = 1; x < xSize; ++x) {
        for (auto y = rightToLeftImage.height() - 2; y > 0; --y) {
            int curValue = inputImage.pixelColor(x, y).value();
            if (curValue != 255) {//// it's not strong pixel
                if (inputImage.pixelColor(x, y - 1).value() == 255 or
                    inputImage.pixelColor(x, y + 1).value() == 255 or
                    inputImage.pixelColor(x + 1, y).value() == 255 or
                    inputImage.pixelColor(x - 1, y).value() == 255 or
                    inputImage.pixelColor(x - 1, y - 1).value() == 255 or
                    inputImage.pixelColor(x + 1, y + 1).value() == 255 or
                    inputImage.pixelColor(x + 1, y - 1).value() == 255 or
                    inputImage.pixelColor(x - 1, y + 1).value() == 255) {
                    rightToLeftImage.setPixel(x, y, QColor(255, 255, 255).rgb());
                } else {
                    rightToLeftImage.setPixel(x, y, QColor(0, 0, 0).rgb());
                }
            }
        }
    }

    auto leftToRightImage = inputImage.copy();
    for (auto x = bottomToTopImg.width() - 2; x > 0; --x) {
        for (auto y = 1; y < ySize; ++y) {
            int curValue = inputImage.pixelColor(x, y).value();
            if (curValue != 255) {//// it's not strong pixel
                if (inputImage.pixelColor(x, y - 1).value() == 255 or
                    inputImage.pixelColor(x, y + 1).value() == 255 or
                    inputImage.pixelColor(x + 1, y).value() == 255 or
                    inputImage.pixelColor(x - 1, y).value() == 255 or
                    inputImage.pixelColor(x - 1, y - 1).value() == 255 or
                    inputImage.pixelColor(x + 1, y + 1).value() == 255 or
                    inputImage.pixelColor(x + 1, y - 1).value() == 255 or
                    inputImage.pixelColor(x - 1, y + 1).value() == 255) {
                    leftToRightImage.setPixel(x, y, QColor(255, 255, 255).rgb());
                } else {
                    leftToRightImage.setPixel(x, y, QColor(0, 0, 0).rgb());
                }
            }
        }
    }
    auto outputImg = inputImage.copy();

    for (auto x = 0; x < inputImage.width(); ++x) {
        for (auto y = 0; y < inputImage.height(); ++y) {
            int value = topToBottomImage.pixelColor(x, y).value() + bottomToTopImg.pixelColor(x, y).value() +
                        leftToRightImage.pixelColor(x, y).value() + rightToLeftImage.pixelColor(x, y).value();
            value = value > 255 ? 255 : value;
            outputImg.setPixel(x, y, QColor(value, value, value).rgb());
        }
    }
    return outputImg;
}


QImage
Algorithms::CannyEdgeDetection(const QImage &inputImage, double gaussSigma, int gaussKernelSize, int lowThreshold,
                               int highThreshold) {
    auto outImage = Filters::gaussFilter(inputImage, gaussSigma, gaussKernelSize);
    std::vector<double> gradAngleVector;
    std::tie(outImage, gradAngleVector) = Filters::sobelWithGradients(outImage);
    outImage = nonMaxSuppression(outImage, gradAngleVector);
    outImage = doubleThresholding(outImage, lowThreshold, highThreshold);
    outImage = hysteresis(outImage);
    outImage = outImage.scaled(inputImage.width(), inputImage.height(), Qt::IgnoreAspectRatio,
                               Qt::SmoothTransformation);
    return outImage;


}

