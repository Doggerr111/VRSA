#ifndef LIPNEWLINELAYERFORM_H
#define LIPNEWLINELAYERFORM_H

#include <QDialog>
#include "liplayercreator.h"
#include <QFileDialog>
#include "vector/lipvectorlayer.h"
#include "lipcoordinatesystemlibrary.h"
namespace Ui {
class LIPNewLineLayerForm;
}

class LIPNewLineLayerForm : public QDialog
{
    Q_OBJECT

public:
    explicit LIPNewLineLayerForm(QWidget *parent = nullptr, LIPGeometryType geomType = LIPGeometryType::LIPPoint);
    ~LIPNewLineLayerForm();

    OGRLayer* returnOGRLayer();
    LIPVectorLayer *returnLayer();


private slots:
    void on_pushButton_clicked();

    void on_pushButton_ok_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::LIPNewLineLayerForm *ui;
    QString fileName;
    OGRLayer *layer;
    LIPVectorLayer *LIPLayer;
    LIPGeometryType geometryType;
};

#endif // LIPNEWLINELAYERFORM_H
