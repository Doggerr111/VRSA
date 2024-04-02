#include "lipvoronoidiagramform.h"
#include "ui_lipvoronoidiagramform.h"

LIPVoronoiDiagramForm::LIPVoronoiDiagramForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LIPVoronoiDiagramForm)
{
    ui->setupUi(this);
    setWindowTitle("Диаграмма Вороного");
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
    {
        QMessageBox::warning(this, "Ошибка", "В качестве исходного слоя необходимо выбрать слой с точечной геометрией");
        return;
    }
    const auto polygons = LIPTriangulationGeos::getVoronoiDiagram(inputLayer);
    std::shared_ptr<LIPCoordinateSystem> crs= std::make_shared<LIPCoordinateSystem>();
    crs->setOGRSpatialRef(inputLayer->getOGRLayer()->GetSpatialRef());
    LIPLayerCreator *lCr=new LIPLayerCreator(LIPGeometryType::LIPPolygon, fileName, "outputLayer",
                                             crs.get());
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
    fileName=QFileDialog::getSaveFileName(this, "Выберите путь для сохранения результата", LIPProject::getInstance().getVectorDataFolder(),
                                          "Shape files (*.shp)");
    if (fileName.isEmpty())
    {
        QMessageBox::warning(this,"Ошибка", "Указан неверный путь");
        return;
    }

    if (!fileName.endsWith(".shp"))
        fileName.append(".shp");
    ui->lineEditPath->setText(fileName);
}

