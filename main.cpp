#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    // app initiation
    QApplication a(argc, argv);
    QCoreApplication::setApplicationName("DziobakPlayer");
    QCoreApplication::setApplicationVersion("1.0");
    QCoreApplication::setOrganizationName("DziobakSoft");

    // window
    MainWindow w;
    w.show();
    return a.exec();
}
