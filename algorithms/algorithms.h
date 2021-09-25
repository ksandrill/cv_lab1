//
// Created by azari on 25.09.2021.
//

#ifndef IMAGE_TRANSFORM_ALGORITHMS_H
#define IMAGE_TRANSFORM_ALGORITHMS_H

#include <vector>
#include <QImage>

namespace Algorithms {
    int getOtsuThreshold(const std::vector<int> &grayScaleVector);
    QImage OtsuBinarization(const QImage &inputImage);
}

#endif //IMAGE_TRANSFORM_ALGORITHMS_H


