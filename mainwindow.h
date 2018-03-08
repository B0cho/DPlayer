#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QString>
#include "csettings.h"
#include "cmediabase.h"

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
    CSettings* const settings;
    CMediaBase* const base;
    // members

    // key names
private slots:
    void WIND_DBErrorNotify(const bool loaded, const QFileInfo localisation) const;


};

#endif // MAINWINDOW_H
