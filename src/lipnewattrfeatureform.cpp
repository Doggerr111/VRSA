#include "lipnewattrfeatureform.h"
#include "ui_lipnewattrfeatureform.h"

LIPNewAttrFeatureForm::LIPNewAttrFeatureForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LIPNewAttrFeatureForm)
{
    ui->setupUi(this);
}

LIPNewAttrFeatureForm::~LIPNewAttrFeatureForm()
{
    delete ui;
}

void LIPNewAttrFeatureForm::setAttributeNames(QVector<QString> names)
{
    attributeNames=names;
    QVBoxLayout* layout = new QVBoxLayout(this); // Создание вертикального макета
    for (int i=0; i<names.size(); i++)
    {
        QLabel* label = new QLabel(names.at(i), this);
        QLineEdit* lineEdit = new QLineEdit(this);
        layout->addWidget(label);
        layout->addWidget(lineEdit);
        lineEdits.append(lineEdit);
    }


    // Установка выравнивания для макета
    layout->setAlignment(Qt::AlignTop);

    setLayout(layout);

}

void LIPNewAttrFeatureForm::setAttributeTypes(QVector<LIPAttributeType> types)
{
    attributeTypes=types;
}

QVector<QString> LIPNewAttrFeatureForm::getValues()
{
    QVector<QString> values;
    for (int i=0; i<lineEdits.size(); i++)
    {
        values.append(lineEdits.at(i)->text());
    }
    return values;
}

QVector<QString> LIPNewAttrFeatureForm::getNames()
{
    return attributeNames;
}
