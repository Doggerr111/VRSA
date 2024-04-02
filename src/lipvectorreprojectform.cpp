#include "lipvectorreprojectform.h"
#include "ui_lipvectorreprojectform.h"

LIPVectorReprojectForm::LIPVectorReprojectForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LIPVectorReprojectForm)
{
    ui->setupUi(this);
    setWindowTitle("Перепроецирование");
}

LIPVectorReprojectForm::~LIPVectorReprojectForm()
{
    delete ui;
}

void LIPVectorReprojectForm::on_buttonBox_accepted()
{
    LIPVectorLayer* inputLayer = ui->comboBoxLayer->getVectorLayer();
    if (inputLayer==nullptr)
    {
        LIPWidgetManager::getInstance().showMessage("Данный слой нельзя перепроецировать", 1000, Error);
        return;
    }
    LIPCoordinateSystem* src = ui->comboBoxCRS->getCurrentCRS();
    if (src==nullptr)
    {
        LIPWidgetManager::getInstance().showMessage("Перепроецирование невозможно", 1000,  Error);
        return;
    }
    //inputLayer->reproject(src);
    LIPLayerCreator::createReprojectedLayer(inputLayer, src, fileName);
}


void LIPVectorReprojectForm::on_pushButtonFileDialog_clicked()
{
    fileName=QFileDialog::getSaveFileName(this, "Выберите путь для сохранения результата", LIPProject::getInstance().getVectorDataFolder(),
                                          "Shape files (*.shp)");
    if (fileName.isEmpty())
    {
        QMessageBox::warning(this,"Ошибка", "Указан неверный путь");
        return;
    }

    if (!fileName.endsWith(".shp"))
        fileName.append(".shp");
    ui->lineEditFileName->setText(fileName);
}

