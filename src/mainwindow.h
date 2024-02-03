#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "lipvectorconvertor.h"
#include <QMainWindow>
#include <lipvectorreader.h>
#include <QFileDialog>
#include <lipmapscene.h>
#include <qgraphicsitem.h>
//#include <lippointgraphicsitem.h>
#include <qgraphicsscene.h>
#include <QWheelEvent>
#include <QThread>
#include <lipmapcalculations.h>
#include <QDesktopWidget>
#include <QScrollBar>
#include <QScreen>
#include <QGraphicsView>
#include <lipaddingitems.h>
#include <QPainter>
#include <QImage>
#include <QPen>
#include <QMessageBox>
#include <liplayertreeitem.h>
#include <liplayertreemodel.h>
#include <QTime>
#include "treeView/lipobjecttreemodel.h"
#include "vector/lippointlayer.h"
#include "vector/liplinelayer.h"
#include "vector/lippolygonlayer.h"
#include <QObject>
#include "lipproject.h"
#include <QTreeWidgetItem>
#include "treeView/liptreewidgetitem.h"
#include "lipvectorstyleform.h"
#include <lippostgisconnectionform.h>
#include <lipwidgetmanager.h>
#include <algorithm>
#include <lipcutlayerform.h>
#include "lipattributestableform.h"
#include "geoprocessing/liptriangulation.h"
#include "lipcoordinatesystemlibrary.h"
#include "liptriangulationgeos.h"
#include "liptriangulationgeosform.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void sceneInitialization();
    void addLayer(LIPVectorLayer* l);

private slots:
    void layerTreeDataChanged(QTreeWidgetItem *item, int column);
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void scenePos(QPointF p);

    void recalculateScale();

    void changeExtent(QPointF click_pos, QPointF cur_pos);

    QRectF getSceneRect();

    //void showContextMenu(QPoint p);


    void test(QPointF p);
    //void on_treeView_clicked(const QModelIndex &index);

    void on_pushButton_GeoTiff_clicked();

    void on_pushButton_4_clicked();

    void on_actionNew_point_layer_triggered();

    void on_actionNew_line_layer_triggered();

    void on_actionNew_polygon_layer_triggered();

    void on_actionLoad_vector_layer_triggered();

    void on_pushButton_addPointFeature_clicked();

    void layersOrderChanged(); //когда срабатывает дроп евент в виджете дерева

    void on_LayerTree_itemActivated(QTreeWidgetItem *item, int column);

    void on_pushButton_addPointFeature_clicked(bool checked);

    void on_actionConnect_to_PostGIS_triggered();

    void on_pushButtonTriangulationTest_clicked();

    void on_pushButton_7_clicked();

    void on_pushButtonVoronoiTest_clicked();

    void on_pushButtonRenderTest_clicked();

    void on_pushButtonTestRep_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_geosTest_clicked();

    void on_actionGeosTriangulation_triggered();

    void on_actionIntersection_triggered();

signals:
    void start_add();
    void newVectorLayer(LIPVectorLayer*);
    void zoomed();
    void scaleFactorChanged(double);
public slots:
    void redrawNeeded(double);
    void showLayerContextMenu(const QPoint&);
    void deleteVector(LIPVectorLayer* layer, QTreeWidgetItem* item);


private:
    Ui::MainWindow *ui;
    double scale_factor=1;
    QPainter *painter;
    QImage img;
    QPixmap pix;
    QPainter pa;
    QImage img2;
    QVector<LIPPoint*> pVect;
    QVector<LIPVectorLayer*> projectLayers;
    LIPObjectTreeModel* layerModel;
    //LIPProject* project;
    LIPMapScene *scene;
//    QTime m_time;
//    int m_frameCount;

    // QWidget interface
protected:
    void wheelEvent(QWheelEvent *event);




    // QObject interface
public:
    bool eventFilter(QObject *watched, QEvent *event);
};
#endif // MAINWINDOW_H
