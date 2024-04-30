#include "liptriangulationgeosform.h"
#include "ui_liptriangulationgeosform.h"

LIPTriangulationGeosForm::LIPTriangulationGeosForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LIPTriangulationGeosForm),
    outputLayer(nullptr)
{
    ui->setupUi(this);
    ui->lineEditFileName->setReadOnly(true);
    setWindowTitle("Триангуляция");
}

LIPTriangulationGeosForm::~LIPTriangulationGeosForm()
{
    outputLayer=nullptr;
    //delete outputLayer;
    delete ui;
}

void LIPTriangulationGeosForm::on_pushButtonFileDialog_clicked()
{
    fileName=QFileDialog::getSaveFileName(this, "Выберите путь для сохранения результата", LIPProject::getInstance().getVectorDataFolder(),
                                          "Shape files (*.shp)");
    if (fileName.isEmpty())
    {
        QMessageBox::warning(this,"Ошибка", "Указан неверный путь");
        return;
    }

    if (!fileName.endsWith(".shp"))
        fileName.append(".shp");
    ui->lineEditFileName->setText(fileName);
}




LIPVectorLayer *LIPTriangulationGeosForm::getTriangulationLayer()
{
    return outputLayer;
}


void LIPTriangulationGeosForm::on_buttonBox_accepted()
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
    {
        QMessageBox::warning(this,"Ошибка", "В качестве исходного слоя должен быть выбрать слой с точечной геометрией");
        return;
    }
    const auto triangles = LIPTriangulationGeos::getTriangulation(inputLayer);
//    std::shared_ptr<LIPCoordinateSystem> crs= std::make_shared<LIPCoordinateSystem>();
//    crs->setOGRSpatialRef(inputLayer->getOGRLayer()->GetSpatialRef());
    LIPLayerCreator *lCr=new LIPLayerCreator(LIPGeometryType::LIPPolygon, fileName, "outputLayer",inputLayer->getCRS());
    outputLayer=lCr->returnLayer();
    for (auto triangle : triangles)
    {
        outputLayer->addFeature(triangle, QVector<LIPAttribute>());
    }
    outputLayer->update();
    delete lCr;
    close();
}


void LIPTriangulationGeosForm::on_buttonBox_rejected()
{
    close();
}

