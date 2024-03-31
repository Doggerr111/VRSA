#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gdal.h"
#include "gdal_priv.h"
#include "ogrsf_frmts.h"
#include "QDebug"
#include <gdalwarper.h>
#include "lipnewlinelayerform.h"
#include <QOpenGLWidget>
#include <QSqlDatabase>
#include <liptriangulation.h>
#include <QSplitter>
#include <gdal/ogr_srs_api.h>
#include <gdal/ogr_geometry.h>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("VRSA");
    setWindowIcon(QIcon(":/images/icons/vector-and-raster-spatian-analysys.png"));
    OGRGeometry *f;



    //LIPProject::getInstance();
    LIPCoordinateSystemLibrary *lib = new LIPCoordinateSystemLibrary();
    foreach(LIPCoordinateSystem *CRS, lib->getCRSLib())
    {
        OGRCoordinateTransformation* ref = OGRCreateCoordinateTransformation(CRS->getOGRSpatialRef(), CRS->getOGRSpatialRef());
        //f->transform(ref);
        qDebug()<<CRS->getName();
        LIPProject::getInstance().addCoordinateSystem(CRS);
    }
    delete lib;
    connect(ui->LayerTree, SIGNAL(itemDropped()), this, SLOT(layersOrderChanged()));
    ui->LayerTree->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->DBLayerTree->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->LayerTree, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(layerTreeDataChanged(QTreeWidgetItem*, int)));
    connect(ui->LayerTree, SIGNAL(customContextMenuRequested(const QPoint&)),
            this, SLOT(showLayerContextMenu(const QPoint&)));
    connect(ui->DBLayerTree, SIGNAL(customContextMenuRequested(const QPoint&)),
            this, SLOT(showDBLayerContextMenu(const QPoint&)));
    sceneInitialization();
    LIPWidgetManager::getInstance().setMainWindow(this);
    LIPWidgetManager::getInstance().showMessage("ffsa", 2000, messageStatus::Success);

    ui->right_menu_frame->setAlignment(Qt::AlignRight);
    ui->left_menu_frame->setAlignment(Qt::AlignLeft);


    OGRSpatialReference srcSRS;
    qDebug()<<srcSRS.importFromProj4("+proj=longlat +ellps=GSK2011 +no_defs +type=crs");

    ui->LayerTree->setDragDropMode(QAbstractItemView::DragDrop);
    ui->LayerTree->setDefaultDropAction(Qt::MoveAction);

    // Разрешить только внешнее перетаскивание
    ui->LayerTree->setDragDropOverwriteMode(false);
    ui->LayerTree->setDragDropMode(QAbstractItemView::InternalMove);
    ui->LayerTree->setDragEnabled(true);
    ui->LayerTree->setAcceptDrops(true);
    ui->LayerTree->setDropIndicatorShown(true);
    ui->LayerTree->setDragDropMode(QAbstractItemView::DragDrop);

    // Запретить вложение элементов
    ui->LayerTree->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->LayerTree->setDropIndicatorShown(true);
    ui->LayerTree->setDragDropMode(QAbstractItemView::InternalMove);
    ui->LayerTree->setDragDropMode(QAbstractItemView::DragDrop);
    ui->LayerTree->setDropIndicatorShown(true);

    connect(ui->LayerTree, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(onItemChanged(QTreeWidgetItem*, int)));
    //srcSRS.IS
    //QSplitter *splitter = new QSplitter(ui->central_frame);
    //splitter->addWidget(ui->frame_2);
    //    img2 = QImage(QSize(ui->graphicsView->viewport()->width(),ui->graphicsView->viewport()->height()), QImage::Format_ARGB32_Premultiplied);
    //    //img2.fill( 0 );
    //    scene= new LIPMapScene();
    //    if (pa.begin( &img2 ))
    //    {
    //        //scene->update();
    //        QMessageBox::warning(this,"all good","all good");
    //    }
    //    pa.setRenderHint( QPainter::Antialiasing, true );

    //    connect(ui->graphicsView,SIGNAL(MapHolderResized()),this,SLOT(recalculateScale()));
    //    //    img.setDotsPerMeterX(200);
    //    //    img.setDotsPerMeterY(200);
    //    pix=QPixmap(300,300);
    //    img=QImage(QSize(40000,30000),QImage::Format_RGB32);
    //    QPen pen1;
    //    pen1.setWidth(1);
    //    pen1.setColor(Qt::yellow);

    //    painter=new QPainter(&pix);
    //    painter->setPen(pen1);

    //    connect(this, SIGNAL(newVectorLayer(LIPVectorLayer*)), scene, SLOT(drawVectorLayer(LIPVectorLayer*)));
    //    //scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    //    ui->graphicsView->setViewport(new QOpenGLWidget);

    //    //LIPMapScene *calculationScene = new LIPMapScene();
    //    connect(scene,SIGNAL(pos_changed(QPointF)),this,SLOT(scenePos(QPointF)));
    //    connect(scene,SIGNAL(scene_dragging(QPointF,QPointF)),this,SLOT(changeExtent(QPointF,QPointF)));
    //    ui->graphicsView->setScene(scene);
    //    ui->graphicsView->setTransformationAnchor( QGraphicsView::AnchorViewCenter );
    //    //ui->graphicsView->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::HighQualityAntialiasing);

    //    //ui->graphicsView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    //    ui->graphicsView->setOptimizationFlags(QGraphicsView::DontSavePainterState);
    //    ui->graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    //    ui->graphicsView->setAttribute(Qt::WA_OpaquePaintEvent);
    //    ui->graphicsView->setAttribute(Qt::WA_NoSystemBackground);
    //    ui->graphicsView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);

    //    layerModel = new LIPObjectTreeModel(this);
    //    QStringList cols;
    //    cols << "objectName";
    //    layerModel->setColumns(cols);
    //    QObject* item1 = new QObject();
    //    item1->setObjectName("Father");
    ////    QObject* item2 = new QObject(item1);
    ////    item2->setProperty("objectName", "Son");
    //    //layerModel->addItem(item1, QModelIndex());
    //    ui->treeView->setModel(layerModel);

    //    ui->graphicsView->scene()->setSceneRect(-180,90,360,-180);

    //    //scene->setSceneRect(-98292019,89852960,704459722-98292019, -453620743-89852960);
    //    //ui->graphicsView->fitInView(-180,90,360,-180);
    //    //ui->graphicsView->setScene(ui->graphicsView->scene());
    //    ui->graphicsView->scale(1,-1);

    //    //ui->graphicsView->setDragMode(QGraphicsView::NoDrag);

    //    //ui->graphicsView->setRenderHints(QPainter::Antialiasing);

    //    //connect(ui->pushButton_addPointFeature, SIGNAL(clicked()),scene, SLOT(addPointFeature()));
    //    project = new LIPProject();
    //    connect(ui->graphicsView, SIGNAL(MapHolderZoomed(double)), this, SLOT(redrawNeeded(double)));

    //    ui->treeView->setDragEnabled (true);              // Enable drag
    //    ui->treeView->viewport ()->setAcceptDrops (true); // ViewPort accepts the action, the default is copy operation
    //    ui->treeView->showDropIndicator ();               // Setting up the indication
    //    ui->treeView->setDragDropMode (QTreeWidget::InternalMove);// internal movement
    //ui->LayerTree->setDragDropMode(QTreeWidget::NoDragDrop);
    //    //LIPTreeWidget *widg=ui->treeView;
    //    connect(ui->LayerTree, SIGNAL(itemDropped()), this, SLOT(layersOrderChanged()));


    //    ui->LayerTree->setContextMenuPolicy(Qt::CustomContextMenu);
    //    connect(ui->LayerTree, SIGNAL(customContextMenuRequested(const QPoint&)),
    //        this, SLOT(showLayerContextMenu(const QPoint&)));




    //    LIPTreeWidgetItem *item = new LIPTreeWidgetItem();
    //    item->setText(0, "123");
    //    item->setCheckState(0,Qt::Checked);
    //    item->setIcon(0,QIcon(":/ui/icons/pointLayer.png"));
    //    //item->setFileName(layerForm->returnLayer().)
    //    ui->LayerTree->addTopLevelItem(item);
    //    ui->LayerTree->setDragEnabled(true);
    //    ui->LayerTree->setDropIndicatorShown(true);
    //    item->setFlags(item->flags() | Qt::ItemIsDragEnabled );
    //    item->setFlags(item->flags() & ~Qt::ItemIsDropEnabled );
    //    item->setToolTip(0, "asfSFs");

    //    LIPTreeWidgetItem *item123 = new LIPTreeWidgetItem();
    //    item123->setText(0, "`13`13`13`");
    //    item123->setCheckState(0,Qt::Unchecked);
    //    item123->setIcon(0,QIcon(":/ui/icons/pointLayer.png"));
    //    //item->setFileName(layerForm->returnLayer().)
    //    ui->LayerTree->addTopLevelItem(item123);
    //    ui->LayerTree->setDragEnabled(true);
    //    ui->LayerTree->setDropIndicatorShown(true);
    //    item123->setFlags(item->flags() | Qt::ItemIsDragEnabled );
    //ui->LayerTree->installEventFilter(this);
    //ui->pushButton_7->hide();
    //ui->pushButtonRenderTest->hide();
    //ui->pushButtonTriangulationTest->hide();
    //ui->pushButtonVoronoiTest->hide();
    //ui->pushButton_3->hide();
    //ui->pushButton_GeoTiff->hide();
    //ui->pushButton->hide();
    //ui->pushButton_2->hide();
    //ui->pushButton_4->hide();
//    QPushButton *button2 = new QPushButton();
//    button2->setIcon(ui->actionAssignVectorProjection->icon());
//    ui->frameLayerTree->layout()->addWidget(button2);


}

MainWindow::~MainWindow()
{
    delete scene;
    delete ui;
}

void MainWindow::sceneInitialization()
{
    scene= new LIPMapScene();
    //ui->graphicsView->setViewport(new QOpenGLWidget);
    ui->graphicsView->setScene(scene);
    //connect(ui->graphicsView,SIGNAL(MapHolderResized()),this,SLOT(recalculateScale()));
    connect(this, SIGNAL(newVectorLayer(LIPVectorLayer*)), scene, SLOT(drawVectorLayer(LIPVectorLayer*)));
    connect(scene,SIGNAL(pos_changed(QPointF)),this,SLOT(scenePos(QPointF)));
    connect(scene,SIGNAL(scene_dragging(QPointF,QPointF)),this,SLOT(changeExtent(QPointF,QPointF)));

    //connect(scene,SIGNAL(startAdding()),ui->graphicsView,SLOT(onAddingFeatures()));
    //connect(scene,SIGNAL(stopAdding()),ui->graphicsView,SLOT(onStopAddingFeatures()));

    connect(ui->graphicsView, SIGNAL(MapHolderZoomed(double)), this, SLOT(redrawNeeded(double)));
    ui->graphicsView->setOptimizationFlags(QGraphicsView::DontSavePainterState);
    ui->graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    //ui->graphicsView->setAttribute(Qt::WA_OpaquePaintEvent);
    //ui->graphicsView->setAttribute(Qt::WA_NoSystemBackground);
    ui->graphicsView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    // ui->graphicsView->scene()->setSceneRect(-180,90,360,-180);
    double xMin = -20037508.34; // примерно для -180 градусов
    double yMax = 20037508.34;  // примерно для 90 градусов
    double width = 40075016.68; // ширина всей карты в EPSG:3857 координатах
    double height = 20037508.34; // высота всей карты в EPSG:3857 координатах

    // Установка сцены в пределах EPSG:3857 координат
    ui->graphicsView->scale(1,-1);
    //ui->graphicsView->scene()->setSceneRect(xMin, yMax, width, height);

    ui->graphicsView->setSceneRect(xMin, yMax, width, -2*height);
    //ui->graphicsView->setFixedWidth(100);



    //ui->graphicsView->setRenderHints(QPainter::Antialiasing);

}

