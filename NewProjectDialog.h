#ifndef NEWPROJECTDIALOG_H
#define NEWPROJECTDIALOG_H

#include <QDialog>
#include <QDir>
#include <QMessageBox>
#include <QFileDialog>
#include <QStandardPaths>
#include "Project.h"

namespace Ui {
class NewProjectDialog;
}

class NewProjectDialog : public QDialog
{
    Q_OBJECT

public:
    NewProjectDialog(QWidget *parent = 0);
    NewProjectDialog(Project *modproject, QWidget *parent = 0);
    virtual ~NewProjectDialog();

    void        launch();
    Project*    getProject() const;

private slots:
    void    on_Confirm_clicked();
    void    on_Browse_clicked();

    void on_projectName_textChanged(const QString &text);

private:
    bool    checkForm();


    Ui::NewProjectDialog* _ui;
    Project*            _project;
};

#endif // NEWPROJECTDIALOG_H
