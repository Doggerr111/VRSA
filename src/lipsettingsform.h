#ifndef LIPSETTINGSFORM_H
#define LIPSETTINGSFORM_H

#include <QFileDialog>
#include "lipwidgetmanager.h"
#include <QAction>
#include <QMenu>
#include <QCheckBox>

namespace Ui {
class LIPSettingsForm;
}

class LIPSettingsForm : public QDialog
{
    Q_OBJECT

public:
    explicit LIPSettingsForm(QWidget *parent = nullptr);
    ~LIPSettingsForm();


private slots:
    void on_pushButtonToolBarSettings_clicked();

    void on_tabWidget_currentChanged(int index);

    void on_pushButtonVectorDir_clicked();

    void on_pushButtonRasterDir_clicked();

    void on_pushButtonDirSave_clicked();

    void on_pushButtonMapSettingSave_clicked();

    void on_pushButtonMapColor_clicked();

private:
    Ui::LIPSettingsForm *ui;
    /** */
    void setActionsTable();
    bool isMapColChanged;
};

#endif // LIPSETTINGSFORM_H
