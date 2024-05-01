#include "lipintersectionform.h"
#include "ui_lipintersectionform.h"

LIPIntersectionForm::LIPIntersectionForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LIPIntersectionForm)
{
    ui->setupUi(this);
    setWindowTitle("Пересечение");
}

LIPIntersectionForm::~LIPIntersectionForm()
{
    delete ui;
}




void LIPIntersectionForm::on_pushButtonFileDialog_clicked()
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


void LIPIntersectionForm::on_buttonBox_accepted()
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
    if (inputLayer==intersectLayer)
    {
        QMessageBox::warning(this,"Ошибка", "Указаны 2 одинаковых слоя");
        return;
    }
    //if (!LIPVectorTypeChecker::isPolygonLayer(intersectLayer))
    //    return;
    auto polyLayer = intersectLayer->toPolygonLayer();
    if (polyLayer==nullptr)
    {
        QMessageBox::warning(this,"Ошибка", "Слой наложения должен быть полигональным!");
        return;
    }
    auto coordinatesVect = LIPVectorIntersection::getIntersection(inputLayer, polyLayer);
    //LIPPointLayer *layer = inputLayer->top
    if (inputLayer->toPointLayer()!=nullptr)
    {

        LIPLayerCreator *lCr=new LIPLayerCreator(LIPGeometryType::LIPPoint, fileName, "outputLayer",
                                                 inputLayer->getCRS());
        outputLayer=lCr->returnLayer();
        for (auto pointFeatureCord : coordinatesVect)
        {
            for (auto point:pointFeatureCord)
            {
                QVector<QPointF> tempVec{point};

                outputLayer->addFeature(tempVec, QVector<LIPAttribute>());
            }
        }
        delete lCr;
        outputLayer->update();
    }

    else if (inputLayer->toLineLayer()!=nullptr)
    {
        LIPLayerCreator *lCr=new LIPLayerCreator(LIPGeometryType::LIPLineString, fileName, "outputLayer",
                                                 inputLayer->getCRS());
        outputLayer=lCr->returnLayer();
        for (auto lineFeatureCords : coordinatesVect)
        {
            outputLayer->addFeature(lineFeatureCords, QVector<LIPAttribute>());
        }
        delete lCr;
        outputLayer->update();
    }

    else if (inputLayer->toPolygonLayer()!=nullptr)
    {
        LIPLayerCreator *lCr=new LIPLayerCreator(LIPGeometryType::LIPPolygon, fileName, "outputLayer",
                                                 inputLayer->getCRS());
        outputLayer=lCr->returnLayer();
        for (auto polyFeatureCoord : coordinatesVect)
        {
            outputLayer->addFeature(polyFeatureCoord, QVector<LIPAttribute>());
        }
        delete lCr;
        outputLayer->update();
    }

    close();
}


void LIPIntersectionForm::on_buttonBox_rejected()
{
    close();
}

