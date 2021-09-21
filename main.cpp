#include <QApplication>
#include "MainWindow.h"



int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    auto *window = new MainWindow(QString("dafaq"), 1000, 800);
    window->show();
    return QApplication::exec();
}
