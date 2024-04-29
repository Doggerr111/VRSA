#ifndef LIPCUTRASTERLAYERFORM_H
#define LIPCUTRASTERLAYERFORM_H

#include <QDialog>
#include "lipcutrasterlayer.h"
#include <QFileDialog>

namespace Ui {
class LIPCutRasterLayerForm;
}

class LIPCutRasterLayerForm : public QDialog
{
    Q_OBJECT

public:
    explicit LIPCutRasterLayerForm(QWidget *parent = nullptr);
    ~LIPCutRasterLayerForm();

private slots:
    void on_buttonBox_accepted();

    void on_pushButtonPath_clicked();

private:
    Ui::LIPCutRasterLayerForm *ui;
    QString fileName;
};

#endif // LIPCUTRASTERLAYERFORM_H
