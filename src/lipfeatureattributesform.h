#ifndef LIPFEATUREATTRIBUTESFORM_H
#define LIPFEATUREATTRIBUTESFORM_H

#include <QDialog>
#include "vector/lipvectorlayer.h"
namespace Ui {
class LIPFeatureAttributesForm;
}

class LIPFeatureAttributesForm : public QDialog
{
    Q_OBJECT

public:
    explicit LIPFeatureAttributesForm(QWidget *parent = nullptr);
    ~LIPFeatureAttributesForm();
    void setFeature(OGRLayer* layer, int featureIndex);
private:
    Ui::LIPFeatureAttributesForm *ui;
    OGRLayer *mLayer;
    int mIndex;
};

#endif // LIPFEATUREATTRIBUTESFORM_H
