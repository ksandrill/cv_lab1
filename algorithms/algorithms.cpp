//
// Created by azari on 25.09.2021.
//

#include <algorithm>
#include "algorithms.h"
#include "cmath"
#include "qdebug.h"

int Algorithms::getOtsuThreshold(const std::vector<int> &grayScaleVector) {
    auto minmax = std::minmax_element(std::begin(grayScaleVector), std::end(grayScaleVector));
    int min = *(minmax.first);
    int max = *(minmax.second);
    auto histVector = std::vector<int>(max - min, 0);
    if (histVector.empty()) {
        return 0;
    }
    qDebug() << grayScaleVector[0] << " " << min << " " << max;
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

