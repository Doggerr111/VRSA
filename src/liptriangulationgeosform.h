#ifndef LIPTRIANGULATIONGEOSFORM_H
#define LIPTRIANGULATIONGEOSFORM_H

#include <QDialog>
#include <QFileDialog>
#include "lipproject.h"
#include "liplayercreator.h"
#include "geoprocessing/liptriangulationgeos.h"
#include "vector/lipvectortypechecker.h"
namespace Ui {
class LIPTriangulationGeosForm;
}

class LIPTriangulationGeosForm : public QDialog
{
    Q_OBJECT

public:
    explicit LIPTriangulationGeosForm(QWidget *parent = nullptr);
    ~LIPTriangulationGeosForm();
    LIPVectorLayer* getTriangulationLayer();

private slots:
    void on_pushButtonFileDialog_clicked();

    void on_pushButtonOk_clicked();

    void on_pushButtonCancel_clicked();



private:
    Ui::LIPTriangulationGeosForm *ui;
    QString fileName;
    LIPVectorLayer* outputLayer;
};

#endif // LIPTRIANGULATIONGEOSFORM_H
