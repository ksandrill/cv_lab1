//
// Created by azari on 24.09.2021.
//

#include <QImage>

#ifndef IMAGE_TRANSFORM_FILTERS_H
#define IMAGE_TRANSFORM_FILTERS_H

#endif //IMAGE_TRANSFORM_FILTERS_H
namespace Filters {
    QImage sobelFilter(const QImage &inputImg, int threshold);

    QImage gaussFilter(const QImage &inputImg, double sigma, int kernelSize);
}