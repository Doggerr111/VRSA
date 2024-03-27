#ifndef LIPVECTORPROPERTIESFORM_H
#define LIPVECTORPROPERTIESFORM_H

#include <QDialog>
#include "vector/lipvectorlayer.h"
namespace Ui {
class LIPVectorPropertiesForm;
}

class LIPVectorPropertiesForm : public QDialog
{
    Q_OBJECT

public:
    explicit LIPVectorPropertiesForm(LIPVectorLayer *layer, QWidget *parent = nullptr);
    ~LIPVectorPropertiesForm();

private:
    Ui::LIPVectorPropertiesForm *ui;
};

#endif // LIPVECTORPROPERTIESFORM_H
