#ifndef LIPCUTRASTERLAYERFORM_H
#define LIPCUTRASTERLAYERFORM_H

#include <QDialog>
#include "lipcutrasterlayer.h"

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

private:
    Ui::LIPCutRasterLayerForm *ui;
};

#endif // LIPCUTRASTERLAYERFORM_H