void MainWindow::addLayer(LIPVectorLayer *l)
{
    emit newVectorLayer(l);
    connect(this, SIGNAL(scaleFactorChanged(double)), l, SLOT(setSceneScaleFactor(double)));
    connect(l, SIGNAL(needRepaint()),scene, SLOT(updateVectorLayer()));
    connect(&LIPProject::getInstance(), SIGNAL(crsChanged()), l, SLOT(flyReprojection()));
    LIPProject::getInstance().addVectorLayer(l);

    LIPPointLayer* new_layer=dynamic_cast<LIPPointLayer*>(l);
    LIPTreeWidgetItem *item = new LIPTreeWidgetItem();
    item->setText(0,l->returnGISName());
    item->setCheckState(0,Qt::Checked);
    item->setFlags(item->flags() | Qt::ItemIsDragEnabled );
    item->setToolTip(0,l->returnFileName());
    ui->LayerTree->addTopLevelItem(item);
    layersOrderChanged();
    if (new_layer!=nullptr) //если активный слой точечный
    {
        item->setIcon(0,QIcon(":/images/icons/pointLayer.png"));
        return;


    }
    LIPLineLayer* new_line_layer=dynamic_cast<LIPLineLayer*>(l);
    if(new_line_layer!=nullptr) //если линия
    {
        item->setIcon(0,QIcon(":/images/icons/lineLayer.png"));
        return;
    }

    LIPPolygonLayer* new_poly_layer=dynamic_cast<LIPPolygonLayer*>(l);
    if(new_poly_layer!=nullptr) //если полигон
    {
        item->setIcon(0,QIcon(":/images/icons/polygonLayer.png"));

        return;
    }

}

void MainWindow::addRasterLayer(LIPRasterLayer *l)
{
    LIPWidgetManager::getInstance().getScene()->addItem(l->getPixmapItem());
    LIPProject::getInstance().addRasterLayer(l);
    LIPTreeWidgetRasterItem *item = new LIPTreeWidgetRasterItem();
    item->setText(0,l->getGISName());
    item->setCheckState(0,Qt::Checked);
    item->setFlags(item->flags() | Qt::ItemIsDragEnabled );
    item->setToolTip(0,l->getFileName());
    ui->LayerTree->addTopLevelItem(item);
    layersOrderChanged();
}

void MainWindow::layerTreeDataChanged(QTreeWidgetItem *item, int column)
{

    LIPVectorLayer* activeLayer = LIPProject::getInstance().getVectorLayerByPath(item->toolTip(0));
    if (activeLayer==nullptr)
    {
        LIPRasterLayer* rasterLayer = LIPProject::getInstance().getRasterLayerByPath(item->toolTip(0));
        if (rasterLayer==nullptr)
            return;
        rasterLayer->setVisible(item->checkState(column));
        return;
    }
    LIPPointLayer* newPointLayer=dynamic_cast<LIPPointLayer*>(activeLayer);
    if (newPointLayer!=nullptr) //если активный слой точечный
    {
        newPointLayer->setVisible(item->checkState(column));
        return;
    }

    LIPLineLayer* newLineLayer=dynamic_cast<LIPLineLayer*>(activeLayer);
    if(newLineLayer!=nullptr) //если линия
    {
        newLineLayer->setVisible(item->checkState(column));
        return;
    }

    LIPPolygonLayer* newPolyLayer=dynamic_cast<LIPPolygonLayer*>(activeLayer);
    if(newPolyLayer!=nullptr) //если полигон
    {
        newPolyLayer->setVisible(item->checkState(column));
        return;
    }

}

void MainWindow::onItemChanged(QTreeWidgetItem *item, int column)
{
    //QMessageBox::warning(this,"","");
}


void MainWindow::on_pushButton_clicked()
{
    //Writing
    GDALAllRegister();
    const char *shpFileName = "/home/doger/Downloads/ASM_adm/ASM_adm3.shp";
    const char *driverName = "ESRI Shapefile";
    GDALDriver *driver = GetGDALDriverManager()->GetDriverByName(driverName); //getting a driver
    GDALDataset *dataset = driver->Create(shpFileName, 0,0,0, GDT_Unknown, NULL);
    OGRLayer *layer = dataset->CreateLayer("ring", NULL, wkbMultiLineString, NULL);




}

void MainWindow::redrawNeeded(double f)
{
    emit scaleFactorChanged(ui->graphicsView->transform().m11());
}

void MainWindow::showLayerContextMenu(const QPoint &f)
{
    QTreeWidgetItem *clickedItem=ui->LayerTree->itemAt(f);
    if (clickedItem==nullptr)
        return;
    QString path=clickedItem->toolTip(0);
    if (path.isEmpty())
        return;
    if (dynamic_cast<LIPTreeWidgetRasterItem*>(clickedItem)!=nullptr) //если выбран растровый слой
    {
        LIPRasterLayer* selectedRasterLayer=LIPProject::getInstance().getRasterLayerByPath(path);
        if (selectedRasterLayer==nullptr)
            return;
        QMenu menu;
        QAction* actionProperties = new QAction(QString::fromUtf8("Свойства"), this);
        menu.addAction(actionProperties);
        menu.addSeparator();
        QAction* actionStyle = new QAction(QString::fromUtf8("Стиль"), this);
        menu.addAction(actionStyle);
        // стилизация
        connect(actionStyle, &QAction::triggered, this, [selectedRasterLayer]() {
            LIPRasterStyleDialog* form = new LIPRasterStyleDialog(selectedRasterLayer);
            form->exec();
        });
        menu.addAction(actionStyle);
        QAction* actionDelete = new QAction(QString::fromUtf8("Удалить"), this);
        connect(actionDelete, &QAction::triggered, this, [selectedRasterLayer, clickedItem, this]() {

            for(int i=0; i<LIPProject::getInstance().getRasterLayers().size();i++)
            {
                if (LIPProject::getInstance().getRasterLayers().at(i)==selectedRasterLayer)
                {
                    LIPProject::getInstance().deleteRasterLayerByPath(selectedRasterLayer->getFileName());

                    QTreeWidgetItem *parent = clickedItem->parent();
                    int index;
                    if (parent) {
                        index = parent->indexOfChild(clickedItem);
                        delete parent->takeChild(index);
                    }
                    else {
                        index = ui->LayerTree->indexOfTopLevelItem(clickedItem);
                        delete ui->LayerTree->takeTopLevelItem(index);
                    }
                    //delete clickedItem;
                }
            }
        });
        menu.addAction(actionDelete);
        menu.show();
        menu.exec(ui->LayerTree->mapToGlobal(f));
        delete actionProperties;
        delete actionStyle;
        delete actionDelete;

    }

    LIPVectorLayer* selectedLayer=LIPProject::getInstance().getVectorLayerByPath(path);
    if (selectedLayer==nullptr)
        return;
    QMenu menu;
    QAction* actionProperties = new QAction(QString::fromUtf8("Свойства"), this);
    connect(actionProperties, &QAction::triggered, this, [selectedLayer]() {
        LIPVectorPropertiesForm* form = new LIPVectorPropertiesForm(selectedLayer);
        form->exec();
        delete form;
    });
    menu.addAction(actionProperties);
    menu.addSeparator();
    QAction* actionStyle = new QAction(QString::fromUtf8("Стиль"), this);
    menu.addAction(actionStyle);
    connect(actionStyle, &QAction::triggered, this, [selectedLayer]() {
        LIPVectorStyleForm* form = new LIPVectorStyleForm(nullptr,selectedLayer);
        form->exec();
        delete form;
    });
    menu.addAction(actionStyle);
    QAction* actionDelete = new QAction(QString::fromUtf8("Удалить"), this);
    connect(actionDelete, &QAction::triggered, this, [selectedLayer, clickedItem, this]() {
        for(int i=0; i<LIPProject::getInstance().getVectorLayers().size();i++)
        {
            if (LIPProject::getInstance().getVectorLayers().at(i)==selectedLayer)
            {
                LIPProject::getInstance().deleteVectorByPath(selectedLayer->returnFileName());

                QTreeWidgetItem *parent = clickedItem->parent();
                int index;
                if (parent) {
                    index = parent->indexOfChild(clickedItem);
                    delete parent->takeChild(index);
                }
                else {
                    index = ui->LayerTree->indexOfTopLevelItem(clickedItem);
                    delete ui->LayerTree->takeTopLevelItem(index);
                }
                //delete clickedItem;
            }
        }
    });

    QAction* attrTable = new QAction(QString::fromUtf8("Таблица атрибутов"), this);
    connect(attrTable, &QAction::triggered, this, [selectedLayer, clickedItem, this]() {
        LIPAttributesTableForm* form = new LIPAttributesTableForm();
        if (selectedLayer!=nullptr)
        {
            form->setLayer(selectedLayer);
            form->exec();
            delete form;
        }


    });
    menu.addAction(actionDelete);
    menu.addAction(attrTable);
    menu.show();
    menu.exec(ui->LayerTree->mapToGlobal(f));
    delete actionProperties;
    delete actionStyle;
    delete actionDelete;


}

void MainWindow::showDBLayerContextMenu(const QPoint &f)
{
    QTreeWidgetItem *clickedItem=ui->DBLayerTree->itemAt(f);
    if (clickedItem==nullptr)
        return;
    GDALDataset *pgDs = LIPProject::getInstance().getPostGISDataSet();
    if (pgDs==nullptr)
        return;



    QMenu menu;
    QAction* actionProperties = new QAction(QString::fromUtf8("Свойства"), this);
    menu.addAction(actionProperties);
    menu.addSeparator();


    QAction* attrTable = new QAction(QString::fromUtf8("Добавить слой в проект"), this);
    connect(attrTable, &QAction::triggered, this, [pgDs, clickedItem]()
    {
        QVector<OGRLayer*> vect=LIPVectorReader::readLayersFromDataset(pgDs);
        for (int i=0; i<vect.count(); i++)
        {
            OGRLayer *newLayer=vect.at(i);
            if (newLayer==nullptr)
                continue;
            if (vect.at(i)->GetName()!=clickedItem->text(0))
                continue;
            LIPGeometryType type = LIPVectorReader::readGeometryType(newLayer);
            switch (type)
            {

            case LIPGeometryType::LIPPoint:
            {
                new LIPPointLayer(newLayer,newLayer->GetName(), "postgis_connection/"+QString(newLayer->GetName()), pgDs);
                delete clickedItem;
                return;

            }

            case LIPGeometryType::LIPLineString:
            {
                new LIPLineLayer(newLayer,newLayer->GetName(), "postgis_connection/"+QString(newLayer->GetName()), pgDs);
                delete clickedItem;
                return;
            }
            case LIPGeometryType::LIPPolygon:
            {
                new LIPPolygonLayer(newLayer,newLayer->GetName(), "postgis_connection/"+QString(newLayer->GetName()), pgDs);
                delete clickedItem;
                return;
            }
            case LIPGeometryType::LIPUnknown:
            {
                LIPWidgetManager::getInstance().showMessage("Тип геометрии слоя не известен или не поддерживается", 1000, Error);
                delete clickedItem;
                return;
            }
            }
        }

    });

    menu.addAction(attrTable);
    menu.show();
    menu.exec(ui->DBLayerTree->mapToGlobal(f));
    delete actionProperties;

}

void MainWindow::deleteVector(LIPVectorLayer *layer, QTreeWidgetItem *item)
{

}

