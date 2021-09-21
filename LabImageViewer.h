//
// Created by azari on 20.09.2021.
//

#ifndef IMAGE_TRANSFORM_LABIMAGEVIEWER_H
#define IMAGE_TRANSFORM_LABIMAGEVIEWER_H


#include <QLabel>
#include <QMouseEvent>
class LabImageViewer: public QLabel{
Q_OBJECT
private:
    QLabel  *schemeColors;
public:
    LabImageViewer(QWidget *parent = nullptr);
    void mouseMoveEvent(QMouseEvent *me);
    ~LabImageViewer() override =default;


};


#endif //IMAGE_TRANSFORM_LABIMAGEVIEWER_H
