//
// Created by azari on 07.09.2021.
//

#include "MainWindow.h"
#include "colorsScheme.h"
#include <QAction>
#include <QMenuBar>
#include <QApplication>
#include <QStandardPaths>
#include <QFileDialog>
#include <QMessageBox>
#include <QImageReader>
#include <QProcess>
#include <fstream>
#include <QImageWriter>

MainWindow::MainWindow(const QString &title, int minW, int minH, QWidget *parent) : QMainWindow(parent) {
    qDebug() << "main_window_size: " << this->size();
    this->setMinimumSize(minW, minH);
    this->setWindowTitle(title);

    this->imageViewer = new LabImageViewer(this);
    this->imageViewer->show();
    this->imageViewer->setGeometry(10, 20, int(minW * 0.7), int(minH * 0.9));
    this->hsvSlider = new HsvSlider(this->imageViewer, this);
    this->hsvSlider->setMinimumSize(200, 200);
    this->hsvSlider->setGeometry(800, 600, 300, 300);
    this->histogramButton = new QPushButton(this);
    this->histogramButton->setText("L-histogram");
    this->histogramButton->setGeometry(800, 300, 100, 50);
    connect(this->histogramButton, &QPushButton::clicked, this, &MainWindow::onPressHistogram);
    QMenu *imageMenu;
    imageMenu = menuBar()->addMenu("image");
    auto *saveImage = new QAction("save image", this);
    auto *loadImage = new QAction("load image", this);
    connect(saveImage, &QAction::triggered, this, &MainWindow::onPressSaveImage);
    connect(loadImage, &QAction::triggered, this, &MainWindow::onPressLoadImage);
    imageMenu->addAction(loadImage);
    imageMenu->addAction(saveImage);
    QMenu *filterMenu;
    filterMenu = menuBar()->addMenu("filters");
    auto *sobelOperator = new QAction("Sobel operator");
    auto *gaborOperator = new QAction("Gabor operator");
    connect(sobelOperator, &QAction::triggered, this, &MainWindow::onPressSobel);
    connect(gaborOperator, &QAction::triggered, this, &MainWindow::onPressGabor);
    filterMenu->addAction(sobelOperator);
    filterMenu->addAction(gaborOperator);
    QMenu *algorithms;
    algorithms = menuBar()->addMenu("algorithms");


}

void MainWindow::onPressLoadImage() {
    qDebug() << "press_load_img";
    QString qStrFilePath = QFileDialog::getOpenFileName(this, tr("Open Image"), QStandardPaths::writableLocation(
            QStandardPaths::PicturesLocation), tr("Image Files (*.png *.jpg *.bmp)"));
    QImageReader reader(qStrFilePath);
    qDebug() << (qStrFilePath);
    if (qStrFilePath.isEmpty())
        return;
    if (!reader.canRead()) {
        showMessageBox(QString("can't read image"));

    }

    QImage image = reader.read();
    image = image.scaled(imageViewer->width(), imageViewer->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    hsvSlider->setImage(image.copy());
    hsvSlider->setOriginalImage(image.copy());
    hsvSlider->resetSliders();
    imageViewer->setPixmap(QPixmap::fromImage(image));

}

void MainWindow::onPressSaveImage() {
    qDebug() << "press_save_img";
    QString qStrFilePath = QFileDialog::getSaveFileName(this,
                                                        tr("Save Image"),
                                                        QStandardPaths::writableLocation(
                                                                QStandardPaths::PicturesLocation)),
            tr("JPG file (*.jpg);;PNG file (*.png);;BMP file (*.bmp)");

    if (qStrFilePath.isEmpty())
        return;

    QImageWriter writer(qStrFilePath);
    if (!writer.canWrite()) {
        showMessageBox(QString("can't write fie"));
        return;
    }
    writer.write(imageViewer->pixmap().toImage());

}

void MainWindow::onPressGabor() {
    qDebug() << "Gabor";


}

void MainWindow::onPressSobel() {
    qDebug() << "Sobel";


}

void MainWindow::showMessageBox(const QString &msgText) {
    QMessageBox messageBox;
    messageBox.setText(msgText);
    messageBox.exec();

}

void MainWindow::onPressHistogram() {
    qDebug() << "press histogram";
    auto image = imageViewer->pixmap().toImage();
    std::vector<int> hist_list{};
    auto width = image.width();
    auto height = image.height();
    int r, g, b;
    RgbColor rgb;
    XyzColor xyz;
    LabColor lab;
    int value;
    for (auto x = 0; x < width; ++x) {
        for (auto y = 0; y < height; ++y) {
            auto img_pixel = image.pixel(x, y);
            auto color = QColor(img_pixel);
            color.getRgb(&r, &g, &b);
            rgb.r = r;
            rgb.g = g;
            rgb.b = b;
            xyz = rgbToXyz(rgb);
            lab = xyzToLab(xyz);
            value = int(lab.l);
            hist_list.emplace_back(value);
        }
    }
    {
        std::ofstream outf("hist_info.txt");
        for (auto val: hist_list) {
            outf << val << std::endl;
        }
    }


    QStringList arguments{"hist_drawer.py"};
    QProcess p;
    p.start("python", arguments);
    p.waitForFinished();


}


