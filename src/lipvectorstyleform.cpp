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
    ui->graphicsView->scale(1,-1);
    mScene->setSceneRect(0,0,298,198);
    ui->frameTableWidgetCustomDashLIne->hide();

//    pointItem = new LIPPointGraphicsItem;
//    LIPPoint* pointCenter = new LIPPoint;
//    pointCenter->setX(25);
//    pointCenter->setY(25);
//    pointItem->setPoint(pointCenter);
//    pointItem->setPos(scene->sceneRect().center());
    if (mStyle==nullptr)
        mStyle=new LIPVectorStyle;

    LIPVectorStyle style1 = *mStyle;
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
        ui->lineEditPointSize->setText(QString::number(mStyle->getPointSize()));
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
    default:
        break;

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
    QColorDialog *dial = new QColorDialog(this);
    dial->setOption(QColorDialog::ShowAlphaChannel);
    dial->exec();
    //pointColorDialog->exec();
    QColor pointColor = dial->selectedColor();
    if (pointColor.isValid())
    {
        QPen pen = mStyle->getPen();
        pen.setColor(pointColor);
        mStyle->setPen(pen);
        ui->pushButtonPointColor->setStyleSheet("background-color: " + pointColor.name() + ";");
    }
    //pointItem->setVectorStyle(mStyle);
    delete dial;
    pointItem->update();
    ui->graphicsView->scene()->update();



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
    Q_UNUSED(arg1)
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





void LIPVectorStyleForm::on_pushButtonSquareSymbol_clicked()
{
    mStyle->setPointType(Square);
    pointItem->update();
    ui->graphicsView->scene()->update();
}


void LIPVectorStyleForm::on_pushButtonCircleSymbol_clicked()
{
    mStyle->setPointType(Circle);
    pointItem->update();
    ui->graphicsView->scene()->update();
}


void LIPVectorStyleForm::on_pushButtonTriangleSymbol_clicked()
{
    mStyle->setPointType(Triangle);
    pointItem->update();
    ui->graphicsView->scene()->update();
}


void LIPVectorStyleForm::on_pushButtonRhombusSymbol_clicked()
{
    mStyle->setPointType(Rhombus);
    pointItem->update();
    ui->graphicsView->scene()->update();
}


void LIPVectorStyleForm::on_pushButtonPointBrushColor_clicked()
{
    //QColorDialog pointColorDialog; //= new QColorDialog(this);
    QColorDialog *dial = new QColorDialog(this);
    dial->setOption(QColorDialog::ShowAlphaChannel);
    dial->exec();
    //pointColorDialog->exec();
    QColor pointColor = dial->selectedColor();
    if (pointColor.isValid())
    {
        QBrush brush = mStyle->getBrush();
        brush.setColor(pointColor);
        mStyle->setBrush(brush);
        ui->pushButtonPointColor->setStyleSheet("background-color: " + pointColor.name() + ";");
    }
    //pointItem->setVectorStyle(mStyle);
    delete dial;
    pointItem->update();
    ui->graphicsView->scene()->update();

}


void LIPVectorStyleForm::on_pushButtonInvTriangleSymbol_clicked()
{
    mStyle->setPointType(InvertedTriangle);
    pointItem->update();
    ui->graphicsView->scene()->update();
}


void LIPVectorStyleForm::on_pushButtonLoadImage_clicked()
{
    mStyle->setPointType(CustomImage);
    QString fileName=QFileDialog::getOpenFileName(this, "", "", "Image files (*.jpg *.png)");
    if (!fileName.isEmpty())
        mStyle->setCustomImage(new QImage(fileName));
    pointItem->update();
    ui->graphicsView->scene()->update();


}


void LIPVectorStyleForm::on_comboBoxPointBrushType_currentIndexChanged(int index)
{
//    QBrush curBrush = mStyle->getBrush();
//    curBrush.setStyle(ui->comboBoxPointBrushType->getStyle());
//    mStyle->setBrush(curBrush);

//    pointItem->update();
//    ui->graphicsView->scene()->update();
    Q_UNUSED(index);
}


void LIPVectorStyleForm::on_comboBox_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    QBrush curBrush = mStyle->getBrush();
    curBrush.setStyle(ui->comboBox->getStyle());
    mStyle->setBrush(curBrush);

    pointItem->update();
    ui->graphicsView->scene()->update();
}


void LIPVectorStyleForm::on_lineEditPointPenWidth_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
//    QPen pen = mStyle->getPen();
//    pen.setWidthF(arg1.toDouble());
//    mStyle->setPen(pen);
//    pointItem->update();
//    ui->graphicsView->scene()->update();
}


//void LIPVectorStyleForm::on_comboBox_2_currentIndexChanged(int index)
//{
//    QPen pen = mStyle->getPen();
//    pen.setStyle(Qt::DashDotLine);
//    mStyle->setPen(pen);
//}





void LIPVectorStyleForm::on_comboBoxLinePenStyle_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    QPen pen = mStyle->getPen();
    pen.setStyle(ui->comboBoxLinePenStyle->getStyle());

    mStyle->setPen(pen);
    lineItem->update();
    ui->graphicsView->scene()->update();
}


void LIPVectorStyleForm::on_comboBoxLineCapStyle_currentIndexChanged(int index)
{
    QPen pen = mStyle->getPen();
    switch (index)
    {
    case 0:
        pen.setCapStyle(Qt::SquareCap);
        break;
    case 1:
        pen.setCapStyle(Qt::FlatCap);
        break;
    case 2:
        pen.setCapStyle(Qt::RoundCap);
        break;
    }

    mStyle->setPen(pen);
    lineItem->update();
    ui->graphicsView->scene()->update();
}


