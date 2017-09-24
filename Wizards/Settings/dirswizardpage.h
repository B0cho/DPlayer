#ifndef DIRSWIZARDPAGE_H
#define DIRSWIZARDPAGE_H

#include <QWizardPage>
#include <QListWidget>
#include <QStringList>
#include "dbwizardpage.h"

namespace Ui {
class dirsWizardPage;
}

class dirsWizardPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit dirsWizardPage(QWidget *parent = 0);
    ~dirsWizardPage();
    QStringList getDirectories() const;

private slots:
    void on_pushButton_addDirectory_clicked();

    void on_listWidget_directories_itemClicked(QListWidgetItem *item);

    void on_pushButton_deleteDirectory_clicked();

private:
    Ui::dirsWizardPage *ui;

};

#endif // DIRSWIZARDPAGE_H
