#include "liprasterstyledialog.h"
#include "ui_liprasterstyledialog.h"

LIPRasterStyleDialog::LIPRasterStyleDialog(LIPRasterLayer* l, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LIPRasterStyleDialog),
    layer{l},
    mStyle{nullptr}
{
    ui->setupUi(this);
    if (l==nullptr)
    {
        LIPWidgetManager::getInstance().showMessage("Для данного слоя невозможно настроить стиль", 1000, Error);
        delete this;
        return;
    }

    LIPRasterStyle *style = layer->getStyle();
    if (style==nullptr)
    {
        LIPWidgetManager::getInstance().showMessage("Для данного слоя невозможно настроить стиль", 1000, Error);
        delete this;
        return;
    }
    for (int i=0; i<layer->getBandCount(); i++)
    {
        ui->comboBoxBlue->addItem("Канал №"+QString::number(i+1));
        ui->comboBoxRed->addItem("Канал №"+QString::number(i+1));
        ui->comboBoxGreen->addItem("Канал №"+QString::number(i+1));
    }

    QIntValidator* intValidator = new QIntValidator(0, 65535, this);
    ui->lineEditBand1Max->setValidator(intValidator);
    ui->lineEditBand1Min->setValidator(intValidator);
    ui->lineEditBand2Max->setValidator(intValidator);
    ui->lineEditBand2Min->setValidator(intValidator);
    ui->lineEditBand3Max->setValidator(intValidator);
    ui->lineEditBand3Min->setValidator(intValidator);

    mStyle = dynamic_cast<LIPRasterRGBStyle*>(style);
    ui->lineEditBand1Max->setText(QString::number(mStyle->maxPixelValue_band1));
    ui->lineEditBand1Min->setText(QString::number(mStyle->minPixelValue_band1));

    ui->lineEditBand2Max->setText(QString::number(mStyle->maxPixelValue_band2));
    ui->lineEditBand2Min->setText(QString::number(mStyle->minPixelValue_band2));

    ui->lineEditBand3Max->setText(QString::number(mStyle->maxPixelValue_band3));
    ui->lineEditBand3Min->setText(QString::number(mStyle->minPixelValue_band3));

    ui->comboBoxRed->setCurrentIndex(mStyle->bandIndex1-1);
    ui->comboBoxGreen->setCurrentIndex(mStyle->bandIndex2-1);
    ui->comboBoxBlue->setCurrentIndex(mStyle->bandIndex3-1);


}

LIPRasterStyleDialog::~LIPRasterStyleDialog()
{
    delete ui;
}

void LIPRasterStyleDialog::on_buttonBox_accepted()
{
    mStyle->bandIndex1=ui->comboBoxRed->currentIndex()+1;
    mStyle->bandIndex2=ui->comboBoxGreen->currentIndex()+1;
    mStyle->bandIndex3=ui->comboBoxBlue->currentIndex()+1;

    mStyle->maxPixelValue_band1=ui->lineEditBand1Max->text().toInt();
    mStyle->minPixelValue_band1=ui->lineEditBand1Min->text().toInt();

    mStyle->maxPixelValue_band2=ui->lineEditBand2Max->text().toInt();
    mStyle->minPixelValue_band2=ui->lineEditBand2Min->text().toInt();

    mStyle->maxPixelValue_band3=ui->lineEditBand3Max->text().toInt();
    mStyle->minPixelValue_band3=ui->lineEditBand3Min->text().toInt();
    layer->update();
}