void MainWindow::on_pushButton_2_clicked()
{
    //reading shp!)
    QString fileName=QFileDialog::getOpenFileName(this,"","");
    QFile file(fileName);
    if (file.open(QFile::ReadWrite))
    {
        //QMessageBox::information(this,"","Open!");
    }
    OGRRegisterAll();
    QByteArray bytea=fileName.toLocal8Bit();
    const char *charname=bytea.data();
    GDALDataset *poDS = (GDALDataset *)GDALOpenEx(charname, GDAL_OF_READONLY, NULL, NULL, NULL);
    if (poDS == NULL)
    {
        qDebug()<<"Open failed.\n";
        return;
    }

    OGRLayer *poLayer = poDS->GetLayer(0);

    OGRFeature *poFeature;

    poLayer->ResetReading();
    //ui->textEdit->append(QString::number(poLayer->GetSpatialRef()->GetEPSGGeogCS()));
    while ((poFeature = poLayer->GetNextFeature()) != NULL)
    {
        OGRFeatureDefn *poFDefn = poLayer->GetLayerDefn();
        int iField;


        for (iField = 0; iField < poFDefn->GetFieldCount(); iField++)
        {
            OGRFieldDefn *poFieldDefn = poFDefn->GetFieldDefn(iField);
            if (poFieldDefn->GetType() == OFTInteger)
                qDebug()<<"%d, " +  QString::number(poFeature->GetFieldAsInteger(iField));
            else if (poFieldDefn->GetType() == OFTInteger64)
                qDebug()<<"%lld, "+QString::number( poFeature->GetFieldAsInteger64(iField));
            else if (poFieldDefn->GetType() == OFTReal)
                qDebug()<<"%.3f, "+QString::number(poFeature->GetFieldAsDouble(iField));
            else if (poFieldDefn->GetType() == OFTString)
                qDebug()<<"%s, " + QString(poFeature->GetFieldAsString(iField));
            else
                qDebug()<<"%s, " + QString(poFeature->GetFieldAsString(iField));
        }
        qDebug()<<("\n");

        OGRGeometry *poGeometry = poFeature->GetGeometryRef();

        if (poGeometry != NULL)
        {
            switch (poGeometry->getGeometryType())
            {
            case wkbLineString:
            {
                OGRLineString *line = (OGRLineString *)poGeometry;
                for (int i = 0; i < line->getNumPoints(); i++)
                {
                    //qDebug()<<"wkbLineString %d: x=%g y=%g z=%g\n", i, line->getX(i), line->getY(i), line->getZ(i);
                }
                break;
            }
            case wkbPolygon:
            {
                OGRPolygon *poly = (OGRPolygon *)poGeometry;
                OGRLinearRing *ring = (OGRLinearRing *)poly->getExteriorRingCurve();
                for (int i = 0; i < ring->getNumPoints(); i++)
                {
                    //qDebug()<<"wkbPolygon %d: x=%g y=%g z=%g\n" +QString::number(i) + QString::number( ring->getX(i)), ring->getY(i), ring->getZ(i);
                }
                break;
            }
            case wkbMultiLineString:
            {
                OGRMultiLineString *multiLine = (OGRMultiLineString *)poGeometry;
                for (int n = 0; n < multiLine->getNumGeometries(); n++)
                {
                    OGRLineString *line = (OGRLineString *)multiLine->getGeometryRef(n);
                    for (int i = 0; i < line->getNumPoints(); i++)
                    {
                        qDebug()<<"wkbMultiLineString %d-%d: x=%g y=%g z=%g\n", n, i, line->getX(i), line->getY(i), line->getZ(i);
                    }
                }

                break;
            }
            default:
                qDebug()<<"no point geometry\n";
                break;
            }
        }

        OGRFeature::DestroyFeature(poFeature);
    }

    GDALClose(poDS);
}


void MainWindow::on_pushButton_3_clicked()
{
    //    LIPMapScene *scene= new LIPMapScene();
    //    LIPMapScene *calculationScene = new LIPMapScene();
    //    connect(scene,SIGNAL(pos_changed(QPointF)),this,SLOT(scenePos(QPointF)));
    //    connect(scene,SIGNAL(scene_dragging(QPointF,QPointF)),this,SLOT(changeExtent(QPointF,QPointF)));
    //    ui->graphicsView->setScene(scene);
    //    ui->graphicsView->setTransformationAnchor( QGraphicsView::AnchorUnderMouse );
    //    ui->graphicsView->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::HighQualityAntialiasing);

    QString fileName=QFileDialog::getOpenFileName(this,"","");
    QByteArray bytea=fileName.toLocal8Bit();
    const char *charname=bytea.data();

    LIPVectorReader reader(LIPTypes::LIPDrivers::ESRIShapefile, charname);
    //reader.moveToThread(thread);


    reader.ReadGeometry();
    ui->graphicsView->scene()->setSceneRect(-180,90,360,-180);

    //scene->setSceneRect(-98292019,89852960,704459722-98292019, -453620743-89852960);
    //ui->graphicsView->fitInView(-180,90,360,-180);
    ui->graphicsView->setScene(ui->graphicsView->scene());
    ui->graphicsView->scale(1,-1);

    //LIPPointLayer *l=reader.returnLayer();

    QRectF rect=reader.ReadBoundingBox();
    QVector<QGraphicsItem*> items;
    //LIPPointLayer *layer = new LIPPointLayer;
    QPainter *painter=new QPainter(ui->graphicsView);
    LIPLayerTreeModel *model=new LIPLayerTreeModel();
    //ui->treeView->setModel(model);
    QStringList cols;
    cols << "2" << "1";
    model->setColumns(cols);
    QObject* item1 = new QObject();
    item1->setObjectName("Point Layer");
    item1->setProperty("x", "d");
    model->addItem(item1,QModelIndex());
    QObject* item2 = new QObject();
    item2->setObjectName("Point Layer");
    item2->setProperty("2","Vector layer 1");
    //    for (int i=0;i<l->struct_size();i++)
    //    {
    //        //layer->addPoint(l->pointAt(i));
    //        l->pointAt(i)->draw(painter);
    //        QList<QVariant> list;
    //        pVect.append(l->pointAt(i));
    //        list.append(l->pointAt(i)->x());
    //        list.append(l->pointAt(i)->y());
    //        LIPLayerTreeItem item(list);
    //        //test(QPointF(l->pointAt(i)->x(),l->pointAt(i)->y()));

    //        //break;
    //        //        QObject* item1 = new QObject();
    //        //        item1->setObjectName("Point Layer");
    //        //        item1->setProperty("2", l->pointAt(i)->x());

    //        //        //Item2 (parent: item1)
    //        //        QObject* item2 = new QObject(item1);
    //        //        item2->setObjectName("Point");
    //        //        item2->setProperty("1", l->pointAt(i)->y());

    //        model->addItem(item1, ui->treeView->currentIndex());
    //        model->addItem(item2, ui->treeView->currentIndex());


    //        LIPPointGraphicsItem * itemp = new LIPPointGraphicsItem;

    //        QGraphicsEllipseItem *el=new QGraphicsEllipseItem;
    ////        el->setFlag(QGraphicsItem::ItemIsMovable,false);
    ////        el->setFlag(QGraphicsItem::ItemIsSelectable, false);
    ////        el->setFlag(QGraphicsItem::ItemIsFocusable, false);
    ////        el->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    ////        el->setRect(l->pointAt(i)->x(),l->pointAt(i)->y(),3.77*5,3.77*5);
    ////        itemp->setPoint(l->pointAt(i));
    ////        itemp->setBoundingRect(rect);
    ////        QPen pen;

    //        //pen.setWidthF(0.05);

    //        //el->setPen(pen);
    //        el->setBrush(Qt::blue);

    //        ui->graphicsView->scene()->addItem(el);
    //        //scene->addItem(el);



    //        //scene->addItem(item);
    //        //item->setPos(l->pointAt(i)->x(),l->pointAt(i)->y());
    //        //        qDebug()<<l->pointAt(i);

    //    }
    //    QPixmap rendered(ui->graphicsView->viewport()->rect().width(),ui->graphicsView->viewport()->rect().height());  // Create the image with the exact size of the shrunk scene
    //    rendered.fill(Qt::transparent);
    //    QPainter *paintr = new QPainter(&rendered);
    //    //calculationScene->render(paintr);
    //    rendered.save("/home/doger/Pictures/abf1.png");
    //    ui->graphicsView->scene()->addPixmap(rendered);

    //    //ui->graphicsView->scene()->addPixmap(QPixmap::fromImage(img2));
    //    QGraphicsPixmapItem *item = new QGraphicsPixmapItem(pix);
    //    //scene->addItem(item);
    //    item->setPos(0,0);
    //    item->show();
    //    ui->graphicsView->scene()->update();
    //    //scene->addItem(layer);
    //    LIPAddingItems *add=new LIPAddingItems(ui->graphicsView->scene(),items);
    //    connect(this, SIGNAL(start_add()),add,SLOT(start()));
    //    QThread *thread1 = new QThread();
    //    //thread1->start();
    //    //add->moveToThread(thread1);
    //    //thread1->start(); //getting errors but work ok
    //    emit start_add();

    //    //ui->graphicsView->fitInView(43,56,3,3);

    //    fileName=QFileDialog::getOpenFileName(this,"","");
    //    bytea=fileName.toLocal8Bit();
    //    charname=bytea.data();

    //    GDALDataset* dataset = (GDALDataset*) GDALOpen(charname, GA_ReadOnly);
    //    if (dataset == NULL) {
    //        // Обработка ошибки открытия файла
    //    }

    //    // Получение информации о размере изображения
    //    int width = dataset->GetRasterXSize();
    //    int height = dataset->GetRasterYSize();

    //    double geoTransform[6];
    //    dataset->GetGeoTransform(geoTransform);

    //    const char* targetProjection = "EPSG:4326";
    //    const char* resamplingMethod = "bilinear";

    //    // Создание объекта gdalwarp

    //    GDALWarpOptions* warpOptions = GDALCreateWarpOptions();
    //    warpOptions->hSrcDS = dataset;
    //    warpOptions->hDstDS = NULL;
    //    warpOptions->nBandCount = 1;
    //    warpOptions->panSrcBands = (int*) CPLMalloc(sizeof(int));
    //    warpOptions->panSrcBands[0] = 1;
    //    warpOptions->panDstBands = (int*) CPLMalloc(sizeof(int));
    //    warpOptions->panDstBands[0] = 1;
    //    warpOptions->papszWarpOptions = CSLDuplicate(NULL);
    //    CSLAddString(warpOptions->papszWarpOptions, "-overwrite");
    //    warpOptions->papszWarpOptions = CSLSetNameValue(warpOptions->papszWarpOptions,
    //                                                    "SRC_SRS", dataset->GetProjectionRef());
    //    warpOptions->papszWarpOptions = CSLSetNameValue(warpOptions->papszWarpOptions,
    //                                                    "DST_SRS", targetProjection);
    //    warpOptions->papszWarpOptions = CSLSetNameValue(warpOptions->papszWarpOptions,
    //                                                    "RESAMPLING", resamplingMethod);

    //    // Создание нового GeoTIFF-файла с перепроектированным растровым изображением
    //    dataset= (GDALDataset*) GDALAutoCreateWarpedVRT(dataset, dataset->GetProjectionRef(),
    //                                                    targetProjection, GRA_Bilinear, 0.0, NULL);
    //    // Установка координатных системы

    //    //ui->graphicsView->setTransform(transform);

    //    // Чтение данных изображения
    //    GDALRasterBand* rasterBand = dataset->GetRasterBand(1);
    //    std::vector<uint16_t> data(width * height);
    //    rasterBand->RasterIO(GF_Read, 0, 0, width, height, &data[0], width, height, GDT_UInt16, 0, 0);


    //    // Создание графической сцены и добавление элементов

    //    for (int y = 0; y < height; y++) {
    //        for (int x = 0; x < width; x++) {
    //            uint16_t pixelValue = data[y * width + x];
    //            // Создание элемента QGraphicsRectItem с цветом, соответствующим значению пикселя
    //            QColor color(pixelValue >> 8, pixelValue & 0xFF, 0);
    //            QGraphicsRectItem* item = new QGraphicsRectItem(x, y, 1, 1);
    //            item->setBrush(QBrush(color));
    //            //ui->graphicsView->scene()->addItem(item);
    //        }
    //    }

    //    // Создание и настройка виджета QGraphicsView


    //    // Отображение виджета
    //    //view.show();

    //    // Освобождение памяти
    //    GDALClose(dataset);


}

void MainWindow::scenePos(QPointF p)
{
    QString xCoord = QString::number(p.x(), 'f', 2); // Форматируем координату x с двумя знаками после запятой
    QString yCoord = QString::number(p.y(), 'f', 2); // Форматируем координату y с двумя знаками после запятой

    QString coords = QString("%1  %2").arg(xCoord, yCoord); // Соединяем координаты в одну строку с пробелом между ними

    ui->lineEdit->setText(coords);
    //ui->lineEdit->setText(QString::number(p.x()) + " "+QString::number(p.y()));
    //ui->graphicsView->centerOn(p);
}

