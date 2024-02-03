#ifndef LIPCUTLAYERFORM_H
#define LIPCUTLAYERFORM_H

#include <QDialog>
#include "lipcombobox.h"
#include "lipvectorclipper.h"
#include <QFileDialog>
#include <QMessageBox>
#include "lipfilevalidator.h"
#include "liplinelayer.h"
#include "lippointlayer.h"
#include "lippolygonlayer.h"
#include "lipvectorreader.h"

namespace Ui {
class LIPCutLayerForm;
}

class LIPCutLayerForm : public QDialog
{
    Q_OBJECT

public:
    explicit LIPCutLayerForm(QWidget *parent = nullptr);
    ~LIPCutLayerForm();
    LIPVectorLayer* getResult();

private slots:
    void on_pushButtonOk_clicked();

    void on_pushButtonPath_clicked();

    void on_pushButtonCancel_clicked();

private:
    Ui::LIPCutLayerForm *ui;
    QString fileName;
    OGRLayer* resultLayer;
    LIPVectorLayer *resultLIPLayer;
};

#endif // LIPCUTLAYERFORM_H
