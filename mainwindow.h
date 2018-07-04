#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QString>
#include <QItemSelection> //
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
    // MODELS
    //boost::shared_ptr<CPlaylistsModel> _playlistModel;

    // key names
public slots:
    void WIND_DBErrorNotify(const bool loaded, const QFileInfo localisation) const;
    void WIND_updateSettings();

private slots:
    void WIND_enableDeleteButton(const QItemSelection deselected, const QItemSelection selected);
    void WIND_changeFragmentsList(const QItemSelection deselected, const QItemSelection selected);
};

#endif // MAINWINDOW_H