void MainWindow::recalculateScale()
{
    //    LIPMapCalculations *calculator = new LIPMapCalculations();
    //    calculator->setDpi(QGuiApplication::primaryScreen()->logicalDotsPerInch());
    //    QMatrix const matrix = ui->graphicsView->matrix().inverted();

    //    QRectF visibleRect = matrix.mapRect(ui->graphicsView->viewport()->rect());
    //    visibleRect.moveTopLeft(matrix.map(QPoint(ui->graphicsView->horizontalScrollBar()->value(),
    //                                              ui->graphicsView->verticalScrollBar()->value())));
    //    qDebug()<<visibleRect.width();
    //    double scale = calculator->calculate(visibleRect, ui->graphicsView->width());
    //    ui->lineEdit_2->setText(QString::number(static_cast<int>(scale)));
    //    qDebug()<<"map scale is "+ QString::number(scale);
    //    delete calculator;

    //TODO !!!change to QRectF!!!
    LIPMapCalculations *calculator = new LIPMapCalculations();
    calculator->setDpi(QGuiApplication::primaryScreen()->logicalDotsPerInch());
    QMatrix const matrix = ui->graphicsView->matrix().inverted();

    QRectF visibleRect = matrix.mapRect((ui->graphicsView->viewport()->rect()));
    visibleRect.moveTopLeft(matrix.map(QPoint(ui->graphicsView->horizontalScrollBar()->value(),
                                              ui->graphicsView->verticalScrollBar()->value())));

    QRectF visibleRect1 = ui->graphicsView->mapToScene(ui->graphicsView->viewport()->rect()).boundingRect();
    qDebug()<<visibleRect;
    qDebug()<<"extent";
    qDebug()<<visibleRect1;
    qDebug()<<ui->graphicsView->width();
    double scale = calculator->calculate(visibleRect1, ui->graphicsView->width());
    //scale=calculator->calculate(scene.)
    ui->lineEdit_2->setText(QString::number(static_cast<int>(scale)));
    qDebug()<<"map scale is "+ QString::number(scale);
    delete calculator;
}

void MainWindow::changeExtent(QPointF clickPos, QPointF curPos)
{
    //    QMatrix const matrix = ui->graphicsView->matrix().inverted();
    //    QRectF visibleRect = matrix.mapRect(ui->graphicsView->viewport()->rect());
    //    visibleRect.moveTopLeft(matrix.map(QPointF(ui->graphicsView->horizontalScrollBar()->value(),
    //                                            ui->graphicsView->verticalScrollBar()->value())));
    QRectF visibleRect=getSceneRect();
    double x=-(visibleRect.x()+curPos.x()-clickPos.x());
    double y=visibleRect.y()+curPos.y()-clickPos.y();
    QRectF rect(x,y,visibleRect.width(),visibleRect.height());
    qDebug()<<clickPos;
    qDebug()<<curPos;
    qDebug()<<QString::number(rect.x())+" "+QString::number(rect.y())+" "+QString::number(rect.width())+" "+QString::number(rect.height());
    //ui->textEdit->append(QString::number(rect.x())+" "+QString::number(rect.y())+" "+QString::number(rect.width())+" "+QString::number(rect.height()));

    //ui->graphicsView->scale(1,-1);
    //    QRectF G=ui->graphicsView->mapToScene(ui->graphicsView->rect()).boundingRect();
    //    qDebug()<<G.width();


    //ui->graphicsView->fitInView(rect);
    //ui->graphicsView->setSceneRect(rect);
    //ui->graphicsView->setSceneRect(rect);
    //ui->graphicsView->centerOn(ui->graphicsView->mapFromScene(curPos.toPoint()));
}

QRectF MainWindow::getSceneRect()
{
    return ui->graphicsView->mapToScene(ui->graphicsView->rect()).boundingRect();
}

//void MainWindow::showContextMenu(QPoint p)
//{
//    QPoint globalPos;
//    // если запрос от QAbstractScrollArea
//    if (sender()->inherits("QTreeView"))
//        globalPos = (qobject_cast<QTreeView*>(sender())->viewport()->mapToGlobal(p));
//    // если от других виджетов
//    else
//        globalPos = ui->treeView->mapToGlobal(p);

//    QMenu menu;
//    // Создаем пункт меню
//    QAction* action1 = new QAction(QString::fromUtf8("Пункт 1"), this);
//    // добавляем пункт в меню
//    menu.addAction(action1);
//    // добавляем разделитель
//    menu.addSeparator();
//    // добавляем еще один пункт меню
//    QAction* action2 = new QAction(QString::fromUtf8("Пункт 2"), this);
//    menu.addAction(action2);
//    menu.show();
//    menu.exec(globalPos);
//}



void MainWindow::test(QPointF point)
{
    const int dot_per_millimeter = qRound(qApp->primaryScreen()->physicalDotsPerInch() / 25.40);
    QPen mPen;
    QBrush mBrush;
    mBrush.setColor(Qt::blue);
    mBrush.setStyle(Qt::SolidPattern);
    mPen.setWidthF(1.0);
    mPen.setColor(Qt::blue);
    double scaledSize = 4.0;

    // calculate necessary image size for the cache
    double pw = (( mPen.widthF() == 0 ? 1 : mPen.widthF() ) + 1 ) / 2 * 2; // make even (round up); handle cosmetic pen
    int imageSize = (( int ) scaledSize + pw ) / 2 * 2 + 1; //  make image width, height odd; account for pen width

    double center = (( double ) imageSize / 2 ) + 0.5; // add 1/2 pixel for proper rounding when the figure's coordinates are added

    QImage mCache = QImage( QSize( 30, 30 ), QImage::Format_ARGB32_Premultiplied );
    mCache.fill( 0 );
    double sdg=mCache.devicePixelRatio();
    //mCache.setDevicePixelRatio(200);
    QPainter p;
    p.begin( &mCache );
    p.setRenderHint( QPainter::Antialiasing );
    p.setBrush( mBrush );




    //p.translate( QPointF( center, center ) );

    int dpm = 1000 / 0.0254; // ~300 DPI
    //кол-во точек в 1 мм
    //mPen.setWidthF(mm * 10); //
    p.setPen(5);
    mCache.setDotsPerMeterX(dpm);
    mCache.setDotsPerMeterY(dpm);
    qDebug()<<mCache.physicalDpiX();


    //допустим 10 милиметров


    //pixel to mm
    const int dots_per_millimeter = qRound(qApp->primaryScreen()->physicalDotsPerInch() / 25.40);

    p.drawEllipse(QRectF(0,0,3.77*5,3.77*5));
    //    if (mCache.save("/home/doger/Pictures/abf.png"))
    //    {
    //        QMessageBox::warning(this,"","");
    //    }

    pa.drawImage(QPointF(point.x()-mCache.width()/2,point.y()-mCache.height()/2),mCache);

    ui->graphicsView->scene()->update();


    // Construct the selected version of the Cache




    // Check that the selected version is different.  If not, then re-render,
    // filling the background with the selection color and using the normal
    // colors for the symbol .. could be ugly!





}

void MainWindow::wheelEvent(QWheelEvent *event)
{
    //    if (event->angleDelta().y()<0 )
    //    {
    //        scale_factor=0.3;

    //        ui->graphicsView->scale(scale_factor, scale_factor);
    //    }
    //    else if (event->angleDelta().y()>0) {
    //        scale_factor=2;
    //        ui->graphicsView->scale(scale_factor, scale_factor);
    //    }


    //ui->graphicsView->setSceneRect(0,0,12,12);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //ui->graphicsView->setResizeAnchor(QGraphicsView::AnchorViewCenter );

    recalculateScale();
    qDebug()<<ui->graphicsView->transform().m11();
    //ui->graphicsView->setFocusPolicy( Qt::NoFocus );
    //        ui->graphicsView->scene()->clear();
    //        pa.end();


    //ui->graphicsView->scene()->clear();

    for (int i=0; i<pVect.size(); i++)
    {
        LIPPoint *l=new LIPPoint;
        l=pVect.at(i);
        //ui->graphicsView->setInteractive(false);
        QGraphicsEllipseItem *el=new QGraphicsEllipseItem;
        int scaleF=ui->graphicsView->transform().m11();
        el->setFlag(QGraphicsItem::ItemIsMovable,false);
        el->setFlag(QGraphicsItem::ItemIsSelectable, false);
        el->setFlag(QGraphicsItem::ItemIsFocusable, false);
        el->setFlag(QGraphicsItem::ItemClipsToShape, false);
        el->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
        el->setRect(l->x(),l->y(),3.77*5/scaleF,3.77*5/scaleF);

        QPen pen;

        pen.setWidthF(0);

        el->setPen(pen);
        el->setBrush(Qt::blue);

        ui->graphicsView->scene()->addItem(el);
    }
    QTransform tr(1, 0, 0, 1, 50.0, 50.0);
    QMatrix const matrix = ui->graphicsView->matrix().inverted();
    QRect visibleRect = matrix.mapRect(ui->graphicsView->viewport()->rect());
    //передача масштабы сцены для отрисовки
    emit scaleFactorChanged(ui->graphicsView->transform().m11());


    //        img2 = QImage(QSize(visibleRect.width(),visibleRect.height()), QImage::Format_ARGB32_Premultiplied);
    //        pa.begin(&img2);
    //        //pa.setViewport(visibleRect);
    //        for(int i=0;i<pVect.size();i++)
    //        {
    //            test(QPointF(pVect.at(i)->x(),pVect.at(i)->y()));

    //        }
    //        ui->graphicsView->scene()->addPixmap(QPixmap::fromImage(img2));
}




//void MainWindow::on_treeView_clicked(const QModelIndex &index)
//{
//    //QMessageBox::information(this,"",QString::number(index.row()));
//    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);
//    connect(ui->treeView, SIGNAL(customContextMenuRequested(QPoint)),
//            this, SLOT(showContextMenu(QPoint)));


//    QMessageBox::information(this,"",layerModel->data(ui->treeView->selectionModel()->selectedIndexes().at(0), 0).toString());
//}


