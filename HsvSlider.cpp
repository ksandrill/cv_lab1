//
// Created by azari on 21.09.2021.
//

#include <QLabel>
#include "HsvSlider.h"


HsvSlider::HsvSlider(LabImageViewer *imageViewer, QWidget *parent) : QWidget(parent) {
    this->imageViewer = imageViewer;
    auto hueLabel = new QLabel("h", this);
    hueLabel->setGeometry(30, 10, 20, 20);
    this->hueSlider = new QSlider(this);
    this->hueSlider->setGeometry(30, 60, 30, 90);
    this->hueSlider->setMinimum(-100);
    this->hueSlider->setMaximum(100);
    this->hueSlider->setValue(0);
    auto saturationLabel = new QLabel("s", this);
    saturationLabel->setGeometry(60, 10, 20, 20);
    this->saturationSlider = new QSlider(this);
    this->saturationSlider->setGeometry(60, 60, 30, 90);
    this->saturationSlider->setMinimum(-100);
    this->saturationSlider->setMaximum(100);
    this->saturationSlider->setValue(0);
    auto valueLabel = new QLabel("v", this);
    valueLabel->setGeometry(100, 10, 20, 20);
    this->valueSlider = new QSlider(this);
    this->valueSlider->setGeometry(90, 60, 30, 90);
    this->valueSlider->setMinimum(-100);
    this->valueSlider->setMaximum(100);
    this->valueSlider->setValue(0);
    this->value = 0;
    this->hue = 0;
    this->saturation = 0;


    ///slider position check
    connect(hueSlider, &QSlider::valueChanged, this, &HsvSlider::setHue);
    connect(saturationSlider, &QSlider::valueChanged, this, &HsvSlider::setSaturation);
    connect(valueSlider, &QSlider::valueChanged, this, &HsvSlider::setValue);
    /// change image for position
    connect(this, &HsvSlider::hueChanged, this, &HsvSlider::changeImageHue);
    connect(this, &HsvSlider::saturationChanged, this, &HsvSlider::changeImageSaturation);
    connect(this, &HsvSlider::valueChanged, this, &HsvSlider::changeImageValue);


}

void HsvSlider::setHue(int hue) {
    if (this->hue == hue)
        return;
    this->hue = hue;
    emit hueChanged(hue);
}

void HsvSlider::setSaturation(int saturation) {
    if (this->saturation == saturation)
        return;
    this->saturation = saturation;

    emit saturationChanged(saturation);

}

void HsvSlider::setValue(int value) {
    if (this->value == value) {
        return;
    }
    this->value = value;
    emit valueChanged(value);

}

void HsvSlider::changeImageSaturation(int saturation) {
    auto width = image.width();
    auto height = image.height();
    for (auto x = 0; x < width; x++) {
        for (auto y = 0; y < height; y++) {
            QColor color = originalImage.pixelColor(x, y);
            int up_saturation = color.hsvSaturation() * saturation / 100 + color.hsvSaturation();
            if (up_saturation > 255) up_saturation = 255;
            if (up_saturation < 0) up_saturation = 0;
            ////qDebug() << x << "/" << width << "," << y << " /" << height << " " << up_saturation;
            color.setHsv(color.hsvHue(),
                         up_saturation,
                         color.value());
            image.setPixelColor(x, y, color);
        }
    }
    imageViewer->setPixmap(QPixmap::fromImage(image));

}

void HsvSlider::changeImageHue(int hue) {
    auto width = image.width();
    auto height = image.height();
    for (auto x = 0; x < width; x++) {
        for (auto y = 0; y < height; y++) {
            QColor color = originalImage.pixelColor(x, y);
            int up_hue = color.hsvHue() * hue / 100 + color.hsvHue();
            if (up_hue > 359) up_hue = 359;
            if (up_hue < 0) up_hue = 0;
            ////qDebug() << x << "/" << width << "," << y << " /" << height << " " << up_hue;
            color.setHsv(up_hue,
                         color.hsvSaturation(),
                         color.value());
            image.setPixelColor(x, y, color);
        }
    }
    imageViewer->setPixmap(QPixmap::fromImage(image));

}


void HsvSlider::changeImageValue(int value) {
    auto width = image.width();
    auto height = image.height();
    for (auto x = 0; x < width; x++) {
        for (auto y = 0; y < height; y++) {
            ///qDebug() << x << "/" << width << "," << y << " /" << height;
            QColor color = originalImage.pixelColor(x, y);
            int up_val = color.value() * value / 100 + color.value();
            if (up_val > 255) up_val = 255;
            if (up_val < -1) up_val = -1;
            color.setHsv(color.hsvHue(),
                         color.hsvSaturation(),
                         up_val);
            image.setPixelColor(x, y, color);
        }
    }
    imageViewer->setPixmap(QPixmap::fromImage(image));


}

const QImage &HsvSlider::getImage() const {
    return image;
}

void HsvSlider::setImage(const QImage &image) {
    HsvSlider::image = image;
}

const QImage &HsvSlider::getOriginalImage() const {
    return originalImage;
}

void HsvSlider::setOriginalImage(const QImage &originalImage) {
    HsvSlider::originalImage = originalImage;
}

QSlider *HsvSlider::getHueSlider() const {
    return hueSlider;
}

void HsvSlider::setHueSlider(QSlider *hueSlider) {
    HsvSlider::hueSlider = hueSlider;
}

QSlider *HsvSlider::getSaturationSlider() const {
    return saturationSlider;
}

void HsvSlider::setSaturationSlider(QSlider *saturationSlider) {
    HsvSlider::saturationSlider = saturationSlider;
}

QSlider *HsvSlider::getValueSlider() const {
    return valueSlider;
}

void HsvSlider::setValueSlider(QSlider *valueSlider) {
    HsvSlider::valueSlider = valueSlider;
}

void HsvSlider::resetSliders() {
    hueSlider->setValue(0);
    saturationSlider->setValue(0);
    valueSlider->setValue(0);
}




