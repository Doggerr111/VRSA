#ifndef LIPRASTERCONTOURSFORM_H
#define LIPRASTERCONTOURSFORM_H

#include <QFileDialog>
#include "liprasterprocessing.h"
namespace Ui {
class LIPRasterContoursForm;
}

class LIPRasterContoursForm : public QDialog
{
    Q_OBJECT

public:
    explicit LIPRasterContoursForm(QWidget *parent = nullptr);
    ~LIPRasterContoursForm();

private slots:
    void on_buttonBox_accepted();

    void on_pushButtonPath_clicked();

    void on_comboBoxRaster_currentIndexChanged(int index);

private:
    Ui::LIPRasterContoursForm *ui;
    QString fileName;
};

#endif // LIPRASTERCONTOURSFORM_H
