#include "lipcutrasterlayerform.h"
#include "ui_lipcutrasterlayerform.h"

LIPCutRasterLayerForm::LIPCutRasterLayerForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LIPCutRasterLayerForm)
{
    ui->setupUi(this);
    setWindowTitle("Обрезка растра");
}

LIPCutRasterLayerForm::~LIPCutRasterLayerForm()
{
    delete ui;
}

void LIPCutRasterLayerForm::on_buttonBox_accepted()
{
    if (fileName.isEmpty())
    {
        QMessageBox::warning(this, "Ошибка", "Для работы инструмента необходимо указать путь");
        return;
    }
    LIPPolygonLayer *mask = dynamic_cast<LIPPolygonLayer*>(ui->comboBoxVector->getVectorLayer());
    if (mask == nullptr)
    {
        QMessageBox::warning(this, "Ошибка", "Некорректный векторный слой");
        return;
    }
    LIPRasterLayer* layer = ui->comboBoxRaster->getRasterLayer();
    if (layer==nullptr)
    {
        QMessageBox::warning(this, "Ошибка", "Выбран некорректный растровый слой");
        return;
    }
    if (LIPCutRasterLayer::cutRasterByVector(ui->comboBoxRaster->getRasterLayer(), mask, fileName))
    {
        new LIPRasterLayer(fileName);
        close();
    }
    else
    {
        QMessageBox::warning(this, "Ошибка", "Ошибка обрезки растра");
    }
}


void LIPCutRasterLayerForm::on_pushButtonPath_clicked()
{
    fileName=QFileDialog::getSaveFileName(this,"","","*.tif");
    if (fileName.endsWith(".tif"))
        ui->lineEditPath->setText(fileName);
    else
    {
        fileName += ".tif";
        ui->lineEditPath->setText(fileName);
    }

}

