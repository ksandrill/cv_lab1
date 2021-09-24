//
// Created by azari on 24.09.2021.
//

#include <QImage>

#ifndef IMAGE_TRANSFORM_FILTERS_H
#define IMAGE_TRANSFORM_FILTERS_H

#endif //IMAGE_TRANSFORM_FILTERS_H
namespace Filters {
    QImage sobelFilter(const QImage &inputImg, int threshold = 255);

    QImage gaussFilter(const QImage &inputImg, double sigma, int kernelSize);

    QImage gaborFilter(const QImage &inputImg, double lambda, double theta, double phi, double gama, int kernelSize);
}