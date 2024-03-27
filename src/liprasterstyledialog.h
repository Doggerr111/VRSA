#ifndef LIPRASTERSTYLEDIALOG_H
#define LIPRASTERSTYLEDIALOG_H

#include <QDialog>
#include "raster/liprasterlayer.h"
namespace Ui {
class LIPRasterStyleDialog;
}

class LIPRasterStyleDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LIPRasterStyleDialog(LIPRasterLayer* layer, QWidget *parent = nullptr);
    ~LIPRasterStyleDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::LIPRasterStyleDialog *ui;
    LIPRasterLayer* layer;
    LIPRasterRGBStyle *mStyle;
};

#endif // LIPRASTERSTYLEDIALOG_H
