#include "lipcutrasterlayerform.h"
#include "ui_lipcutrasterlayerform.h"

LIPCutRasterLayerForm::LIPCutRasterLayerForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LIPCutRasterLayerForm)
{
    ui->setupUi(this);
}

LIPCutRasterLayerForm::~LIPCutRasterLayerForm()
{
    delete ui;
}

void LIPCutRasterLayerForm::on_buttonBox_accepted()
{
    LIPPolygonLayer *mask = dynamic_cast<LIPPolygonLayer*>(ui->comboBoxVector->getVectorLayer());
    LIPCutRasterLayer::cutRasterByVector(ui->comboBoxRaster->getRasterLayer(), mask);

}

