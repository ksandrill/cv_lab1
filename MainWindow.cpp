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
#include "algorithms/filters.h"
#include "ParamDialog.h"
#include "algorithms//algorithms.h"
#include "algorithms/segmentation.h"

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
    auto *resetImage = new QAction("reset image", this);
    connect(resetImage, &QAction::triggered, this, &MainWindow::onPressResetImage);
    connect(saveImage, &QAction::triggered, this, &MainWindow::onPressSaveImage);
    connect(loadImage, &QAction::triggered, this, &MainWindow::onPressLoadImage);
    imageMenu->addAction(loadImage);
    imageMenu->addAction(saveImage);
    imageMenu->addAction(resetImage);
    QMenu *filterMenu;
    filterMenu = menuBar()->addMenu("filters");
    auto *sobelOperator = new QAction("Sobel operator", this);
    auto *gaborOperator = new QAction("Gabor operator", this);
    auto *gaussOperator = new QAction("Gauss operator", this);
    connect(gaussOperator, &QAction::triggered, this, &MainWindow::onPressGauss);
    connect(sobelOperator, &QAction::triggered, this, &MainWindow::onPressSobel);
    connect(gaborOperator, &QAction::triggered, this, &MainWindow::onPressGabor);
    filterMenu->addAction(sobelOperator);
    filterMenu->addAction(gaborOperator);
    filterMenu->addAction(gaussOperator);
    QMenu *algoMenu;
    algoMenu = menuBar()->addMenu("algorithms");
    auto *otsu = new QAction("Otsu", this);
    connect(otsu, &QAction::triggered, this, &MainWindow::onPressOtsu);
    auto *kanny = new QAction("kanny", this);
    connect(kanny, &QAction::triggered, this, &MainWindow::onPressKanny);
    algoMenu->addAction(otsu);
    algoMenu->addAction(kanny);
    auto *meanShiftSegmentation = new QAction("Mean Shift seg", this);
    connect(meanShiftSegmentation, &QAction::triggered, this, &MainWindow::onPressMeanShift);
    algoMenu->addAction(meanShiftSegmentation);
    auto *kmeansSegmentation = new QAction("K-means seg", this);
    connect(kmeansSegmentation, &QAction::triggered, this, &MainWindow::onPressKmeans);
    algoMenu->addAction(kmeansSegmentation);

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
    hsvSlider->setImage(image);
    hsvSlider->setOriginalImage(image);
    hsvSlider->resetSliders();
    this->origPixmap = QPixmap::fromImage(image);
    imageViewer->setPixmap(origPixmap);

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
    ParamDialog dlg(this, QString("lambda, theta, phi,gama,kernel_size"));
    switch (dlg.exec()) {
        case QDialog::Accepted: {
            auto params = dlg.getEditStr()->text().split(",");
            auto lambda = params[0].toDouble();
            auto theta = params[1].toDouble();
            auto phi = params[2].toDouble();
            auto gama = params[3].toDouble();
            auto kernelSize = params[4].toInt();
            auto outImg = Filters::gaborFilter(imageViewer->pixmap().toImage(), lambda, theta, phi, gama, kernelSize);
            imageViewer->setPixmap(QPixmap::fromImage(outImg));
            hsvSlider->setOriginalImage(outImg);
            hsvSlider->setImage(outImg);
            break;
        }
        case QDialog::Rejected:
            qDebug() << "Rejected";
            break;
        default:
            qDebug() << "Unexpected";
    }


}

