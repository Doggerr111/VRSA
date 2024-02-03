#include "lipcutlayerform.h"
#include "ui_lipcutlayerform.h"

LIPCutLayerForm::LIPCutLayerForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LIPCutLayerForm),
    resultLayer{nullptr},
    resultLIPLayer{nullptr}

{
    ui->setupUi(this);
}

LIPCutLayerForm::~LIPCutLayerForm()
{
    delete ui;
}

LIPVectorLayer* LIPCutLayerForm::getResult()
{
    return resultLIPLayer;
}

void LIPCutLayerForm::on_pushButtonOk_clicked()
{
    fileName=ui->lineEditPath->text();
    if (!LIPFileValidator::isPathValid(fileName))
    {
        QMessageBox::warning(this, "Ошибка", "Указан неверный путь для сохранения результирующего слоя");
        return;
    }
    QString inputPath = ui->comboBoxInput->itemData(ui->comboBoxInput->currentIndex(),Qt::ToolTipRole).toString();
    QString overlayPath = ui->comboBoxInput->itemData(ui->comboBoxOverlay->currentIndex(),Qt::ToolTipRole).toString();

    LIPVectorLayer *inputLayer = LIPProject::getInstance().getVectorLayerByPath(inputPath);
    LIPVectorLayer *overLayer = LIPProject::getInstance().getVectorLayerByPath(overlayPath);
    OGRLayer * layer = LIPVectorClipper::clip(inputLayer, overLayer);

    LIPGeometryType type = LIPVectorReader::readGeometryType(layer);
    switch (type)
    {
    case LIPGeometryType::LIPPoint:
    {

        //LIPLayerCreator *newLayer = new LIPLayerCreator(type, fileName, name);

        resultLIPLayer = new LIPPointLayer(layer,"intersectionOutput", fileName, nullptr);

        break;
    }
    case LIPGeometryType::LIPLineString:
    {
        resultLIPLayer = new LIPLineLayer(layer,"intersectionOutput", fileName, nullptr);

        break;


    }
    case LIPGeometryType::LIPPolygon:
    {
        resultLIPLayer = new LIPPolygonLayer(layer,"intersectionOutput", fileName, nullptr);
        break;
    }
    }
}


void LIPCutLayerForm::on_pushButtonPath_clicked()
{
    fileName=QFileDialog::getSaveFileName(this,"","","*.shp");
    if (fileName.endsWith(".shp"))
        ui->lineEditPath->setText(fileName);
    else
    {
        ui->lineEditPath->setText(fileName+".shp");
    }
}


void LIPCutLayerForm::on_pushButtonCancel_clicked()
{
    close();
}

