//
// Created by azari on 24.09.2021.
//

#include <QBoxLayout>
#include "ParamDialog.h"
#include <QPushButton>

ParamDialog::ParamDialog(QWidget *parent, const QString &title) : QDialog(parent) {
    this->setWindowTitle(title);
    QBoxLayout *layout = new QHBoxLayout(this);
    editStr = new QLineEdit(this);
    layout->addWidget(editStr);

    auto *okBtn = new QPushButton("OK", this);
    connect(okBtn, SIGNAL(clicked()), SLOT(accept()));
    layout->addWidget(okBtn);

    auto *cancelBtn = new QPushButton("Cancel", this);
    connect(cancelBtn, SIGNAL(clicked()), SLOT(reject()));
    layout->addWidget(cancelBtn);

    setLayout(layout);
}

ParamDialog::~ParamDialog() = default;

QLineEdit *ParamDialog::getEditStr() const {
    return editStr;
}

void ParamDialog::setEditStr(QLineEdit *editStr) {
    ParamDialog::editStr = editStr;
}


