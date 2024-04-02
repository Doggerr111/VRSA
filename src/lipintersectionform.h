#ifndef LIPINTERSECTIONFORM_H
#define LIPINTERSECTIONFORM_H

#include <QDialog>
#include "lipvectortypechecker.h"
#include <QFileDialog>
#include "lipvectorintersection.h"
#include "liplayercreator.h"
namespace Ui {
class LIPIntersectionForm;
}

class LIPIntersectionForm : public QDialog
{
    Q_OBJECT

public:
    explicit LIPIntersectionForm(QWidget *parent = nullptr);
    ~LIPIntersectionForm();

private slots:
    //void on_pushButtonOk_clicked();

    void on_pushButtonFileDialog_clicked();

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::LIPIntersectionForm *ui;
    QString fileName;
    LIPVectorLayer* outputLayer;
};

#endif // LIPINTERSECTIONFORM_H