void MainWindow::onPressSobel() {
    auto outImg = Filters::sobelFilter(imageViewer->pixmap().toImage()); /// no handle input
    imageViewer->setPixmap(QPixmap::fromImage(outImg));
    hsvSlider->setOriginalImage(outImg);
    hsvSlider->setImage(outImg);

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

void MainWindow::onPressGauss() {
    ParamDialog dlg(this, QString("sigma, kernel_size "));
    switch (dlg.exec()) {
        case QDialog::Accepted: { ///no validation
            qDebug() << "Accepted";
            auto params = dlg.getEditStr()->text().split(",");
            auto sigma = params[0].toDouble();
            auto kernelSize = params[1].toInt();
            auto outImg = Filters::gaussFilter(imageViewer->pixmap().toImage(), sigma, kernelSize);
            imageViewer->setPixmap(QPixmap::fromImage(outImg));
            hsvSlider->setOriginalImage(outImg);
            hsvSlider->setImage(outImg);
            break;
        }
        case QDialog::Rejected:
            qDebug() << "Rejected";
            break;
        default:
            qDebug() << "Unexpected";
    }

}

const QPixmap &MainWindow::getOrigPixmap() const {
    return origPixmap;
}

void MainWindow::setOrigPixmap(const QPixmap &origPixmap) {
    MainWindow::origPixmap = origPixmap;
}

void MainWindow::onPressResetImage() {
    imageViewer->setPixmap(origPixmap);
    auto image = origPixmap.toImage();
    hsvSlider->setImage(image);
    hsvSlider->setOriginalImage(image);
    hsvSlider->resetSliders();


}

void MainWindow::onPressOtsu() {
    auto outImg = Algorithms::OtsuBinarization(imageViewer->pixmap().toImage());
    imageViewer->setPixmap(QPixmap::fromImage(outImg));
    hsvSlider->setOriginalImage(outImg);
    hsvSlider->setImage(outImg);
}

void MainWindow::onPressKanny() {


    ParamDialog dlg(this, QString("sigma, kernel_size, low_thr, high_thr "));
    switch (dlg.exec()) {
        case QDialog::Accepted: { ///no validation
            qDebug() << "Accepted";
            auto params = dlg.getEditStr()->text().split(",");
            auto sigma = params[0].toDouble();
            auto kernelSize = params[1].toInt();
            auto lowThr = params[2].toInt();
            auto HighThr = params[3].toInt();
            auto outImg = Algorithms::CannyEdgeDetection(imageViewer->pixmap().toImage(), sigma, kernelSize, lowThr,
                                                         HighThr);
            imageViewer->setPixmap(QPixmap::fromImage(outImg));
            hsvSlider->setOriginalImage(outImg);
            hsvSlider->setImage(outImg);
            break;
        }
        case QDialog::Rejected:
            qDebug() << "Rejected";
            break;
        default:
            qDebug() << "Unexpected";
    }
}

void MainWindow::onPressMeanShift() {
    qDebug() << "MeanShift";
    ParamDialog dlg(this, QString("iterations, radius, scaleFactor"));
    switch (dlg.exec()) {
        case QDialog::Accepted: {
            auto params = dlg.getEditStr()->text().split(",");
            auto iter = params[0].toInt();
            auto radius = params[1].toFloat();
            auto scaleFactor = params[2].toFloat();
            auto inputImg = imageViewer->pixmap().toImage();
            inputImg = inputImg.scaled(int(float(inputImg.width()) * scaleFactor),
                                       int(float(inputImg.height()) * scaleFactor));

            auto outImg = Segmentation::meanShiftSegmentation(inputImg, radius,
                                                              iter);
            outImg = outImg.scaled(imageViewer->pixmap().toImage().size());
            imageViewer->setPixmap(QPixmap::fromImage(outImg));
            hsvSlider->setOriginalImage(outImg);
            hsvSlider->setImage(outImg);
            break;
        }
        case QDialog::Rejected:
            qDebug() << "Rejected";
            break;
        default:
            qDebug() << "Unexpected";
    }

}

void MainWindow::onPressKmeans() {
    qDebug() << "kmeans";
    ParamDialog dlg(this, QString("iterations, cluster_count, scaleFactor"));
    switch (dlg.exec()) {
        case QDialog::Accepted: {
            auto params = dlg.getEditStr()->text().split(",");
            auto iter = params[0].toInt();
            auto cluster_count = params[1].toInt();
            auto scaleFactor = params[2].toFloat();
            auto inputImg = imageViewer->pixmap().toImage();
            inputImg = inputImg.scaled(int(float(inputImg.width()) * scaleFactor),
                                       int(float(inputImg.height()) * scaleFactor));

            auto outImg = Segmentation::kMeansSegmentation(inputImg, cluster_count,
                                                           iter);
            outImg = outImg.scaled(imageViewer->pixmap().toImage().size());
            imageViewer->setPixmap(QPixmap::fromImage(outImg));
            hsvSlider->setOriginalImage(outImg);
            hsvSlider->setImage(outImg);
            break;
        }
        case QDialog::Rejected:
            qDebug() << "Rejected";
            break;
        default:
            qDebug() << "Unexpected";
    }

}
