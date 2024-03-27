#include "liprastercontoursform.h"
#include "ui_liprastercontoursform.h"

LIPRasterContoursForm::LIPRasterContoursForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LIPRasterContoursForm)
{
    ui->setupUi(this);
    const auto layer = ui->comboBoxRaster->getRasterLayer();
    if (layer)
    {
        for (int i=0; i<layer->getBandCount(); i++)
        {
            ui->comboBoxRasterBands->addItem("Канал №"+QString::number(i+1));
        }
    }
}

LIPRasterContoursForm::~LIPRasterContoursForm()
{
    delete ui;
}

void LIPRasterContoursForm::on_buttonBox_accepted()
{
    LIPRasterLayer* inputLayer=ui->comboBoxRaster->getRasterLayer();
    if (!inputLayer)
        return;
    int bandIndex = ui->comboBoxRasterBands->currentIndex()+1;
    double interval = ui->spinBoxInterval->value();
    double base = ui->spinBoxBase->value();
    LIPRasterProcessing::createContours(inputLayer, bandIndex, interval, base, fileName);


}


void LIPRasterContoursForm::on_pushButtonPath_clicked()
{
    fileName=QFileDialog::getSaveFileName(this,"","","*.shp");
    if (fileName.endsWith(".shp"))
        ui->lineEditPath->setText(fileName);
    else
    {
        ui->lineEditPath->setText(fileName+".shp");
    }
}


void LIPRasterContoursForm::on_comboBoxRaster_currentIndexChanged(int index)
{
    Q_UNUSED(index)
    const auto lay = ui->comboBoxRaster->getRasterLayer();
    if (!lay)
        return;
    for (int i=0; i<lay->getBandCount(); i++)
    {
        ui->comboBoxRasterBands->addItem("Канал №"+QString::number(i+1));
    }
}

