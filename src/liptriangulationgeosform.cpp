#include "liptriangulationgeosform.h"
#include "ui_liptriangulationgeosform.h"

LIPTriangulationGeosForm::LIPTriangulationGeosForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LIPTriangulationGeosForm),
    outputLayer(nullptr)
{
    ui->setupUi(this);
    ui->lineEditFileName->setReadOnly(true);
}

LIPTriangulationGeosForm::~LIPTriangulationGeosForm()
{
    outputLayer=nullptr;
    //delete outputLayer;
    delete ui;
}

void LIPTriangulationGeosForm::on_pushButtonFileDialog_clicked()
{
    fileName=QFileDialog::getSaveFileName(this, "", "");
    if (fileName.isEmpty())
        QMessageBox::warning(this,"Ошибка", "Указан неверный путь");
    else{
        ui->lineEditFileName->setText(fileName);
    }
}


void LIPTriangulationGeosForm::on_pushButtonOk_clicked()
{
    if (fileName.isEmpty())
    {
        QMessageBox::warning(this,"Ошибка", "Указан неверный путь для сохранения результирующего слоя");
        return;
    }
    LIPVectorLayer* inputLayer = LIPProject::getInstance().getVectorLayerByPath(
                ui->comboBoxLayers->itemData(ui->comboBoxLayers->currentIndex()).toString());
    if (inputLayer==nullptr)
        return;
    if (!LIPVectorTypeChecker::isPointLayer(inputLayer))
        return;
    const auto triangles = LIPTriangulationGeos::getTriangulation(inputLayer);
    LIPLayerCreator *lCr=new LIPLayerCreator(LIPGeometryType::LIPPolygon, fileName, "outputLayer",
                                             dynamic_cast<LIPCoordinateSystem*>(inputLayer->getOGRLayer()->GetSpatialRef()));
    outputLayer=lCr->returnLayer();
    for (auto triangle : triangles)
    {
        outputLayer->addFeature(triangle, QVector<LIPAttribute>());
    }
    outputLayer->update();

    close();




}


void LIPTriangulationGeosForm::on_pushButtonCancel_clicked()
{

    close();
}

LIPVectorLayer *LIPTriangulationGeosForm::getTriangulationLayer()
{
    return outputLayer;
}

