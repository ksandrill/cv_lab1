//
// Created by azari on 24.09.2021.
//

#ifndef IMAGE_TRANSFORM_PARAMDIALOG_H
#define IMAGE_TRANSFORM_PARAMDIALOG_H


#include <QString>
#include <QLineEdit>
#include <QDialog>

class ParamDialog : public QDialog {
Q_OBJECT

public:
    ParamDialog(QWidget *parent = nullptr, const QString &title = "param");

    ~ParamDialog();

    QLineEdit *getEditStr() const;

    void setEditStr(QLineEdit *editStr);

signals:

    void applied();

private:
    QLineEdit *editStr;
};


#endif //IMAGE_TRANSFORM_PARAMDIALOG_H
