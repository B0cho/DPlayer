#ifndef DBWIZARDPAGE_H
#define DBWIZARDPAGE_H

#include <QWizardPage>
#include <QFileDialog>
#include <QDesktopServices>

namespace Ui {
class dbWizardPage;
}

class dbWizardPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit dbWizardPage(QWidget *parent = 0);
    ~dbWizardPage();
    static QString getMusicDirectory();

private slots:

    void on_groupBox_playlists_clicked(bool checked);
    void on_groupBox_fragments_clicked(bool checked);
    void on_lineEdit_fragments_textChanged(const QString &arg1);
    void on_lineEdit_playlists_textChanged(const QString &arg1);
    void on_pushButton_playlistsBrowse_clicked();
    void on_pushButton_fragmentsBrowse_clicked();

private:
    Ui::dbWizardPage *ui;
    bool isComplete() const;

};

#endif // DBWIZARDPAGE_H
