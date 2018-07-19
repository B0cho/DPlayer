#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QString>
#include <QItemSelection>
#include <algorithm>
#include "csettings.h"
#include "cmediabase.h"

/// MAIN WINDOW
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /// ctors dtors
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    /// gui
    Ui::MainWindow *ui;
    /// members
    CSettings* const settings; // settings of app
    CMediaBase* const base; // media library

public slots:
    void WIND_DBErrorNotify(const bool loaded, const QFileInfo localisation) const; // throws notify about database file load error
    void WIND_updateSettings(); // updates settings

private slots:
    void WIND_enableDeleteButton(QItemSelection selected, QItemSelection deselected);
    void on_addPushButton_clicked();

    void on_deletePushButton_clicked();
};

#endif // MAINWINDOW_H
