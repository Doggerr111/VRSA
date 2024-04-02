#include "lipvectorunionform.h"
#include "ui_lipvectorunionform.h"

LIPVectorUnionForm::LIPVectorUnionForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LIPVectorUnionForm),
    outputLayer{nullptr}
{
    ui->setupUi(this);
    setWindowTitle("Объединение");
}

LIPVectorUnionForm::~LIPVectorUnionForm()
{
    delete ui;
}

void LIPVectorUnionForm::on_buttonBox_accepted()
{
    if (fileName.isEmpty())
    {
        QMessageBox::warning(this,"Ошибка", "Указан неверный путь для сохранения результирующего слоя");
        return;
    }
    LIPVectorLayer* inputLayer = LIPProject::getInstance().getVectorLayerByPath(
                ui->comboBoxInputLayer->itemData(ui->comboBoxInputLayer->currentIndex()).toString());
    if (inputLayer==nullptr)
        return;

    LIPVectorLayer* unionLayer = LIPProject::getInstance().getVectorLayerByPath(
                ui->comboBoxUnionLayer->itemData(ui->comboBoxUnionLayer->currentIndex()).toString());
    if (unionLayer==nullptr)
        return;
    if (!LIPVectorTypeChecker::isSameType(inputLayer, unionLayer))
        return;
    //if (!LIPVectorTypeChecker::isPolygonLayer(intersectLayer))
    //    return;

    auto coordinatesVect = LIPVectorUnion::getUnion(inputLayer, unionLayer);
    //LIPPointLayer *layer = inputLayer->top
    if (inputLayer->toPointLayer()!=nullptr)
    {
        std::shared_ptr<LIPCoordinateSystem> crs= std::make_shared<LIPCoordinateSystem>();
        crs->setOGRSpatialRef(inputLayer->getOGRLayer()->GetSpatialRef());
        LIPLayerCreator *lCr=new LIPLayerCreator(LIPGeometryType::LIPPoint, fileName, "outputLayer",
                                                 crs.get());
        outputLayer=lCr->returnLayer();
        for (auto pointFeatureCord : coordinatesVect)
        {
            for (auto point:pointFeatureCord)
            {
                QVector<QPointF> tempVec{point};

                outputLayer->addFeature(tempVec, QVector<LIPAttribute>());
            }
        }
        outputLayer->update();
    }

    else if (inputLayer->toLineLayer()!=nullptr)
    {
        std::shared_ptr<LIPCoordinateSystem> crs= std::make_shared<LIPCoordinateSystem>();
        crs->setOGRSpatialRef(inputLayer->getOGRLayer()->GetSpatialRef());
        LIPLayerCreator *lCr=new LIPLayerCreator(LIPGeometryType::LIPLineString, fileName, "outputLayer",
                                                 crs.get());
        outputLayer=lCr->returnLayer();
        for (auto lineFeatureCords : coordinatesVect)
        {
            outputLayer->addFeature(lineFeatureCords, QVector<LIPAttribute>());
        }
        outputLayer->update();
    }

    else if (inputLayer->toPolygonLayer()!=nullptr)
    {
        std::shared_ptr<LIPCoordinateSystem> crs= std::make_shared<LIPCoordinateSystem>();
        crs->setOGRSpatialRef(inputLayer->getOGRLayer()->GetSpatialRef());
        LIPLayerCreator *lCr=new LIPLayerCreator(LIPGeometryType::LIPPolygon, fileName, "outputLayer",
                                                 crs.get());
        outputLayer=lCr->returnLayer();
        for (auto polyFeatureCoord : coordinatesVect)
        {
            outputLayer->addFeature(polyFeatureCoord, QVector<LIPAttribute>());
        }
        outputLayer->update();
    }

    close();
}


void LIPVectorUnionForm::on_buttonBox_rejected()
{
    close();
}


void LIPVectorUnionForm::on_pushButtonFileDialog_clicked()
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

