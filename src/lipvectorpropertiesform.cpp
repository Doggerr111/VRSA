#include "lipvectorpropertiesform.h"
#include "ui_lipvectorpropertiesform.h"
#include <QDebug>
LIPVectorPropertiesForm::LIPVectorPropertiesForm(LIPVectorLayer *layer, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LIPVectorPropertiesForm)
{
    ui->setupUi(this);
    if (layer==nullptr)
    {
        delete this;
        return;
    }

    ui->labelProperty1->setText(layer->returnGISName());
    ui->labelProperty2->setText(layer->returnFileName());
    //qDebug()<<layer->getCRS();
    if (layer->getCRS()!=nullptr)
    {
        char *pszWKT = nullptr;
        layer->getCRS()->getOGRSpatialRef()->exportToWkt(&pszWKT);
        ui->labelProperty3->setText(pszWKT);
    }
    else
    {
        ui->labelProperty3->setText("Система координат не известна");
    }

}

LIPVectorPropertiesForm::~LIPVectorPropertiesForm()
{
    delete ui;
}