void LIPVectorStyleForm::on_comboBoxLineJoinStyle_currentIndexChanged(int index)
{
    QPen pen = mStyle->getPen();
    switch (index)
    {
    case 0:
        pen.setJoinStyle(Qt::BevelJoin);
        break;
    case 1:
        pen.setJoinStyle(Qt::MiterJoin);
        break;
    case 2:
        pen.setJoinStyle(Qt::RoundJoin);
        break;
    }

    mStyle->setPen(pen);
    lineItem->update();
    ui->graphicsView->scene()->update();
}


void LIPVectorStyleForm::on_checkBox_clicked(bool checked)
{
    if (checked)
        ui->frameTableWidgetCustomDashLIne->show();
    else
        ui->frameTableWidgetCustomDashLIne->hide();
}


void LIPVectorStyleForm::on_pushButtonAddRow_clicked()
{
    ui->tableWidgetCustomDashLine->insertRow(ui->tableWidgetCustomDashLine->rowCount());
}


void LIPVectorStyleForm::on_pushButtonDeleteRow_clicked()
{
    //ui->tableWidgetCustomDashLine->insertRow(ui->tableWidgetCustomDashLine->rowCount());
    int rowCount=ui->tableWidgetCustomDashLine->rowCount();
    if (rowCount==0)
        return;
    ui->tableWidgetCustomDashLine->removeRow(rowCount-1);

}


void LIPVectorStyleForm::on_pushButtonAplyCustomDashLine_clicked()
{
    QVector<qreal> dashes;
    for (int i=0; i<ui->tableWidgetCustomDashLine->rowCount(); i++)
    {
        float dash = 0.0;
        float space = 0.0;
        auto dashTable = ui->tableWidgetCustomDashLine->item(i,0);
        auto spaceTable = ui->tableWidgetCustomDashLine->item(i,1);
        if (dashTable && !dashTable->text().isEmpty() && !dashTable->text().isNull())
        {
            bool ok=false;
            dash=LIPVectorStyle::MMToPixel(dashTable->text().toFloat(&ok));
            if (!ok)
                dash=0.0;
        }
        if (spaceTable && !spaceTable->text().isEmpty() && !spaceTable->text().isNull())
        {
            bool ok=false;
            space=LIPVectorStyle::MMToPixel(spaceTable->text().toFloat(&ok));
            if (!ok)
                space=0.0;
        }
        dashes<<dash/LIPVectorStyle::MMToPixel(pen.widthF())
             <<space/LIPVectorStyle::MMToPixel(pen.widthF());
    }
    QPen pen = mStyle->getPen();
    pen.setStyle(Qt::CustomDashLine);
    pen.setDashPattern(dashes);

    mStyle->setPen(pen);
    lineItem->update();
    ui->graphicsView->scene()->update();

}

//poly styling

void LIPVectorStyleForm::on_lineEditPolyBorderWidth_textChanged(const QString &arg1)
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
            polyItem->setVectorStyle(mStyle);
        }
        polyItem->update();
        ui->graphicsView->scene()->update();
        //ui->graphicsView->scene()->addItem(pointItem);
    }
}


void LIPVectorStyleForm::on_pushButtonPolyBorderColor_clicked()
{
    QColorDialog *dial = new QColorDialog(this);
    dial->setOption(QColorDialog::ShowAlphaChannel);
    dial->exec();
    //pointColorDialog->exec();
    QColor pointColor = dial->selectedColor();
    if (pointColor.isValid())
    {
        QPen pen = mStyle->getPen();
        pen.setColor(pointColor);
        mStyle->setPen(pen);
        ui->pushButtonPolyBorderColor->setStyleSheet("background-color: " + pointColor.name() + ";");
    }
    //pointItem->setVectorStyle(mStyle);
    delete dial;
    polyItem->update();
    ui->graphicsView->scene()->update();
}


void LIPVectorStyleForm::on_comboBoxPolyBorderStyle_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    QPen pen = mStyle->getPen();
    pen.setStyle(ui->comboBoxPolyBorderStyle->getStyle());
    mStyle->setPen(pen);
    polyItem->update();
    ui->graphicsView->scene()->update();
}


void LIPVectorStyleForm::on_pushButtonPolygonBrushColor_clicked()
{
    QColorDialog *dial = new QColorDialog(this);
    dial->setOption(QColorDialog::ShowAlphaChannel);
    dial->exec();
    //pointColorDialog->exec();
    QColor pointColor = dial->selectedColor();
    if (pointColor.isValid())
    {
        QBrush brush = mStyle->getBrush();
        brush.setColor(pointColor);
        mStyle->setBrush(brush);
        ui->pushButtonPolygonBrushColor->setStyleSheet("background-color: " + pointColor.name() + ";");
    }
    //pointItem->setVectorStyle(mStyle);
    delete dial;
    polyItem->update();
    ui->graphicsView->scene()->update();

}


void LIPVectorStyleForm::on_comboBoxPolyBrushStyle_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    QBrush brush = mStyle->getBrush();
    brush.setStyle(ui->comboBoxPolyBrushStyle->getStyle());
    mStyle->setBrush(brush);
    polyItem->update();
    ui->graphicsView->scene()->update();
}


void LIPVectorStyleForm::on_comboBoxPolyBorderJoinStyle_currentIndexChanged(int index)
{
    QPen pen = mStyle->getPen();
    switch (index)
    {
    case 0:
        pen.setJoinStyle(Qt::BevelJoin);
        break;
    case 1:
        pen.setJoinStyle(Qt::MiterJoin);
        break;
    case 2:
        pen.setJoinStyle(Qt::RoundJoin);
        break;
    }

    mStyle->setPen(pen);
    polyItem->update();
    ui->graphicsView->scene()->update();
}

