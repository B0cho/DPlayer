#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include "csettings.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    // ctors
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    // members


private:
    Ui::MainWindow *ui;
    // settings
    CSettings* settings;
    // members

    // key names
private slots:


};

#endif // MAINWINDOW_H
