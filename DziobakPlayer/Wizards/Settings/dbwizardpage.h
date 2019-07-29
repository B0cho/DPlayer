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

    void on_groupBox_database_clicked(bool checked);
    void on_lineEdit_database_textChanged(const QString &arg1);
    void on_pushButton_databaseBrowse_clicked();

private:
    Ui::dbWizardPage *ui;
    bool isComplete() const;

};

#endif // DBWIZARDPAGE_H
