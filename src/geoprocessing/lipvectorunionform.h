#ifndef LIPVECTORUNIONFORM_H
#define LIPVECTORUNIONFORM_H

#include <QDialog>
#include <QFileDialog>
#include "lipvectorunion.h"
#include "liplayercreator.h"
#include "lipvectortypechecker.h"
namespace Ui {
class LIPVectorUnionForm;
}

class LIPVectorUnionForm : public QDialog
{
    Q_OBJECT

public:
    explicit LIPVectorUnionForm(QWidget *parent = nullptr);
    ~LIPVectorUnionForm();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_pushButtonFileDialog_clicked();

private:
    Ui::LIPVectorUnionForm *ui;
    QString fileName;
    LIPVectorLayer* outputLayer;
};

#endif // LIPVECTORUNIONFORM_H
