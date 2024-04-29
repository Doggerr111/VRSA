#include "lipreprojectrasterlayerform.h"
#include "ui_lipreprojectrasterlayerform.h"

LIPReprojectRasterLayerForm::LIPReprojectRasterLayerForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LIPReprojectRasterLayerForm)
{
    ui->setupUi(this);
    setWindowTitle("Перепроецирование растра");
}

LIPReprojectRasterLayerForm::~LIPReprojectRasterLayerForm()
{
    delete ui;
}

void LIPReprojectRasterLayerForm::on_buttonBox_accepted()
{
    if (ui->comboBoxRaster->getRasterLayer()==nullptr)
        return;
    if (ui->comboBoxCRS->getCurrentCRS()==nullptr)
        return;

    if (LIPRasterTransform::reproject(ui->comboBoxRaster->getRasterLayer(), ui->comboBoxCRS->getCurrentCRS(), fileName))
        new LIPRasterLayer(fileName);
    else
        LIPWidgetManager::getInstance().showMessage("Ошибка перепроецирования", 1000, Error);
    close();
}


void LIPReprojectRasterLayerForm::on_pushButtonPath_clicked()
{
    fileName=QFileDialog::getSaveFileName(this,"","","*.tif");
    if (fileName.endsWith(".tif"))
        ui->lineEditPath->setText(fileName);
    else
    {
        ui->lineEditPath->setText(fileName+".tif");
    }
}

