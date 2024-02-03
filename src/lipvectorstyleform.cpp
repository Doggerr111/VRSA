#include "lipvectorstyleform.h"
#include "ui_lipvectorstyleform.h"

LIPVectorStyleForm::LIPVectorStyleForm(QWidget *parent, LIPVectorLayer *layer, LIPVectorStyle *style) :
    QDialog(parent),
    pointItem{nullptr},
    polyItem{nullptr},
    lineItem{nullptr},
    mStyle{style},    
    mLayer{layer},
    ui(new Ui::LIPVectorStyleForm)
{
    ui->setupUi(this);
    mScene = new QGraphicsScene;
    ui->graphicsView->setScene(mScene);
    mScene->setSceneRect(0,0,298,198);
//    pointItem = new LIPPointGraphicsItem;
//    LIPPoint* pointCenter = new LIPPoint;
//    pointCenter->setX(25);
//    pointCenter->setY(25);
//    pointItem->setPoint(pointCenter);
//    pointItem->setPos(scene->sceneRect().center());
    if (mStyle==nullptr)
        mStyle=new LIPVectorStyle;
    if (mLayer!=nullptr)
    {
        startRenderFeature();
    }



}

LIPVectorStyleForm::~LIPVectorStyleForm()
{
    delete ui;
}

LIPVectorStyle* LIPVectorStyleForm::getStyle()
{
    return mStyle;
}

void LIPVectorStyleForm::startRenderFeature()
{
    mStyle=mLayer->getStyle();

    switch (mStyle->GetGeomType())
    {
    case LIPGeometryType::LIPPoint:
    {
        ui->stackedWidget->setCurrentIndex(0);
        pointItem = new LIPPointGraphicsItem;
        LIPPoint* pointCenter = new LIPPoint;
        pointCenter->setX(0);
        pointCenter->setY(0);
        pointItem->setPoint(pointCenter);
        pointItem->setVectorStyle(mStyle);
        pointItem->setPos(mScene->sceneRect().center());
        mScene->addItem(pointItem);
        //delete mStyle;
        break;
    }
    case LIPGeometryType::LIPLineString:
    {
        ui->stackedWidget->setCurrentIndex(1);
        lineItem = new LIPLineGraphicsItem;
        QVector<LIPPoint*> vec;
        LIPPoint* point1 = new LIPPoint;
        point1->setX(129);
        point1->setY(79);
        LIPPoint* point2 = new LIPPoint;
        point2->setX(171);
        point2->setY(121);
        vec.append(point1);
        vec.append(point2);
        lineItem->setPoints(vec);
        lineItem->setVectorStyle(mStyle);
        lineItem->setPos(0,0);
        mScene->addItem(lineItem);
        break;
    }
    case LIPGeometryType::LIPPolygon:
    {
        ui->stackedWidget->setCurrentIndex(2);
        polyItem = new LIPPolygonGraphicsItem;
        QVector<LIPPoint*> vec;
        LIPPoint* point1 = new LIPPoint;
        point1->setX(129);
        point1->setY(79);
        LIPPoint* point2 = new LIPPoint;
        point2->setX(171);
        point2->setY(79);
        LIPPoint* point3 = new LIPPoint;
        point3->setX(171);
        point3->setY(121);
        LIPPoint* point4 = new LIPPoint;
        point4->setX(129);
        point4->setY(121);
        vec.append(point1);
        vec.append(point2);
        vec.append(point3);
        vec.append(point4);
        polyItem->setPoints(vec);
        polyItem->setVectorStyle(mStyle);
        polyItem->setPos(0,0);
        mScene->addItem(polyItem);
        break;
    }

    }
}

void LIPVectorStyleForm::on_pushButtonCancel_clicked()
{

}

void LIPVectorStyleForm::on_pushButtonOk_clicked()
{


}

//point styling interface

void LIPVectorStyleForm::on_pushButtonPointColor_clicked()
{
    QColorDialog pointColorDialog; //= new QColorDialog(this);
    //pointColorDialog->exec();
    QColor pointColor = pointColorDialog.getColor();
    if (pointColor.isValid())
    {
        QPen pen = mStyle->getPen();
        pen.setColor(pointColor);
        mStyle->setPen(pen);
        ui->pushButtonPointColor->setStyleSheet("background-color: " + pointColor.name() + ";");
    }
    pointItem->setVectorStyle(mStyle);
    pointItem->update();
    //item->setPen(pen);
    //item->setPos(25,25);



}

void LIPVectorStyleForm::on_lineEditPointSize_textChanged(const QString &arg1)
{
    bool typeFlag=false;
    double width=arg1.toDouble(&typeFlag);
    if (typeFlag)
    {
        if (mStyle!=nullptr)
        {
            mStyle->setPointSize(width);
            pointItem->setVectorStyle(mStyle);
        }
        pointItem->update();
        ui->graphicsView->scene()->update();
        //ui->graphicsView->scene()->addItem(pointItem);
    }
}

void LIPVectorStyleForm::on_lineEditPointSize_textEdited(const QString &arg1)
{

}




// line styling interface

void LIPVectorStyleForm::on_lineEditLineWidth_textChanged(const QString &arg1)
{
    bool typeFlag=false;
    double width=arg1.toDouble(&typeFlag);
    if (typeFlag)
    {
        if (mStyle!=nullptr)
        {
            QPen pen=mStyle->getPen();
            pen.setWidthF(width);
            mStyle->setPen(pen);
            lineItem->setVectorStyle(mStyle);
        }
        lineItem->update();
        ui->graphicsView->scene()->update();
        //ui->graphicsView->scene()->addItem(pointItem);
    }
}


void LIPVectorStyleForm::on_pushButtonLineColor_clicked()
{
    QColorDialog pointColorDialog; //= new QColorDialog(this);
    //pointColorDialog->exec();
    QColor pointColor = pointColorDialog.getColor();
    if (pointColor.isValid())
    {
        QPen pen;
        pen=mStyle->getPen();
        pen.setColor(pointColor);
        mStyle->setPen(pen);
        ui->pushButtonLineColor->setStyleSheet("background-color: " + pointColor.name() + ";");
    }

    lineItem->setVectorStyle(mStyle);
    lineItem->update();
}

