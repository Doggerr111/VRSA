#include "lipnewpointlayerform.h"
#include "ui_lipnewpointlayerform.h"

LIPNewPointLayerForm::LIPNewPointLayerForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LIPNewPointLayerForm)
{
    ui->setupUi(this);
}

LIPNewPointLayerForm::~LIPNewPointLayerForm()
{
    delete ui;
}
