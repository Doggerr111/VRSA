#include "lipintersectionform.h"
#include "ui_lipintersectionform.h"

LIPIntersectionForm::LIPIntersectionForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LIPIntersectionForm)
{
    ui->setupUi(this);
}

LIPIntersectionForm::~LIPIntersectionForm()
{
    delete ui;
}

void LIPIntersectionForm::on_pushButtonOk_clicked()
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

    LIPVectorLayer* intersectLayer = LIPProject::getInstance().getVectorLayerByPath(
                ui->comboBoxIntersectLayer->itemData(ui->comboBoxIntersectLayer->currentIndex()).toString());
    if (intersectLayer==nullptr)
        return;
    //if (!LIPVectorTypeChecker::isPolygonLayer(intersectLayer))
    //    return;
    auto polyLayer = intersectLayer->toPolygonLayer();
    if (polyLayer==nullptr)
        return;
    auto coordinatesVect = LIPVectorIntersection::getIntersection(inputLayer, polyLayer);
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

    else if (inputLayer->toPointLayer()!=nullptr)
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


void LIPIntersectionForm::on_pushButtonFileDialog_clicked()
{
    fileName=QFileDialog::getSaveFileName(this, "", "");
    if (fileName.isEmpty())
        QMessageBox::warning(this,"Ошибка", "Указан неверный путь");
    else{
        ui->lineEditFileName->setText(fileName);
    }
}

