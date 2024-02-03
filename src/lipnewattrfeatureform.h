#ifndef LIPNEWATTRFEATUREFORM_H
#define LIPNEWATTRFEATUREFORM_H

#include <QDialog>
#include <vector/lipvectorlayer.h>
#include "QLabel"
#include "QLineEdit"
#include <QVBoxLayout>
namespace Ui {
class LIPNewAttrFeatureForm;
}

class LIPNewAttrFeatureForm : public QDialog
{
    Q_OBJECT

public:
    explicit LIPNewAttrFeatureForm(QWidget *parent = nullptr);
    ~LIPNewAttrFeatureForm();
    void setAttributeNames(QVector<QString> names);
    void setAttributeTypes(QVector<LIPAttributeType> types);
    QVector<QString> getValues();
    QVector<QString> getNames();

private:
    Ui::LIPNewAttrFeatureForm *ui;
    QVector<QString> attributeNames;
    QVector<LIPAttributeType> attributeTypes;
    QVector<QLineEdit*> lineEdits;

};

#endif // LIPNEWATTRFEATUREFORM_H
