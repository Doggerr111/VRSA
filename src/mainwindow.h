#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "vector/lipvectorconvertor.h"
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
#include "raster/liprasterlayer.h"
#include <QObject>
#include "lipproject.h"
#include <QTreeWidgetItem>
#include "treeView/liptreewidgetitem.h"
#include "lipvectorstyleform.h"
#include <lippostgisconnectionform.h>
#include <lipwidgetmanager.h>
#include <algorithm>
#include <geoprocessing/lipcutlayerform.h>
#include "lipattributestableform.h"
#include "geoprocessing/liptriangulation.h"
#include "lipcoordinatesystemlibrary.h"
#include "liptriangulationgeos.h"
#include "liptriangulationgeosform.h"
#include "lipintersectionform.h"
#include "treeView/liptreewidgetrasteritem.h"
#include "lipvoronoidiagramform.h"
#include "lipvectorunionform.h"
#include "liprasterstyledialog.h"
#include "lipvectorreprojectform.h"
#include "lipvectorassignprojectform.h"
#include "lipvectorpropertiesform.h"
#include "lipcutrasterlayerform.h"
#include "lipreprojectrasterlayerform.h"
#include "liprastercontoursform.h"
#include "lipsettingsform.h"
#include "lipxyzconnection.h"
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
    /** добавляет ВЕКТОРНЫЙ слой */
    void addLayer(LIPVectorLayer* l);
    void addRasterLayer(LIPRasterLayer* l);

private slots:
    void layerTreeDataChanged(QTreeWidgetItem *item, int column);
    void onItemChanged(QTreeWidgetItem* item, int column);

    void scenePos(QPointF p);
    void centerScene();
    void applyScale();

    void recalculateScale();

    void changeExtent(QPointF click_pos, QPointF cur_pos);

    QRectF getSceneRect();

    //void showContextMenu(QPoint p);


    //void test(QPointF p);
    //void on_treeView_clicked(const QModelIndex &index);



    void on_actionNew_point_layer_triggered();

    void on_actionNew_line_layer_triggered();

    void on_actionNew_polygon_layer_triggered();

    void on_actionLoad_vector_layer_triggered();

    void on_pushButton_addPointFeature_clicked();

    void layersOrderChanged(); //когда срабатывает дроп евент в виджете дерева

    void on_LayerTree_itemActivated(QTreeWidgetItem *item, int column);

    void on_pushButton_addPointFeature_clicked(bool checked);

    void on_actionConnect_to_PostGIS_triggered();

    void on_pushButtonTestRep_clicked();

    void on_pushButton_14_clicked();



    void on_actionGeosTriangulation_triggered();

    void on_actionIntersection_triggered();

    void on_pushButton_addFeature_clicked(bool checked);

    void on_actionOpenRasterLayer_triggered();

    void on_actionVoronoiDiagram_triggered();

    void on_actionUnion_triggered();

    void on_actionRenderMap_triggered();

    void on_actionSaveProject_triggered();

    void on_action_triggered();

    void on_crsComboBox_currentIndexChanged(int index);

    void on_actionVectorReproject_triggered();

    void on_actionAssignVectorProjection_triggered();

    void on_actionCutRasterByVectorMask_triggered();

    void on_actionRasterReproject_triggered();

    void on_actionRasterContours_triggered();

    void on_pushButtonMoveLayerUp_clicked();

    void on_pushButtonMoveLayerDown_clicked();

    void on_pushButtonDeleteLayer_clicked();

    void on_actionSettings_triggered();

    void on_pushButtonSelectFeature_clicked();

    void on_pushButtonSelectFeature_clicked(bool checked);

    void on_pushButtonShowAttributeTable_clicked();

    void on_pushButtonZoomToLayer_clicked();

    void on_pushButton_clicked();

signals:
    void start_add();
    void newVectorLayer(LIPVectorLayer*);
    void zoomed();
    void scaleFactorChanged(double);
    void tileLoadNeeded(int, QRectF);
public slots:
    void redrawNeeded(double);
    void showLayerContextMenu(const QPoint&);
    void showDBLayerContextMenu(const QPoint&);
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
