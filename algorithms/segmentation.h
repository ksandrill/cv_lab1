//
// Created by azari on 12.10.2021.
//

#ifndef IMAGE_TRANSFORM_SEGMENTATION_H
#define IMAGE_TRANSFORM_SEGMENTATION_H

#include <QImage>

namespace Segmentation {
    class Point5D {
    public:
        int x;
        int y;
        int r;
        int g;
        int b;

        Point5D(int x, int y, int r, int g, int b);

        Point5D(const Point5D &otherPoint) = default;

        friend std::ostream &operator<<(std::ostream &os, const Point5D &point) {
            return os << "r: " << point.r << " " << "g: " << point.g
                      << " b: " << point.b << " " << "x: " << point.x << " " << "y: " << point.y;;
        }

        Point5D &operator+=(const Point5D &otherPoint);

        Point5D &operator/=(int val);

        bool operator==(const Point5D &otherPoint) const;

        bool operator<(const Point5D &otherPoint) const;

        [[nodiscard]] double spatialDistance(const Point5D &otherPoint) const;

        [[nodiscard]] double colorDistance(const Point5D &otherPoint) const;

        [[nodiscard]] double weightedDistance(const Point5D &otherPoint) const;

        static double weightedDistance(const Point5D &a, const Point5D &b);

        static double spatialDistance(const Point5D &a, const Point5D &b);

        static double colorDistance(const Point5D &a, const Point5D &b);

    };

    auto kMeansSegmentation(const QImage &inputImg, int clusterCount, int iterCount) -> QImage;

    auto getKmeansCentroids(const std::vector<Point5D> &imagePoints, int clusterCount, int iterCount) -> std::vector<Point5D>;

    auto
    getMeanShiftCentroids(const std::vector<Point5D> &imagePoints, float radius, int iterCount) -> std::vector<Point5D>;

    auto
    meanShiftSegmentation(const QImage &inputImg, float radius, int iterCounter) -> QImage;
}
#endif //IMAGE_TRANSFORM_SEGMENTATION_H
