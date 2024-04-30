#include "lipattributestableform.h"
#include "ui_lipattributestableform.h"

LIPAttributesTableForm::LIPAttributesTableForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LIPAttributesTableForm),
    layer{nullptr}
{
    ui->setupUi(this);

}

LIPAttributesTableForm::~LIPAttributesTableForm()
{
    delete ui;
}

void LIPAttributesTableForm::setLayer(LIPVectorLayer *l)
{
    if (l!=nullptr)
    {
        ui->tableWidget->setLayer(l);
        layer=l;
        layer->deselectItems();
    }

}

void LIPAttributesTableForm::on_tableWidget_itemSelectionChanged()
{
    QList<QTableWidgetItem*> selectedItems = ui->tableWidget->selectedItems();
    layer->deselectItems();
    for (int i=0; i<selectedItems.count(); i++)
    {
        layer->selectFeature(selectedItems.at(i)->row());
    }
}



void LIPAttributesTableForm::closeEvent(QCloseEvent *event)
{
    if (layer)
        layer->deselectItems();
}
