#ifndef LIPREPROJECTRASTERLAYERFORM_H
#define LIPREPROJECTRASTERLAYERFORM_H


#include "liprastertransform.h"
#include <QFileDialog>
namespace Ui {
class LIPReprojectRasterLayerForm;
}

class LIPReprojectRasterLayerForm : public QDialog
{
    Q_OBJECT

public:
    explicit LIPReprojectRasterLayerForm(QWidget *parent = nullptr);
    ~LIPReprojectRasterLayerForm();

private slots:
    void on_buttonBox_accepted();

    void on_pushButtonPath_clicked();

private:
    Ui::LIPReprojectRasterLayerForm *ui;
    QString fileName;
};

#endif // LIPREPROJECTRASTERLAYERFORM_H
