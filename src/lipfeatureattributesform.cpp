#include "lipfeatureattributesform.h"
#include "ui_lipfeatureattributesform.h"

LIPFeatureAttributesForm::LIPFeatureAttributesForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LIPFeatureAttributesForm),
    mLayer{nullptr},
    mIndex{-1}
{
    ui->setupUi(this);
}

LIPFeatureAttributesForm::~LIPFeatureAttributesForm()
{
    delete ui;
}

void LIPFeatureAttributesForm::setFeature(OGRLayer *layer, int featureIndex)
{
    mLayer = layer;
    mIndex = featureIndex;
    layer->ResetReading(); // Сбросить указатель на начало чтения

    for (int i = 0; i <= mIndex; ++i)
    {
        auto shpFeature = layer->GetNextFeature();
        if (i == mIndex)
        {
            OGRFeatureDefn *poFDefn = layer->GetLayerDefn();
            int iField;
            for (iField = 0; iField < poFDefn->GetFieldCount(); iField++)
            {
                OGRFieldDefn *poFieldDefn = poFDefn->GetFieldDefn(iField);
                poFieldDefn->GetNameRef();
                ui->tableWidget->insertRow(iField);
                QTableWidgetItem *newItem = new QTableWidgetItem(poFieldDefn->GetNameRef());
                ui->tableWidget->setVerticalHeaderItem(iField, newItem);

                QTableWidgetItem *value = new QTableWidgetItem;
                if (poFieldDefn->GetType() == OFTInteger)
                    value->setText(QString::number(shpFeature->GetFieldAsInteger(iField)));
                else if (poFieldDefn->GetType() == OFTInteger64)
                    value->setText(QString::number( shpFeature->GetFieldAsInteger64(iField)));
                else if (poFieldDefn->GetType() == OFTReal)
                    value->setText(QString::number(shpFeature->GetFieldAsDouble(iField)));
                else if (poFieldDefn->GetType() == OFTString)
                    value->setText(QString(shpFeature->GetFieldAsString(iField)));
                else
                    value->setText(QString(shpFeature->GetFieldAsString(iField)));
                ui->tableWidget->setItem(iField, 0, value);
            }
        }
        OGRFeature::DestroyFeature(shpFeature);
    }

}
