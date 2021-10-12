//
// Created by azari on 12.10.2021.
//

#include <set>
#include <iostream>
#include <random>
#include "segmentation.h"
#include "QDebug"

#define DISTANCE_RATIO 1.75

auto
Segmentation::getMeanShiftCentroids(const std::vector<Point5D> &imagePoints, float radius,
                                    int iterCount) -> std::vector<Point5D> {
    auto centroids = imagePoints;/// firstly, centroids - all image points
    std::cout << "start make centroids" << std::endl;
    for (auto i = 0; i < iterCount; ++i) {
        std::cout << "iteration: " << i + 1 << std::endl;
        auto newCentroids = std::vector<Point5D>();
        for (auto curCentroid: centroids) {
            auto inCluster = std::vector<Point5D>();
            Point5D avgPoint(0, 0, 0, 0, 0);
            for (auto point: imagePoints) {
                if (curCentroid.weightedDistance(point) < radius) {
                    inCluster.emplace_back(point);
                    avgPoint += point;

                }
            }
            avgPoint /= int(inCluster.size());
            newCentroids.emplace_back(avgPoint); /// recalculate centroids and append them in vector
        }
        std::set<Point5D> s(newCentroids.begin(), newCentroids.end());
        centroids.assign(s.begin(), s.end());
    }
    std::cout << " centroids created" << std::endl;
    return centroids;


}

auto
Segmentation::meanShiftSegmentation(const QImage &inputImg, float radius, int iterCounter) -> QImage {
    auto outputImage = inputImg.copy();
    auto imagePoints = std::vector<Point5D>();
    auto imgWidth = inputImg.width();
    auto imgHeight = inputImg.height();
    for (auto x = 0; x < imgWidth; ++x) {
        for (auto y = 0; y < imgHeight; ++y) {
            int r, g, b;
            inputImg.pixelColor(x, y).getRgb(&r, &g, &b);
            imagePoints.emplace_back(Point5D(x, y, r, g, b));
        }
    }
    auto centroids = Segmentation::getMeanShiftCentroids(imagePoints, radius, iterCounter);
    auto minPoint = Point5D(0, 0, 0, 0, 0);
    auto maxPoint = Point5D(imgWidth - 1, imgHeight - 1, 255, 255, 255);
    auto startWeightedDist = Segmentation::Point5D::weightedDistance(maxPoint, minPoint);
    std::cout << " start clastering" << std::endl;
    for (auto point: imagePoints) {

        auto minWeightedDist = startWeightedDist;
        auto pointCentroid = maxPoint;
        for (auto centroid: centroids) {
            auto weightedDist = point.weightedDistance(centroid);
            if (weightedDist < minWeightedDist) {
                pointCentroid = centroid;
                minWeightedDist = weightedDist;
            }
        }
        outputImage.setPixelColor(point.x, point.y, QColor(pointCentroid.r, pointCentroid.g, pointCentroid.b).rgb());
    }
    std::cout << " end clustering" << std::endl;
//    for (auto centroid: centroids) {
//        std::cout << centroid << std::endl;
//    }
    std::cout << "cluster_number: " << centroids.size() << std::endl;
    return outputImage;
}

auto Segmentation::kMeansSegmentation(const QImage &inputImg, int clusterCount, int iterCount) -> QImage {
    auto outputImg = inputImg.copy();
    auto imagePoints = std::vector<Point5D>();
    auto imgWidth = inputImg.width();
    auto imgHeight = inputImg.height();
    for (auto x = 0; x < imgWidth; ++x) {
        for (auto y = 0; y < imgHeight; ++y) {
            int r, g, b;
            inputImg.pixelColor(x, y).getRgb(&r, &g, &b);
            imagePoints.emplace_back(Point5D(x, y, r, g, b));
        }
    }
    auto centroids = getKmeansCentroids(imagePoints, clusterCount, iterCount);
    auto minPoint = Point5D(0, 0, 0, 0, 0);
    auto maxPoint = Point5D(imgWidth - 1, imgHeight - 1, 255, 255, 255);
    auto startWeightedDist = Segmentation::Point5D::weightedDistance(maxPoint, minPoint);
    std::cout << " start clastering" << std::endl;
    for (auto point: imagePoints) {
        auto minWeightedDist = startWeightedDist;
        auto pointCentroid = maxPoint;
        for (auto centroid: centroids) {
            auto weightedDist = point.weightedDistance(centroid);
            if (weightedDist < minWeightedDist) {
                pointCentroid = centroid;
                minWeightedDist = weightedDist;
            }
        }
        outputImg.setPixelColor(point.x, point.y, QColor(pointCentroid.r, pointCentroid.g, pointCentroid.b).rgb());
    }
    std::cout << " end clustering" << std::endl;
    return outputImg;
}