void MainWindow::on_pushButton_GeoTiff_clicked()
{
    GDALAllRegister();
    QString fileName=QFileDialog::getOpenFileName(this,"","");
    QByteArray bytea=fileName.toLocal8Bit();
    const char *charname=bytea.data();
    GDALAllRegister();
    GDALDataset* dataset = static_cast<GDALDataset*>(GDALOpen(charname, GA_ReadOnly));

    // Проверка успешной загрузки файла
    if (dataset == nullptr) {
        // Обработка ошибки
        return;
    }

    // Получение ширины и высоты изображения
    int width = dataset->GetRasterXSize();
    int height = dataset->GetRasterYSize();

    // Получение преобразования геопривязки
    double geoTransform[6];
    if (dataset->GetGeoTransform(geoTransform) != CE_None) {
        // Обработка ошибки
        GDALClose(dataset);
        return;
    }


    // Чтение данных растрового изображения
    for (int i=0; i<dataset->GetRasterCount(); i++)
    {
        if (!dataset->GetRasterBand(i+1))
            continue;
        qDebug()<<dataset->GetRasterBand(i+1)->GetColorInterpretation();
        switch (dataset->GetRasterBand(i+1)->GetColorInterpretation())
        {
        case GCI_RedBand:
        {
            QMessageBox::information(this,"","red band detected");
        }
        case GCI_GreenBand:
        {
            QMessageBox::information(this,"","green band detected");
        }
        case GCI_BlueBand:
        {
            QMessageBox::information(this,"","blue band detected");
        }
        }


    }




    GDALRasterBand* band_1 = dataset->GetRasterBand(1);
    GDALRasterBand* band_2 = dataset->GetRasterBand(2);
    GDALRasterBand* band_3 = dataset->GetRasterBand(3);
    if (!band_1 || !band_2 || !band_3)
        return;
    int numPixels=width*height;
    ushort* bufferRed = new ushort[numPixels];
    ushort* bufferGreen = new ushort[numPixels];
    ushort* bufferBlue = new ushort[numPixels];

    band_1->RasterIO(GF_Read, 0, 0, width, height, bufferRed, width, height, GDT_UInt16, 0, 0);
    qDebug()<<"test1";
    unsigned char* greenData = new unsigned char[numPixels];
    band_2->RasterIO(GF_Read, 0, 0, width, height, bufferGreen, width, height, GDT_UInt16, 0, 0);
    qDebug()<<"test2";
    unsigned char* blueData = new unsigned char[numPixels];
    band_3->RasterIO(GF_Read, 0, 0, width, height, bufferBlue, width, height, GDT_UInt16, 0, 0);
    //    qDebug()<<"test3";
    QImage mergedImage(width, height, QImage::Format_ARGB32_Premultiplied);


    double minVal3, maxVal3;

    double* minmax3 = new double[1];
    band_3->ComputeRasterMinMax(false, minmax3);
    minmax3[0]=7000;
    minmax3[1]=10000;
    double* minmax2 = new double[1];
    band_2->ComputeRasterMinMax(false, minmax2);
    minmax2[0]=7000;
    minmax2[1]=10000;
    double* minmax1 = new double[1];
    band_1->ComputeRasterMinMax(false, minmax1);
    minmax1[0]=7000;
    minmax1[1]=10000;
    double minVal2, maxVal2;
    //band_2->ComputeRasterMinMax(minVal2, &maxVal2);
    double minVal1, maxVal1;
    //band_1->ComputeRasterMinMax(minVal1, &maxVal1);

    minVal1=band_1->GetMinimum();
    maxVal1=band_1->GetMaximum();
    qDebug()<<minVal1;
    qDebug()<<maxVal1;
    minVal2=band_2->GetMinimum();
    maxVal2=band_2->GetMaximum();

    minVal3=band_3->GetMinimum();
    maxVal3=band_3->GetMaximum();



    int numPixels2 = width*height;
    for (int y = 0; y < numPixels; y++) {


        auto pixelValue1 = bufferRed[y];

        //qDebug()<<pixelValue1;
        auto pixelValue2 = bufferGreen[y];
        auto pixelValue3 = bufferBlue[y];

        int normalizedBrightness1 = (pixelValue1 - minmax1[0]) * 255 / (minmax1[1] - minmax1[0]);
        int normalizedBrightness2 = (pixelValue2 - minmax2[0]) * 255 / (minmax2[1] - minmax2[0]);
        int normalizedBrightness3 = (pixelValue3 - minmax3[0]) * 255 / (minmax3[1] - minmax3[0]);
        if (normalizedBrightness1>255)
            normalizedBrightness1=255;
        if (normalizedBrightness2>255)
            normalizedBrightness2=255;
        if (normalizedBrightness3>255)
            normalizedBrightness3=255;
        QRgb color = qRgb(normalizedBrightness1, normalizedBrightness2, normalizedBrightness3);
        mergedImage.setPixel(y % width, y / width, color);

    }
    //    float* buffer = new float[width * height];
    //    band_3->RasterIO(GF_Read, 0, 0, width, height, buffer, width, height, GDT_Float32, 0, 0);

    //    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    //    ui->graphicsView->setRenderHint(QPainter::SmoothPixmapTransform);

    // Преобразование данных растрового изображения в QPixmap
    //mergedImage=QImage();

    //    for (int y = 0; y < height; y++) {
    //            for (int x = 0; x < width; x++) {
    //                float pixelValue_3;
    //                float pixelValue_2;
    //                float pixelValue_1;
    //                dataset->GetRasterBand(3)->RasterIO(GF_Read, x, y, 1, 1, &pixelValue_3, 1, 1, GDT_Float32, 0, 0);
    //                dataset->GetRasterBand(2)->RasterIO(GF_Read, x, y, 1, 1, &pixelValue_2, 1, 1, GDT_Float32, 0, 0);
    //                dataset->GetRasterBand(1)->RasterIO(GF_Read, x, y, 1, 1, &pixelValue_1, 1, 1, GDT_Float32, 0, 0);
    //                // Нормализация значения пикселя
    //                int colorValue1 = 255 * (pixelValue_1 - minVal1) / (maxVal1 - minVal1);
    //                int colorValue2 = 255 * (pixelValue_2 - minVal2) / (maxVal2 - minVal2);
    //                int colorValue3 = 255 * (pixelValue_3 - minVal3) / (maxVal3 - minVal3);
    //                QRgb color = qRgb(colorValue1, colorValue2, colorValue3); // Простейший пример преобразования в градации серого

    //                mergedImage.setPixel(x, y, color);
    //            }
    //        }






    // mergedImage.save("/home/doger/Documents/gisTest/Raster/img1.png");
    //    int nBands = dataset->GetRasterCount();

    //    for(int b=0; b < nBands; b++)
    //    {
    //        GDALRasterBand *band = dataset->GetRasterBand(b+1);
    //        if(band != nullptr)
    //        {
    //            CPLErr error = band->RasterIO(GF_Read, 0, 0, mergedImage.width(), mergedImage.height(),
    //                                          mergedImage.bits() + b, mergedImage.width(), mergedImage.height(), GDT_Byte, nBands, 0);
    //            if(error != CE_None)
    //            {
    //                // REPORT ERROR
    //            }
    //        }
    //    }

    //    float* buffer = new float[width * height];
    //    band->RasterIO(GF_Read, 0, 0, width, height, buffer, width, height, GDT_Float32, 0, 0);

    // Преобразование данных растрового изображения в QPixmap
    //QImage image(reinterpret_cast<unsigned char*>(buffer), width, height, QImage::Format_RGB32);
    QPixmap pixmap = QPixmap::fromImage(mergedImage);

    // Создание QGraphicsPixmapItem с преобразованием геопривязки
    QGraphicsPixmapItem* pixmapItem = scene->addPixmap(pixmap);

    //pixmapItem->setOffset(geoTransform[0], geoTransform[3]);
    QTransform transform;
    transform.translate(geoTransform[0], geoTransform[3]);
    transform.scale(geoTransform[1], geoTransform[5]);
    pixmapItem->setTransform(transform);

    // Создание QGraphicsView для отображения QGraphicsScene











    //    GDALDataset* dataset = (GDALDataset*) GDALOpen(charname, GA_ReadOnly);
    //    GDALRasterBand  *poBand;
    //    int             nBlockXSize, nBlockYSize;
    //    int             bGotMin, bGotMax;
    //    double          adfMinMax[2];
    //    poBand = dataset->GetRasterBand( 1 );
    //    poBand->GetBlockSize( &nBlockXSize, &nBlockYSize );
    //    adfMinMax[0] = poBand->GetMinimum( &bGotMin );
    //    adfMinMax[1] = poBand->GetMaximum( &bGotMax );
    //    if( ! (bGotMin && bGotMax) )
    //    GDALComputeRasterMinMax((GDALRasterBandH)poBand, TRUE, adfMinMax);float *pafScanline;
    //    int   nXSize = poBand->GetXSize();
    //    int   nYSize = poBand->GetYSize();
    //    pafScanline = (float *) CPLMalloc(sizeof(float)*nXSize * nYSize);
    //    poBand->RasterIO( GF_Read, 0, 0, nXSize, nYSize,
    //    pafScanline, nXSize, nYSize, GDT_Float32, 0, 0 );

    //    QImage image((unsigned char*)pafScanline, nXSize, nYSize, QImage::Format_RGB32);

    //    image.save("blaa.jpg");

    //    // Создание QGraphicsScene и QGraphicsView


    //    // Создание QGraphicsPixmapItem и добавление его на сцену
    //    QPixmap pixmap = QPixmap::fromImage(image);
    //    QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(pixmap);
    //    pixmapItem->setPos(0,0);
    //    ui->graphicsView->scene()->addItem(pixmapItem);

}


void MainWindow::on_pushButton_4_clicked() //create shp test
{
    const char *pszDriverName = "ESRI Shapefile";
    GDALAllRegister();
    OGRRegisterAll();
    GDALDriver *poDriver = OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName(pszDriverName);
    GDALDataset *poDs = poDriver->Create("32.shp", 0, 0, 0, GDT_Unknown, NULL);
    OGRLayer *poLayer = poDs->CreateLayer("roadmarking", NULL, wkbPoint, NULL);
    if (poLayer == NULL)
    {
        //cout << "Layer creation failed!" << endl;
    }
    else
    {
    }
    //cout << "Creating Layer done" << endl;

    // Create a property field
    OGRFieldDefn poFieldID("ID", OFTInteger);
    poFieldID.SetWidth(40);
    OGRFieldDefn poFieldType("Type", OFTInteger);
    poFieldType.SetWidth(40);
    if (poLayer->CreateField(&poFieldID) != OGRERR_NONE) {
        //cout << "ID field creation failed" << endl;
    }
    if (poLayer->CreateField(&poFieldType) != OGRERR_NONE) {
        //cout << "Type field creation failed" << endl;
    }

    // Picture
    int line_N = 90;
    OGRMultiPoint point;
    for (int i = 0; i < line_N; i++) {
        OGRFeature *poFeature = OGRFeature::CreateFeature(poLayer->GetLayerDefn());
        poFeature->SetField("ID", i);
        poFeature->SetField("Type", i);

        OGRPoint p(i,i);
        point.addGeometry(&p);
        poFeature->SetGeometry(&p);

        if (poLayer->CreateFeature(poFeature) != OGRERR_NONE) {
            //cout << "Failed to create feature in shapefile" << endl;
        }
        OGRFeature::DestroyFeature(poFeature);
    }
    GDALClose(poDs);


}





void MainWindow::on_actionNew_point_layer_triggered()
{
    LIPNewLineLayerForm *layerForm = new LIPNewLineLayerForm(nullptr, LIPGeometryType::LIPPoint);
    layerForm->exec();
    LIPPointLayer* newLayer= dynamic_cast<LIPPointLayer*>(layerForm->returnLayer());
    if (newLayer==nullptr)
    {
        LIPWidgetManager::getInstance().showMessage("Ошибка при создании векторного слоя", 2000, messageStatus::Error);
        return;
    }
    //connect(this, SIGNAL(scaleFactorChanged(double)), new_layer, SLOT(setSceneScaleFactor(double)));
    //    QString name=new_layer->returnGISName();
    //    QString fileName = new_layer->getFileName();


    //    emit newVectorLayer(new_layer);
    //    LIPTreeWidgetItem *item = new LIPTreeWidgetItem();
    //    item->setText(0,name);
    //    item->setCheckState(0,Qt::Checked);
    //    item->setIcon(0,QIcon(":/ui/icons/pointLayer.png"));
    //    item->setFlags(item->flags() | Qt::ItemIsDragEnabled );
    //    item->setToolTip(0,fileName);
    //    ui->LayerTree->addTopLevelItem(item);
    //    LIPProject::getInstance().addVectorLayer(new_layer);


}


void MainWindow::on_actionNew_line_layer_triggered() //при нажатии на кнопку создания линейного слоя
{
    LIPNewLineLayerForm *layerForm = new LIPNewLineLayerForm(nullptr, LIPGeometryType::LIPLineString);
    layerForm->exec();
    //LIPVectorLayer *vect = layerForm->returnLayer();
    LIPLineLayer* newLayer= dynamic_cast<LIPLineLayer*>(layerForm->returnLayer());
    if (newLayer==nullptr)
    {
        LIPWidgetManager::getInstance().showMessage("Ошибка при создании векторного слоя", 2000, messageStatus::Error);
        return;
    }
    //TODO добавить этот код в конструктор LIPVectorLayer
    //    connect(this, SIGNAL(scaleFactorChanged(double)), newLayer, SLOT(setSceneScaleFactor(double)));
    //    QString name=newLayer->returnGISName();
    //    QString fileName = newLayer->getFileName();

    //    emit newVectorLayer(newLayer);
    //    LIPTreeWidgetItem *item = new LIPTreeWidgetItem();
    //    item->setText(0,name);
    //    item->setCheckState(0,Qt::Checked);
    //    item->setIcon(0,QIcon(":/ui/icons/lineLayer.png"));
    //    item->setFlags(item->flags() | Qt::ItemIsDragEnabled );
    //    item->setToolTip(0,fileName);
    //    ui->LayerTree->addTopLevelItem(item);
    //    LIPProject::getInstance().addVectorLayer(newLayer);



}


