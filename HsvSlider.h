//
// Created by azari on 21.09.2021.
//

#ifndef IMAGE_TRANSFORM_HSVSLIDER_H
#define IMAGE_TRANSFORM_HSVSLIDER_H


#include <QWidget>
#include <QSlider>
#include "QDebug"
#include "LabImageViewer.h"

class HsvSlider : public QWidget {
Q_OBJECT


public slots:



    void setHue(int hue);

    void setSaturation(int saturation);

    void setValue(int value);


    const QImage &getImage() const;

    void setImage(const QImage &image);

private:
    LabImageViewer *imageViewer;
    int hue{};
    int saturation{};
    int value{};
    QSlider *hueSlider;
    QSlider *saturationSlider;
    QSlider *valueSlider;
    QImage image;
    QImage originalImage;
public:
    QSlider *getHueSlider() const;
    void resetSliders();

    void setHueSlider(QSlider *hueSlider);

    QSlider *getSaturationSlider() const;

    void setSaturationSlider(QSlider *saturationSlider);

    QSlider *getValueSlider() const;

    void setValueSlider(QSlider *valueSlider);

public:
    const QImage &getOriginalImage() const;

    void setOriginalImage(const QImage &originalImage);

private slots:
public:

private:
    void changeImageSaturation(int saturation);

    void changeImageHue(int hue);

    void changeImageValue(int value);

signals:

    void hueChanged(int hue);

    void saturationChanged(int saturation);

    void valueChanged(int value);


public:
    HsvSlider(LabImageViewer *imageViewer, QWidget *parent);

    ///explicit HsvSlider(QWidget *parent = nullptr);

};


#endif //IMAGE_TRANSFORM_HSVSLIDER_H
