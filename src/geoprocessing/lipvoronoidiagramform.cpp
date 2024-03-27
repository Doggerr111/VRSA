#include "lipvoronoidiagramform.h"
#include "ui_lipvoronoidiagramform.h"

LIPVoronoiDiagramForm::LIPVoronoiDiagramForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LIPVoronoiDiagramForm)
{
    ui->setupUi(this);
}

LIPVoronoiDiagramForm::~LIPVoronoiDiagramForm()
{
    delete ui;
}

void LIPVoronoiDiagramForm::on_buttonBox_accepted()
{
    if (fileName.isEmpty())
    {
        QMessageBox::warning(this,"Ошибка", "Указан неверный путь для сохранения результирующего слоя");
        return;
    }
    LIPVectorLayer* inputLayer = LIPProject::getInstance().getVectorLayerByPath(
                ui->comboBoxInput->itemData(ui->comboBoxInput->currentIndex()).toString());
    if (inputLayer==nullptr)
        return;
    if (!LIPVectorTypeChecker::isPointLayer(inputLayer))
        return;
    const auto polygons = LIPTriangulationGeos::getVoronoiDiagram(inputLayer);
    LIPLayerCreator *lCr=new LIPLayerCreator(LIPGeometryType::LIPPolygon, fileName, "outputLayer",
                                             dynamic_cast<LIPCoordinateSystem*>(inputLayer->getOGRLayer()->GetSpatialRef()));
    outputLayer=lCr->returnLayer();
    for (auto polygon : polygons)
    {
        outputLayer->addFeature(polygon, QVector<LIPAttribute>());
    }
    outputLayer->update();

    close();
}


void LIPVoronoiDiagramForm::on_pushButtonPath_clicked()
{
    fileName=QFileDialog::getSaveFileName(this, "", "");
    if (fileName.isEmpty())
        QMessageBox::warning(this,"Ошибка", "Указан неверный путь");
    else{
        ui->lineEditPath->setText(fileName);
    }
}