void MainWindow::on_actionNew_polygon_layer_triggered()
{
    LIPNewLineLayerForm *layerForm = new LIPNewLineLayerForm(nullptr, LIPGeometryType::LIPPolygon);
    layerForm->exec();

    LIPPolygonLayer* newLayer= dynamic_cast<LIPPolygonLayer*>(layerForm->returnLayer());
    if (newLayer==nullptr)
    {
        LIPWidgetManager::getInstance().showMessage("Ошибка при создании векторного слоя", 2000, messageStatus::Error);
        return;
    }

    //    connect(this, SIGNAL(scaleFactorChanged(double)), new_layer, SLOT(setSceneScaleFactor(double)));
    //    QString name=new_layer->returnGISName();
    //    QString fileName = new_layer->getFileName();
    //    emit newVectorLayer(new_layer);


    //    LIPTreeWidgetItem *item = new LIPTreeWidgetItem();
    //    item->setText(0,name);
    //    item->setCheckState(0,Qt::Checked);
    //    item->setIcon(0,QIcon(":/images/icons/polygonLayer.png"));
    //    item->setFlags(item->flags() | Qt::ItemIsDragEnabled );
    //    item->setToolTip(0,fileName);
    //    ui->LayerTree->addTopLevelItem(item);
    //    LIPProject::getInstance().addVectorLayer(new_layer);

}


void MainWindow::on_actionLoad_vector_layer_triggered()
{
    QString fileName=QFileDialog::getOpenFileName(this,"","");
    auto readPair=LIPVectorReader::readOGRLayer(fileName);
    OGRLayer* newLayer = readPair.first;
    GDALDataset* dS = readPair.second;
    if (newLayer==nullptr || dS==nullptr)
        return;
    int startIndex = fileName.lastIndexOf('/') + 1; // Находим индекс символа '/' и добавляем 1, чтобы пропустить его
    int endIndex = fileName.lastIndexOf(".");
    QString name = fileName.mid(startIndex, endIndex - startIndex);
    if (newLayer!=nullptr)
    {
        LIPGeometryType type = LIPVectorReader::readGeometryType(newLayer);
        switch (type)
        {
        case LIPGeometryType::LIPPoint:
        {
            new LIPPointLayer(newLayer,name, fileName, dS);
            break;
        }
        case LIPGeometryType::LIPLineString:
        {
            new LIPLineLayer(newLayer,name, fileName, dS);
            break;
        }
        case LIPGeometryType::LIPPolygon:
        {
            new LIPPolygonLayer(newLayer,name, fileName, dS);
            break;
        }
        }
    }


}


void MainWindow::on_pushButton_addPointFeature_clicked()
{

}

void MainWindow::layersOrderChanged() //срабатывает при перемещении элементов QTreeWidgetItem
{

    for (int i=0; i<ui->LayerTree->topLevelItemCount(); i++)
    {
        //убираем "вложенность" элементов
        QTreeWidgetItem *item = ui->LayerTree->topLevelItem(i);
        qDebug()<<item->childCount();
        for (int i=0; i<item->childCount(); i++)
        {
            auto item2=item->child(i);
            item->removeChild(item2);
            ui->LayerTree->addTopLevelItem(item2);

        }

    }
    int k=0;
    for (int i=ui->LayerTree->topLevelItemCount()-1; i>=0; i--)
    {

        //убираем "вложенность" элементов
        QTreeWidgetItem *item = ui->LayerTree->topLevelItem(i);
        QString path = item->toolTip(0); //получаем имя файла
        LIPVectorLayer *layer = LIPProject::getInstance().getVectorLayerByPath(path);
        if (layer!=nullptr)
        {
            layer->setZValue(k);
            k++;
        }
        else
        {
            LIPRasterLayer* rastLayer = LIPProject::getInstance().getRasterLayerByPath(path);
            if (!rastLayer)
                continue;
            rastLayer->setZValue(k);
            k++;

        }


    }


}


void MainWindow::on_LayerTree_itemActivated(QTreeWidgetItem *item, int column) //выбор пользователем активного слоя
{
    //LIPVectorLayer *layer = LIPProject::getInstance().getVectorLayerByPath(item->toolTip(0)); //получаем элемент по пути к файлу
    auto activeLayer = LIPProject::getInstance().getVectorLayerByPath(item->toolTip(0));
    if (activeLayer==nullptr)
        return;
    LIPProject::getInstance().setActiveLayer(activeLayer);

    int topLevelItemCount = ui->LayerTree->topLevelItemCount();
    for (int i = 0; i < topLevelItemCount; ++i) {
        QTreeWidgetItem *topLevelItem = ui->LayerTree->topLevelItem(i);
        topLevelItem->setBackground(0, QBrush());
    }
    item->setBackground(column, QBrush(QColor(Qt::gray)));

    if (LIPVectorTypeChecker::isPointLayer(activeLayer))
    {
        ui->pushButton_addFeature->setIcon(QIcon(":/images/icons/addPointFeature2.png"));
        ui->pushButton_addFeature->setIconSize(QSize(60,60));
    }
    else if (LIPVectorTypeChecker::isLineLayer(activeLayer))
    {
        ui->pushButton_addFeature->setIcon(QIcon(":/images/icons/addLineFeature.png"));
        ui->pushButton_addFeature->setIconSize(QSize(40,40));
    }
    else if (LIPVectorTypeChecker::isPolygonLayer(activeLayer))
    {
        ui->pushButton_addFeature->setIcon(QIcon(":/images/icons/addPolygonFeature.png"));
        ui->pushButton_addFeature->setIconSize(QSize(50,50));
    }

}


void MainWindow::on_pushButton_addPointFeature_clicked(bool checked)
{

    //    if (checked)
    //    {

    //        LIPVectorLayer *tL = LIPProject::getInstance().getActiveLayer();
    //        if (tL!=nullptr)
    //        {
    //            ui->graphicsView->setCursor(Qt::CrossCursor);
    //            scene->startAddingFeatures(tL);
    //            ui->graphicsView->updateAddingFeaturesFlag(true);
    //            return;
    //        }
    //        LIPWidgetManager::getInstance().showMessage("Для добавления новых объектов необходимо выбрать активный слой",
    //                                                    2000, messageStatus::Error);
    //        ui->graphicsView->updateAddingFeaturesFlag(false);
    //    }
    //    else
    //    {
    //        ui->graphicsView->setCursor(Qt::ArrowCursor);
    //        scene->stopAddingFeatures();
    //        ui->graphicsView->updateAddingFeaturesFlag(false);
    //    }
}



bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{

}

void MainWindow::on_actionConnect_to_PostGIS_triggered()
{
    LIPPostGisConnectionForm* form = new LIPPostGisConnectionForm();
    form->exec();
    GDALDataset* dS = form->returnDataSet();
    if (dS==nullptr)
        return;
    LIPProject::getInstance().setActivePostGISConnection(dS);
    QVector<OGRLayer*> vect = LIPVectorReader::readLayersFromDataset(dS);
    for (int i=0; i<vect.count(); i++)
    {

        OGRLayer *newLayer=vect.at(i);
        if (newLayer==nullptr)
        {
            LIPWidgetManager::getInstance().showMessage(tr("Ошибка чтения слоя"), 1000, messageStatus::Error);
            continue;
        }
        LIPGeometryType type = LIPVectorReader::readGeometryType(newLayer);
        switch (type)
        {
            case LIPGeometryType::LIPPoint:
            {
                //ui->DBL
                LIPTreeWidgetItem *item = new LIPTreeWidgetItem();
                item->setText(0, newLayer->GetName());
                //item->setCheckState(0,Qt::Checked);
                item->setFlags(item->flags() | Qt::ItemIsDragEnabled );
                item->setToolTip(0,"postgis_connection/"+QString(newLayer->GetName()));
                ui->DBLayerTree->addTopLevelItem(item);
                item->setIcon(0,QIcon(":/images/icons/pointLayer.png"));
                //new LIPPointLayer(newLayer,newLayer->GetName(), "postgis_connection/"+QString(newLayer->GetName()), dS);
                break;

            }

            case LIPGeometryType::LIPLineString:
            {
                LIPTreeWidgetItem *item = new LIPTreeWidgetItem();
                item->setText(0, newLayer->GetName());
                //item->setCheckState(0,Qt::Checked);
                item->setFlags(item->flags() | Qt::ItemIsDragEnabled );
                item->setToolTip(0,"postgis_connection/"+QString(newLayer->GetName()));
                ui->DBLayerTree->addTopLevelItem(item);
                item->setIcon(0,QIcon(":/images/icons/lineLayer.png"));
                //new LIPLineLayer(newLayer,newLayer->GetName(), "postgis_connection/"+QString(newLayer->GetName()), dS);
                break;
            }
            case LIPGeometryType::LIPPolygon:
            {
                LIPTreeWidgetItem *item = new LIPTreeWidgetItem();
                item->setText(0, newLayer->GetName());
                //item->setCheckState(0,Qt::Checked);
                item->setFlags(item->flags() | Qt::ItemIsDragEnabled );
                item->setToolTip(0,"postgis_connection/"+QString(newLayer->GetName()));
                ui->DBLayerTree->addTopLevelItem(item);
                item->setIcon(0,QIcon(":/images/icons/polygonLayer.png"));
                //new LIPPolygonLayer(newLayer,newLayer->GetName(), "postgis_connection/"+QString(newLayer->GetName()), dS);
                break;
            }
            case LIPGeometryType::LIPUnknown:
            {
                break;
            }
        }
    }

}


void MainWindow::on_pushButtonTriangulationTest_clicked()
{
    //triangulation test

    LIPPointLayer* new_layer= dynamic_cast<LIPPointLayer*>(LIPProject::getInstance().getActiveLayer());
    QVector<QPointF> pl;
    pl = triangulate(new_layer);
    //    QString name = QFileDialog::getSaveFileName(this,"","");
    //    LIPLayerCreator *polyCreator = new LIPLayerCreator(LIPGeometryType::LIPPolygon, name,"triangulatiuons");
    //    LIPPolygonLayer *l = dynamic_cast<LIPPolygonLayer*>(polyCreator->returnLayer());
    QMessageBox::information(this,"","");

    LIPNewLineLayerForm *layerForm = new LIPNewLineLayerForm(nullptr, LIPGeometryType::LIPPolygon);
    layerForm->exec();

    LIPPolygonLayer* poly= dynamic_cast<LIPPolygonLayer*>(layerForm->returnLayer());
    connect(this, SIGNAL(scaleFactorChanged(double)), poly, SLOT(setSceneScaleFactor(double)));
    QString name=poly->returnGISName();
    QString fileName = poly->getFileName();


    LIPTreeWidgetItem *item = new LIPTreeWidgetItem();
    item->setText(0,name);
    item->setCheckState(0,Qt::Checked);
    item->setIcon(0,QIcon(":/images/icons/polygonLayer.png"));

    item->setFlags(item->flags() | Qt::ItemIsDragEnabled );
    item->setToolTip(0,fileName);
    ui->LayerTree->addTopLevelItem(item);
    LIPProject::getInstance().addVectorLayer(poly);



    //    LIPNewLineLayerForm *form = new LIPNewLineLayerForm();
    //    form->exec();
    //    LIPPolygonLayer *l = dynamic_cast<LIPPolygonLayer*>(form->returnLayer());
    //    if (!l)
    //        QMessageBox::warning(this,"","nullprt layer program crash!");

    //    emit newVectorLayer(l);
    //    connect(this, SIGNAL(scaleFactorChanged(double)), l, SLOT(setSceneScaleFactor(double)));
    //    LIPTreeWidgetItem *item = new LIPTreeWidgetItem();
    //    item->setText(0,"");
    //    item->setCheckState(0,Qt::Checked);
    //    item->setIcon(0,QIcon(":/ui/icons/pointLayer.png"));
    //    item->setFlags(item->flags() | Qt::ItemIsDragEnabled );
    //    item->setToolTip(0,"");
    //    ui->LayerTree->addTopLevelItem(item);
    //    LIPProject::getInstance().addVectorLayer(l);


    int i =0;
    while(i<pl.size()-2)
    {
        QVector<QPointF> vec;
        vec.append( QPointF(pl[i]));
        vec.append( QPointF(pl[i+1]));
        vec.append( QPointF(pl[i+2]));
        qDebug()<<pl[i];
        poly->addFeature(vec, QVector<LIPAttribute>());
        i=i+3;
    }
    poly->setMapFeatures();
    scene->drawVectorLayer(poly);
    QVector<LIPTriangle> ti;


}


