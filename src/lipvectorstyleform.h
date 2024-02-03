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
