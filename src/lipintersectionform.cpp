#include "lipintersectionform.h"
#include "ui_lipintersectionform.h"

LIPIntersectionForm::LIPIntersectionForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LIPIntersectionForm)
{
    ui->setupUi(this);
}

LIPIntersectionForm::~LIPIntersectionForm()
{
    delete ui;
}

void LIPIntersectionForm::on_pushButtonOk_clicked()
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
    if (!LIPVectorTypeChecker::isPolygonLayer(intersectLayer))
        return;


    close();
}


void LIPIntersectionForm::on_pushButtonFileDialog_clicked()
{
    fileName=QFileDialog::getSaveFileName(this, "", "");
    if (fileName.isEmpty())
        QMessageBox::warning(this,"Ошибка", "Указан неверный путь");
    else{
        ui->lineEditFileName->setText(fileName);
    }
}