void MainWindow::on_pushButton_7_clicked()
{
    LIPCutLayerForm* form = new LIPCutLayerForm;
    form->exec();
    if (form->getResult()!=nullptr)
        addLayer(form->getResult());

    //результат обрезки
    //    LIPTriangle tr;
    //    QVector<QPointF> vect;
    //    vect.append(QPointF(10,10));
    //    vect.append(QPointF(20,20));
    //    vect.append(QPointF(10,30));
    //    tr.setVertex(QPointF(10,10), QPointF(20,20), QPointF(10,30));

    //    scene->addPolygon(QPolygonF(vect));

    //    LIPCircle c= tr.getCircumcircle();
    //    qreal left = c.getCenter().x() - c.getRadis();
    //    qreal top = c.getCenter().y() - c.getRadis();
    //    QRectF ellipseRect(left, top, 2*c.getRadis(), 2*c.getRadis());
    //    scene->addEllipse(ellipseRect);
    //    //находим середины сторон
    //    QPointF AB_center=QPointF((tr.getA().x() + tr.getB().x()) / 2, (tr.getA().y() + tr.getB().y()) / 2);
    //    QPointF ВС_center=QPointF((tr.getB().x() + tr.getC().x()) / 2, (tr.getB().y() + tr.getC().y()) / 2);
    //    QPointF AC_center=QPointF((tr.getA().x() + tr.getC().x()) / 2, (tr.getA().y() + tr.getC().y()) / 2);

    //    //находим уравнения прямых
    //    // Находим угловой коэффициент прямой, перпендикулярной AB
    //    double slopeAB = (tr.getB().x() - tr.getA().x()) / (tr.getA().y() - tr.getB().y());
    //    // Находим угловой коэффициент прямой, перпендикулярной BC
    //    double slopeBC = (tr.getC().x() - tr.getB().x()) / (tr.getB().y() - tr.getC().y());

    //    // Находим x-координату центра пересечения перпендикуляров
    //    QPointF centroid;
    //    centroid.setX((slopeAB * AB_center.x() - slopeBC * ВС_center.x() + ВС_center.y() - AB_center.y())/ (slopeAB - slopeBC));
    //    // Находим y-координату центра пересечения перпендикуляров
    //    centroid.setY(slopeAB * (centroid.x() - AB_center.x()) + AB_center.y());




    //    centroid.setX((tr.getA().x()+tr.getB().x()+tr.getC().x())/3);
    //    centroid.setY((tr.getA().y()+tr.getB().y()+tr.getC().y())/3);
    //    scene->addPolygon(QRectF(centroid.x(), centroid.y(), 0.1, 0.1));

    QPointF f(123,1);
    //LIPVectorConvertor::QPointFtoGeosPoint(f);


}


void MainWindow::on_pushButtonVoronoiTest_clicked()
{

    LIPPointLayer* new_layer= dynamic_cast<LIPPointLayer*>(LIPProject::getInstance().getActiveLayer());
    QVector<LIPEdge> vec=generateVoronoiDiagramm(new_layer).first;
    foreach(LIPEdge edg, vec)
    {
        QGraphicsLineItem *line = new QGraphicsLineItem();
        QPen pen;
        pen.setWidthF(0.001);
        line->setPen(pen);
        line->setLine(QLineF(QPointF(edg.x1(),edg.y1()),QPointF(edg.x2(),edg.y2())));
        scene->addItem(line);
    }

    //    QVector<LIPTriangle> vec2=generateVoronoiDiagramm(new_layer).second;
    //    foreach(LIPTriangle tr, vec2)
    //    {
    //        QPen pen;
    //        pen.setWidthF(0.001);
    //        QGraphicsLineItem *line = new QGraphicsLineItem();
    //        line->setPen(pen);
    //        line->setLine(QLineF(tr.getA(),tr.getB()));
    //        scene->addItem(line);

    //        QGraphicsLineItem *line2 = new QGraphicsLineItem();
    //        line2->setPen(pen);
    //        line2->setLine(QLineF(tr.getB(),tr.getC()));
    //        scene->addItem(line2);

    //        QGraphicsLineItem *line3 = new QGraphicsLineItem();
    //        line3->setPen(pen);
    //        line3->setLine(QLineF(tr.getA(),tr.getC()));
    //        scene->addItem(line3);

    //    }



    //    QVector<LIPCircle> vecC = getCircumCircles();
    //    foreach(LIPCircle circ, vecC)
    //    {
    //        QPen pen;
    //        pen.setWidthF(0.0001);
    //        QGraphicsEllipseItem *ell = new QGraphicsEllipseItem();
    //        ell->setPen(pen);
    //        ell->setRect(circ.getCenter().x()-circ.getRadis(), circ.getCenter().y()-circ.getRadis(), circ.getRadis()*2,
    //                     circ.getRadis()*2);
    //        scene->addItem(ell);
    //    }

    //    QVector<QPair<LIPCircle, LIPCircle>> pr=GetPair();
    //    for (int i=0; i<pr.count(); i++)
    //    {
    //        LIPCircle circ=pr.at(i).first;
    //        LIPCircle circ2=pr.at(i).second;
    //        QPen pen;
    //        pen.setWidthF(0.01);
    //        pen.setColor(QColor::fromRgb(QRandomGenerator::global()->bounded(0, 256),
    //                                       QRandomGenerator::global()->bounded(0, 256),
    //                                       QRandomGenerator::global()->bounded(0, 256)));
    //        QGraphicsEllipseItem *ell = new QGraphicsEllipseItem();
    //        ell->setPen(pen);
    //        ell->setRect(circ.getCenter().x()-circ.getRadis(), circ.getCenter().y()-circ.getRadis(), circ.getRadis()*2,
    //                     circ.getRadis()*2);
    //        scene->addItem(ell);

    //        QGraphicsEllipseItem *ell2 = new QGraphicsEllipseItem();
    //        ell2->setPen(pen);
    //        ell2->setRect(circ2.getCenter().x()-circ2.getRadis(), circ2.getCenter().y()-circ2.getRadis(), circ2.getRadis()*2,
    //                     circ2.getRadis()*2);
    //        scene->addItem(ell2);
    //    }


}


void MainWindow::on_pushButtonRenderTest_clicked()
{
    QTransform tr= ui->graphicsView->transform().inverted();
    QRectF sceneRect =  tr.mapRect(scene->sceneRect());

    QMatrix const matrix = ui->graphicsView->matrix().inverted();

    QRectF visibleRect = matrix.mapRect(ui->graphicsView->viewport()->rect());
    visibleRect.moveTopLeft(matrix.map(QPoint(ui->graphicsView->horizontalScrollBar()->value(),
                                              ui->graphicsView->verticalScrollBar()->value())));
    QImage image(1920, 1080, QImage::Format_ARGB32_Premultiplied);
    //image.setDotsPerMeterX(300);
    //image.setDotsPerMeterY(300);
    //image.fill(Qt::yellow);
    image.fill(QColor(Qt::white));

    {
        QPainter painter(&image);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setRenderHint(QPainter::TextAntialiasing);
        painter.setRenderHint(QPainter::SmoothPixmapTransform);

        //painter.setPen(Qt::NoPen);
        //painter.setOpacity(1.0);
        //painter.begin(&image);  // Переместите эту строку перед настройкой порта, окна и трансформаций
        //painter.setViewport(sceneRect.toRect());
        //painter.setWindow(sceneRect.toRect());
        // painter.setTransform(tr);
        //painter.setWorldTransform(ui->graphicsView->transform());
        //painter.begin(&image);
        //sceneRect = scene->itemsBoundingRect();
        //QTransform transform;
        //painter.setMatrix(matrix);
        //transform.translate(image.rect().width() / 2, image.rect().height() / 2);
        //transform.scale(image.rect().width() / sceneRect.width(), image.rect().height() / sceneRect.height());
        //transform.translate(-sceneRect.center().x(), -sceneRect.center().y());
        //painter.setTransform(transform);
        painter.scale(1,-1);
        painter.scale(matrix.m11(),matrix.m22());
        painter.translate(200, 90);
        //painter.translate(0, -visibleRect.height());
        //painter.setTransform(ui->graphicsView->transform());

        qDebug()<<visibleRect;
        scene->render(&painter,image.rect(),QRectF(visibleRect.x()-133,visibleRect.y()-222,
                                                   visibleRect.width(), visibleRect.height()+100));
        painter.end();
    }

    image.save("экстент.png");


    //    scene->setSceneRect(scene->itemsBoundingRect());                          // Re-shrink the scene to it's bounding contents
    //    QImage image(scene->sceneRect().size().toSize(), QImage::Format_ARGB32);  // Create the image with the exact size of the shrunk scene
    //    image.fill(Qt::transparent);                                              // Start all pixels transparent
    //    //image.setDevicePixelRatio(3000);
    //    QPainter painter(&image);
    //    painter.begin(&image);
    //    painter.setTransform(ui->graphicsView->transform());
    //    painter.setRenderHint(QPainter::Antialiasing);
    //    painter.setRenderHint(QPainter::TextAntialiasing);
    //    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    //    scene->render(&painter);
    //    painter.end();
    //    image.save("экстент.png");

}


void MainWindow::on_pushButtonTestRep_clicked()
{
    LIPCoordinateSystemLibrary lib;
    LIPProject::getInstance().getActiveLayer()->reproject(lib.getCRSbyName(ui->crsComboBox->currentText()));
}


void MainWindow::on_pushButton_14_clicked()
{
    LIPCoordinateSystemLibrary lib;
    LIPProject::getInstance().getActiveLayer()->setCoordinateSystem(lib.getCRSbyName(ui->crsComboBox->currentText()));

}


void MainWindow::on_pushButton_geosTest_clicked()
{
    LIPPointLayer* new_layer= dynamic_cast<LIPPointLayer*>(LIPProject::getInstance().getActiveLayer());
    QVector<QPointF> pl;
    LIPTriangulationGeos::getTriangulation(new_layer);
    pl = triangulate(new_layer);
}


void MainWindow::on_actionGeosTriangulation_triggered()
{
    LIPTriangulationGeosForm *form = new LIPTriangulationGeosForm;
    form->exec();
    auto layer=form->getTriangulationLayer();
    connect(this, SIGNAL(scaleFactorChanged(double)), layer, SLOT(setSceneScaleFactor(double)));
    delete form;
    form=nullptr;
}


void MainWindow::on_actionIntersection_triggered() //пересечение
{
    LIPIntersectionForm* form = new LIPIntersectionForm;
    form->exec();
    delete form;
    form=nullptr;
}


void MainWindow::on_pushButton_addFeature_clicked(bool checked)
{
    if (checked)
    {

        LIPVectorLayer *tL = LIPProject::getInstance().getActiveLayer();
        if (tL!=nullptr)
        {
            ui->graphicsView->setCursor(Qt::CrossCursor);
            scene->startAddingFeatures(tL);
            ui->graphicsView->updateAddingFeaturesFlag(true);
            ui->pushButtonSelectFeature->setChecked(false);
            LIPProject::getInstance().setSelectFeatureFlag(false);
            return;
        }
        LIPWidgetManager::getInstance().showMessage("Для добавления новых объектов необходимо выбрать активный слой",
                                                    2000, messageStatus::Error);
        ui->graphicsView->updateAddingFeaturesFlag(false);
        ui->pushButton_addFeature->setChecked(false);

    }
    else
    {
        ui->graphicsView->setCursor(Qt::ArrowCursor);
        scene->stopAddingFeatures();
        ui->graphicsView->updateAddingFeaturesFlag(false);
    }
}


