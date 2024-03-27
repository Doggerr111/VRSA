#ifndef LIPVECTORSTYLEFORM_H
#define LIPVECTORSTYLEFORM_H

#include <QWidget>
#include <QColorDialog>
#include <QPen>
#include <QBrush>
#include "vector/lippointgraphicsitem.h"
#include "vector/liplinegraphicsitem.h"
#include "vector/lippolygongraphicsitem.h"

#include <QGraphicsEllipseItem>
#include <QGraphicsScene>
#include "vector/lipvectorstyle.h"
#include "vector/lipvectorlayer.h"
#include "vector/lippointlayer.h"
#include "vector/lippolygonlayer.h"
#include "vector/liplinelayer.h"
#include <QFileDialog>
namespace Ui {
class LIPVectorStyleForm;
}

class LIPVectorStyleForm : public QDialog
{
    Q_OBJECT

public:
    explicit LIPVectorStyleForm(QWidget *parent = nullptr, LIPVectorLayer* layer=nullptr,LIPVectorStyle* style=nullptr);
    ~LIPVectorStyleForm();
    LIPVectorStyle* getStyle();
    /** Инициализация объекта слоя */
    void startRenderFeature();

private slots:
    void on_pushButtonPointColor_clicked();



    void on_pushButtonOk_clicked();

    void on_lineEditPointSize_textChanged(const QString &arg1);

    void on_lineEditPointSize_textEdited(const QString &arg1);

    void on_pushButtonCancel_clicked();

    void on_lineEditLineWidth_textChanged(const QString &arg1);

    void on_pushButtonLineColor_clicked();



    void on_pushButtonSquareSymbol_clicked();

    void on_pushButtonCircleSymbol_clicked();

    void on_pushButtonTriangleSymbol_clicked();

    void on_pushButtonRhombusSymbol_clicked();

    void on_pushButtonPointBrushColor_clicked();

    void on_pushButtonInvTriangleSymbol_clicked();

    void on_pushButtonLoadImage_clicked();

    void on_comboBoxPointBrushType_currentIndexChanged(int index);

    void on_comboBox_currentIndexChanged(int index);

    void on_lineEditPointPenWidth_textChanged(const QString &arg1);


    void on_comboBoxLinePenStyle_currentIndexChanged(int index);

    void on_comboBoxLineCapStyle_currentIndexChanged(int index);

    void on_comboBoxLineJoinStyle_currentIndexChanged(int index);

    void on_checkBox_clicked(bool checked);

    void on_pushButtonAddRow_clicked();

    void on_pushButtonDeleteRow_clicked();

    void on_pushButtonAplyCustomDashLine_clicked();

    void on_lineEditPolyBorderWidth_textChanged(const QString &arg1);

    void on_pushButtonPolyBorderColor_clicked();



    void on_comboBoxPolyBorderStyle_currentIndexChanged(int index);

    void on_pushButtonPolygonBrushColor_clicked();

    void on_comboBoxPolyBrushStyle_currentIndexChanged(int index);

    void on_comboBoxPolyBorderJoinStyle_currentIndexChanged(int index);

private:
    QPen pen;
    QBrush brush;
    QGraphicsScene* mScene;
    LIPPointGraphicsItem *pointItem;
    LIPPolygonGraphicsItem *polyItem;
    LIPLineGraphicsItem *lineItem;

    //QGraphicsEllipseItem *pointItem
    LIPVectorStyle *mStyle;
    LIPVectorLayer *mLayer;
    Ui::LIPVectorStyleForm *ui;

};

#endif // LIPVECTORSTYLEFORM_H