auto
Segmentation::getKmeansCentroids(const std::vector<Point5D> &imagePoints, int clusterCount,
                                 int iterCount) -> std::vector<Point5D> {
    auto centroids = std::vector<Point5D>();
    auto pointsNumber = imagePoints.size();
    std::random_device rd;
    std::mt19937 mersenne(rd());
    auto pos = mersenne() % pointsNumber;
    centroids.emplace_back(imagePoints[pos]);
    for (auto i = 1; i < clusterCount; ++i) {
        auto distances = std::vector<double>();
        for (auto point: imagePoints) {
            auto d = std::numeric_limits<double>::max();
            for (auto centroid: centroids) {
                d = std::min(d, point.weightedDistance(centroid));
            }
            distances.emplace_back(d);
        }
        auto newCentroidPos = std::distance(distances.begin(), std::max_element(distances.begin(), distances.end()));
        centroids.emplace_back(imagePoints[newCentroidPos]);
    }

    for(auto centroid: centroids){
        std::cout << centroid << std::endl;
    }

    for (auto iter = 0; iter < iterCount; ++iter) {
        std::cout <<"iteration: " <<  iter + 1 << std::endl;
        auto clusterMembersCount = std::vector<int>(clusterCount, 0);
        auto avgClusterMembers = std::vector<Point5D>(clusterCount, Point5D(0, 0, 0, 0, 0));
        for (auto point: imagePoints) {
            auto clusterId = 0;
            auto d = std::numeric_limits<double>::max();
            for (auto centroidId = 0; centroidId < clusterCount; ++centroidId) {
                auto curD = centroids[centroidId].weightedDistance(point);
                if (d >= curD) {
                    d = curD;
                    clusterId = centroidId;
                }
            }
            clusterMembersCount[clusterId] += 1;
            avgClusterMembers[clusterId] += point;
        }
        for (auto i = 0; i < clusterCount; ++i) {
            avgClusterMembers[i] /= clusterMembersCount[i];
        }
        centroids = avgClusterMembers;

    }

    return centroids;
}

Segmentation::Point5D::Point5D(int x, int y, int r, int g, int b) : x(x), y(y), r(r), g(g), b(b) {}

double Segmentation::Point5D::spatialDistance(const Segmentation::Point5D &otherPoint) const {

    return sqrt((pow(this->x - otherPoint.x, 2) + pow(this->y - otherPoint.y, 2)));
}

double Segmentation::Point5D::colorDistance(const Segmentation::Point5D &otherPoint) const {
    return sqrt(pow(this->r - otherPoint.r, 2) + pow(this->g - otherPoint.g, 2) + pow(this->b - otherPoint.b, 2));
}


Segmentation::Point5D &Segmentation::Point5D::operator+=(const Segmentation::Point5D &otherPoint) {
    this->x += otherPoint.x;
    this->y += otherPoint.y;
    this->r += otherPoint.r;
    this->g += otherPoint.g;
    this->b += otherPoint.b;
    return *this;
}

Segmentation::Point5D &Segmentation::Point5D::operator/=(int val) {
    this->x /= val;
    this->y /= val;
    this->r /= val;
    this->g /= val;
    this->b /= val;
    return *this;
}

bool Segmentation::Point5D::operator==(const Segmentation::Point5D &otherPoint) const {
    return std::tie(x, y, r, g, b) == std::tie(otherPoint.x, otherPoint.y, otherPoint.r, otherPoint.g, otherPoint.b);
}

bool Segmentation::Point5D::operator<(const Segmentation::Point5D &otherPoint) const {
    return std::tie(x, y, r, g, b) < std::tie(otherPoint.x, otherPoint.y, otherPoint.r, otherPoint.g, otherPoint.b);
}

double Segmentation::Point5D::spatialDistance(const Segmentation::Point5D &a, const Segmentation::Point5D &b) {
    return a.spatialDistance(b);
}

double Segmentation::Point5D::colorDistance(const Segmentation::Point5D &a, const Segmentation::Point5D &b) {
    return a.colorDistance(b);
}

double Segmentation::Point5D::weightedDistance(const Segmentation::Point5D &otherPoint) const {
    auto spatialRec = pow(x - otherPoint.x, 2) + pow(y - otherPoint.y, 2);
    auto colorRec = pow(this->r - otherPoint.r, 2) + pow(this->g - otherPoint.g, 2) + pow(this->b - otherPoint.b, 2);

    return sqrt(spatialRec * DISTANCE_RATIO + colorRec);
}

double Segmentation::Point5D::weightedDistance(const Segmentation::Point5D &a, const Segmentation::Point5D &b) {
    return a.weightedDistance(b);
}