void MainWindow::on_actionOpenRasterLayer_triggered()
{
    QString fileName=QFileDialog::getOpenFileName(this,"","");
    new LIPRasterLayer(fileName);
}


void MainWindow::on_actionVoronoiDiagram_triggered()
{
    LIPVoronoiDiagramForm *form = new LIPVoronoiDiagramForm;
    form->exec();
    delete form;
    form=nullptr;
}


void MainWindow::on_actionUnion_triggered()
{
    LIPVectorUnionForm *form = new LIPVectorUnionForm;
    form->exec();
    delete form;
    form=nullptr;
}


void MainWindow::on_actionRenderMap_triggered()
{
    //    QTransform tr= ui->graphicsView->transform().inverted();
    //    QRectF sceneRect =  tr.mapRect(scene->sceneRect());

    //    QMatrix const matrix = ui->graphicsView->matrix().inverted();
    //    QRectF visibleRect1 = ui->graphicsView->mapToScene(ui->graphicsView->viewport()->rect()).boundingRect();


    //    qDebug() << "Visible Rectangle Scene Coordinate: " << visibleRect1;
    //     tr.mapRect(visibleRect1);
    //     qDebug() << "Visible Rectangle Scene Coordinate: " << visibleRect1;
    //    QRectF visibleRect = matrix.mapRect(ui->graphicsView->viewport()->rect());

    //    visibleRect.moveTopLeft(matrix.map(QPoint(ui->graphicsView->horizontalScrollBar()->value(),
    //                                              ui->graphicsView->verticalScrollBar()->value())));

    //    QImage image(2920, 1080, QImage::Format_ARGB32_Premultiplied);
    //    //image.setDotsPerMeterX(300);
    //    //image.setDotsPerMeterY(300);
    //    //image.fill(Qt::yellow);
    //    image.fill(QColor(Qt::white));

    //    {
    //        QPainter painter(&image);
    //        painter.setRenderHint(QPainter::Antialiasing);
    //        painter.setRenderHint(QPainter::TextAntialiasing);
    //        painter.setRenderHint(QPainter::SmoothPixmapTransform);

    //        //painter.setPen(Qt::NoPen);
    //        //painter.setOpacity(1.0);
    //        //painter.begin(&image);  // Переместите эту строку перед настройкой порта, окна и трансформаций
    //        //painter.setViewport(sceneRect.toRect());
    //        //painter.setWindow(sceneRect.toRect());
    //        // painter.setTransform(tr);
    //        //painter.setWorldTransform(ui->graphicsView->transform());
    //        //painter.begin(&image);
    //        //sceneRect = scene->itemsBoundingRect();
    //        //QTransform transform;
    //        //painter.setMatrix(matrix);
    //        //transform.translate(image.rect().width() / 2, image.rect().height() / 2);
    //        //transform.scale(image.rect().width() / sceneRect.width(), image.rect().height() / sceneRect.height());
    //        //transform.translate(-sceneRect.center().x(), -sceneRect.center().y());
    //        //painter.setTransform(transform);

    //        painter.scale(matrix.m11(),matrix.m22());
    //        painter.scale(1,-1);
    //        //painter.translate(180, -90);
    //        //painter.translate(0, -visibleRect.height());
    //        //painter.setTransform(ui->graphicsView->transform());
    //        qDebug()<<visibleRect;
    //        auto rect = ui->graphicsView->mapToScene(visibleRect.toRect()).boundingRect();

    ////        scene->render(&painter,image.rect(),QRectF(visibleRect.x(),visibleRect.y()-222,
    ////                                                   visibleRect.width(), visibleRect.height()+100));
    //        scene->render(&painter, image.rect(), visibleRect1);
    //        qDebug()<<rect;
    //        painter.end();
    //    }
    //    //image.mirrored(true, false);
    //    image = image.mirrored(false, true);
    //    image.save("экстент.png");
    QString fileName=QFileDialog::getSaveFileName(this,"","","Image files (*.jpg *.png)");
    if (fileName.isNull() || fileName.isEmpty())
        return;

    auto oldHints=ui->graphicsView->renderHints();
    ui->graphicsView->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::HighQualityAntialiasing);
    ui->graphicsView->update();
    QPixmap pixMap = this->ui->graphicsView->grab();
    auto im = pixMap.toImage();
    pixMap.save(fileName, "PNG");
    ui->graphicsView->setRenderHints(oldHints);
    //qDebug()<<ui->graphicsView->renderHints();
}


void MainWindow::on_actionSaveProject_triggered()
{
    QString fileName=QFileDialog::getSaveFileName(this,"","","Project file (*.vrsa)");
    if (fileName.isNull() || fileName.isEmpty())
        return;
    fileName=fileName+".vrsa";
    LIPProject::getInstance().saveProject(fileName);

}


void MainWindow::on_action_triggered()
{
    QString fileName=QFileDialog::getOpenFileName(this,"","","Project file (*.vrsa)");
    if (fileName.isNull() || fileName.isEmpty())
        return;
    LIPProject::getInstance().openProject(fileName);
}


void MainWindow::on_crsComboBox_currentIndexChanged(int index)
{
    //ui->crsComboBox->currentText();
    LIPCoordinateSystemLibrary *lib = new LIPCoordinateSystemLibrary;
    LIPProject::getInstance().setProjectCRS(
                lib->getCRSbyName(ui->crsComboBox->currentText()));
    delete lib;

}


void MainWindow::on_actionVectorReproject_triggered()
{
    LIPVectorReprojectForm* form = new LIPVectorReprojectForm;
    form->exec();
    delete form;
}


void MainWindow::on_actionAssignVectorProjection_triggered()
{
    LIPVectorAssignProjectForm* form = new LIPVectorAssignProjectForm;
    form->exec();
    delete form;
}


void MainWindow::on_actionCutRasterByVectorMask_triggered()
{
    LIPCutRasterLayerForm * form = new LIPCutRasterLayerForm;
    form->exec();
    delete form;
}


void MainWindow::on_actionRasterReproject_triggered()
{
    LIPReprojectRasterLayerForm *form = new LIPReprojectRasterLayerForm;
    form->exec();
    delete form;
}


void MainWindow::on_actionRasterContours_triggered()
{
    LIPRasterContoursForm* form = new LIPRasterContoursForm();
    form->exec();
    delete form;
}


void MainWindow::on_pushButtonMoveLayerUp_clicked()
{
    QTreeWidgetItem* item = ui->LayerTree->currentItem(); //получаем первый выбранный элемент
    if (item == nullptr)
        return;

    int currentIndex = ui->LayerTree->indexOfTopLevelItem(item);
    if (currentIndex == 0) //если уже "вверху"
        return;
    QTreeWidgetItem* currentItem = ui->LayerTree->takeTopLevelItem(currentIndex);
    ui->LayerTree->insertTopLevelItem(currentIndex - 1, currentItem);
    ui->LayerTree->setCurrentItem(currentItem);

}


void MainWindow::on_pushButtonMoveLayerDown_clicked()
{
    QTreeWidgetItem* item = ui->LayerTree->currentItem(); //получаем первый выбранный элемент
    if (item == nullptr)
        return;

    int currentIndex = ui->LayerTree->indexOfTopLevelItem(item);
    if (currentIndex == ui->LayerTree->topLevelItemCount()-1) //если уже "внизу"
        return;
    QTreeWidgetItem* currentItem = ui->LayerTree->takeTopLevelItem(currentIndex);
    ui->LayerTree->insertTopLevelItem(currentIndex + 1, currentItem);
    ui->LayerTree->setCurrentItem(currentItem);
}


void MainWindow::on_pushButtonDeleteLayer_clicked()
{
    QTreeWidgetItem* clickedItem = ui->LayerTree->currentItem(); //получаем первый выбранный элемент
    if (clickedItem == nullptr)
        return;
    //убираем "вложенность" элементов
    QString path = clickedItem->toolTip(0); //получаем имя файла
    LIPVectorLayer *selectedLayer = LIPProject::getInstance().getVectorLayerByPath(path);
    if (selectedLayer!=nullptr)
    {
        for(int i=0; i<LIPProject::getInstance().getVectorLayers().size();i++)
        {
            if (LIPProject::getInstance().getVectorLayers().at(i)==selectedLayer)
            {
                LIPProject::getInstance().deleteVectorByPath(selectedLayer->returnFileName());

                QTreeWidgetItem *parent = clickedItem->parent();
                int index;
                if (parent)
                {
                    index = parent->indexOfChild(clickedItem);
                    delete parent->takeChild(index);
                }
                else
                {
                    index = ui->LayerTree->indexOfTopLevelItem(clickedItem);
                    delete ui->LayerTree->takeTopLevelItem(index);
                }
                //delete clickedItem;
            }
        }
    }
    else
    {
        LIPRasterLayer* selectedRasterLayer = LIPProject::getInstance().getRasterLayerByPath(path);
        if (!selectedRasterLayer)
            return;
        for(int i=0; i<LIPProject::getInstance().getRasterLayers().size();i++)
        {
            if (LIPProject::getInstance().getRasterLayers().at(i)==selectedRasterLayer)
            {
                LIPProject::getInstance().deleteRasterLayerByPath(selectedRasterLayer->getFileName());

                QTreeWidgetItem *parent = clickedItem->parent();
                int index;
                if (parent)
                {
                    index = parent->indexOfChild(clickedItem);
                    delete parent->takeChild(index);
                }
                else
                {
                    index = ui->LayerTree->indexOfTopLevelItem(clickedItem);
                    delete ui->LayerTree->takeTopLevelItem(index);
                }
                //delete clickedItem;
            }
        }

    }



}


void MainWindow::on_actionSettings_triggered()
{
    LIPSettingsForm* form = new LIPSettingsForm;
    form->exec();
    delete form;
}


void MainWindow::on_pushButtonSelectFeature_clicked()
{

}


void MainWindow::on_pushButtonSelectFeature_clicked(bool checked)
{
    if (checked)
    {
        scene->stopAddingFeatures();
        ui->graphicsView->updateAddingFeaturesFlag(false);
        ui->pushButton_addFeature->setChecked(false);
        LIPProject::getInstance().setSelectFeatureFlag(true);
        ui->graphicsView->setCursor(Qt::WhatsThisCursor);
    }
    else
    {
        LIPProject::getInstance().setSelectFeatureFlag(false);
        ui->graphicsView->setCursor(Qt::ArrowCursor);
    }
}


void MainWindow::on_pushButtonShowAttributeTable_clicked()
{

    LIPVectorLayer *tL = LIPProject::getInstance().getActiveLayer();
    if (tL!=nullptr)
    {
        LIPAttributesTableForm* form = new LIPAttributesTableForm();
        form->setLayer(tL);
        form->exec();
        delete form;
        return;
    }
    LIPWidgetManager::getInstance().showMessage("Для просмотра атрибутов выберите активный слой",
                                                2000, messageStatus::Error);
}


void MainWindow::on_pushButtonZoomToLayer_clicked()
{
    QTreeWidgetItem* clickedItem = ui->LayerTree->currentItem(); //получаем первый выбранный элемент
    if (clickedItem == nullptr)
        return;
    QString path = clickedItem->toolTip(0); //получаем имя файла
    LIPVectorLayer *selectedLayer = LIPProject::getInstance().getVectorLayerByPath(path);
    if (selectedLayer!=nullptr)
    {
        QRectF targetRect = selectedLayer->getBoundingBox();
        ui->graphicsView->fitInView(targetRect, Qt::KeepAspectRatio);
        ui->graphicsView->centerOn(targetRect.center());
        emit scaleFactorChanged(ui->graphicsView->transform().m11());
        //ui->graphicsView->zoomToRect(selectedLayer->getBoundingBox());
    }
    else
    {
        LIPRasterLayer* selectedRasterLayer = LIPProject::getInstance().getRasterLayerByPath(path);
        if (!selectedRasterLayer)
            return;
        QRectF targetRect = selectedRasterLayer->getBoundingBox();
        ui->graphicsView->fitInView(targetRect, Qt::KeepAspectRatio);
        ui->graphicsView->centerOn(targetRect.center());
        emit scaleFactorChanged(ui->graphicsView->transform().m11());

    }

}

