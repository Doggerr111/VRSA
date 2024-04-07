#include "lipnewlinelayerform.h"
#include "ui_lipnewlinelayerform.h"

LIPNewLineLayerForm::LIPNewLineLayerForm(QWidget *parent, LIPGeometryType geomType) :
    QDialog(parent),
    ui(new Ui::LIPNewLineLayerForm),
    LIPLayer{nullptr},
    geometryType{geomType}
{
    ui->setupUi(this);
    QComboBox *typeBox = new QComboBox();
    typeBox->addItem("Int32");
    typeBox->addItem("Real");
    typeBox->addItem("String");
    ui->tableWidget->setCellWidget(0,1,typeBox);
}

LIPNewLineLayerForm::~LIPNewLineLayerForm()
{
    delete ui;
}

LIPVectorLayer *LIPNewLineLayerForm::returnLayer()
{
    return LIPLayer;

}

OGRLayer *LIPNewLineLayerForm::returnOGRLayer()
{
    return layer;
}

void LIPNewLineLayerForm::on_pushButton_clicked()
{
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
    QComboBox *typeBox = new QComboBox();
    typeBox->addItem("Int32");
    typeBox->addItem("Real");
    typeBox->addItem("String");
    ui->tableWidget->setCellWidget(ui->tableWidget->rowCount()-1,1,typeBox);
}


void LIPNewLineLayerForm::on_pushButton_ok_clicked()
{
    //создание слоя

    QString name = ui->lineEdit_Name->text(); //toDO proverka na dlinu
    LIPCoordinateSystemLibrary *lib = LIPProject::getInstance().getCRSLibrary();
    LIPLayerCreator *newLayer = new LIPLayerCreator(geometryType, fileName, name,
                                                    lib->getCRSbyName(ui->comboBox->currentText()));
    //newLayer->setName(name);
    for (int i=0; i<ui->tableWidget->rowCount(); i++)
    {
        QComboBox *box =  qobject_cast< QComboBox* >(ui->tableWidget->cellWidget(i,1));
        if (box->currentIndex()==0)
            newLayer->createAttribute(LIPAttributeType::INT32, ui->tableWidget->item(i,0)->text());
        else if (box->currentIndex()==1)
            newLayer->createAttribute(LIPAttributeType::Real, ui->tableWidget->item(i,0)->text());
        else if (box->currentIndex()==2)
            newLayer->createAttribute(LIPAttributeType::String, ui->tableWidget->item(i,0)->text());
    }

    layer = newLayer->returnOGRLayer();


    LIPLayer = newLayer->returnLayer();
    //        switch (geometryType)
    //        {

    //            LIPLineLayer *l=dynamic_cast<LIPLineLayer*>(LIPLayer);
    //        }


    //LIPLayer->setFileName(fileName);

    delete newLayer;

//    }
//        break;
//    case LIPGeometryType::LIPLineString:
//    {
//        QString name = ui->lineEdit_Name->text(); //toDO proverka na dlinu
//        QString CRS = ui->comboBox->currentText();
//        LIPLayerCreator *newLayer = new LIPLayerCreator(LIPGeometryType::LIPLineString, fileName, name);
//        //newLayer->setName(name);
//        for (int i=0; i<ui->tableWidget->rowCount(); i++)
//        {
//            QComboBox *box =  qobject_cast< QComboBox* >(ui->tableWidget->cellWidget(i,1));
//            if (box->currentIndex()==0)
//                newLayer->createAttribute(LIPAttributeType::INT32, ui->tableWidget->item(i,0)->text());
//            else if (box->currentIndex()==1)
//                newLayer->createAttribute(LIPAttributeType::Real, ui->tableWidget->item(i,0)->text());
//            else if (box->currentIndex()==2)
//                newLayer->createAttribute(LIPAttributeType::String, ui->tableWidget->item(i,0)->text());
//        }
//        layer = newLayer->returnOGRLayer();
//        LIPLayer = newLayer->returnLayer();
//        delete newLayer;
//    }
//        break;
//    case LIPGeometryType::LIPPolygon:
//    {
//        QString name = ui->lineEdit_Name->text(); //toDO proverka na dlinu
//        QString CRS = ui->comboBox->currentText();
//        LIPLayerCreator *newLayer = new LIPLayerCreator(LIPGeometryType::LIPPolygon, fileName, name);
//        //newLayer->setName(name);
//        for (int i=0; i<ui->tableWidget->rowCount(); i++)
//        {
//            QComboBox *box =  qobject_cast< QComboBox* >(ui->tableWidget->cellWidget(i,1));
//            if (box->currentIndex()==0)
//                newLayer->createAttribute(LIPAttributeType::INT32, ui->tableWidget->item(i,0)->text());
//            else if (box->currentIndex()==1)
//                newLayer->createAttribute(LIPAttributeType::Real, ui->tableWidget->item(i,0)->text());
//            else if (box->currentIndex()==2)
//                newLayer->createAttribute(LIPAttributeType::String, ui->tableWidget->item(i,0)->text());
//        }
//        layer = newLayer->returnOGRLayer();
//        LIPLayer = newLayer->returnLayer();
//        delete newLayer;
//    }
//        break;
//    }
}


void LIPNewLineLayerForm::on_pushButton_2_clicked()
{
    fileName=QFileDialog::getSaveFileName(this,"","","*.shp") + ".shp";
    ui->lineEdit_fileName->setText(fileName);

}

