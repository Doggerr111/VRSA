#include "lipvectorassignprojectform.h"
#include "ui_lipvectorassignprojectform.h"

LIPVectorAssignProjectForm::LIPVectorAssignProjectForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LIPVectorAssignProjectForm)
{
    ui->setupUi(this);
    setWindowTitle("Назначить проекцию");
}

LIPVectorAssignProjectForm::~LIPVectorAssignProjectForm()
{
    delete ui;
}

void LIPVectorAssignProjectForm::on_buttonBox_accepted()
{
    LIPVectorLayer* inputLayer = ui->comboBoxLayer->getVectorLayer();
    if (inputLayer==nullptr)
    {
        LIPWidgetManager::getInstance().showMessage("Для данного слоя нельзя назначить проекцию", 1000, Error);
        return;
    }
    if (inputLayer->getCRS()!=nullptr)
    {
        LIPWidgetManager::getInstance().showMessage("Для данного слоя уже назначена проекция. Для изменения проекции воспользуйтесь"
                                                    "инструментом перепроецирования", 2000, Error);
        return;
    }
    LIPCoordinateSystem* crs = ui->comboBoxCRS->getCurrentCRS();
    if (crs==nullptr)
    {
        LIPWidgetManager::getInstance().showMessage("Перепроецирование невозможно", 1000,  Error);
        return;
    }
    //inputLayer->reproject(src);
    inputLayer->setCRS(crs);
}

