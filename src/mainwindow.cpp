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
    LIPCoordinateSystemLibrary *lib = LIPProject::getInstance().getCRSLibrary();
    for(LIPCoordinateSystem *CRS: lib->getCRSLib())
        LIPProject::getInstance().addCoordinateSystem(CRS);
    connect(ui->LayerTree, SIGNAL(itemDropped()), this, SLOT(layersOrderChanged()));
    ui->LayerTree->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->DBLayerTree->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->LayerTree, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(layerTreeDataChanged(QTreeWidgetItem*, int)));
    connect(ui->LayerTree, SIGNAL(customContextMenuRequested(const QPoint&)),
            this, SLOT(showLayerContextMenu(const QPoint&)));
    connect(ui->DBLayerTree, SIGNAL(customContextMenuRequested(const QPoint&)),
            this, SLOT(showDBLayerContextMenu(const QPoint&)));
    connect(ui->LayerTree, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(onItemChanged(QTreeWidgetItem*, int)));
    connect(ui->lineEditCoordinates, &QLineEdit::returnPressed, this, &MainWindow::centerScene);
    connect(ui->lineEditScale, &QLineEdit::returnPressed, this, &MainWindow::applyScale);
    sceneInitialization();
    recalculateScale();

    ui->right_menu_frame->setAlignment(Qt::AlignRight);
    ui->left_menu_frame->setAlignment(Qt::AlignLeft);

    //ui->LayerTree->setDragDropMode(QAbstractItemView::DragDrop);
    ui->LayerTree->setDefaultDropAction(Qt::MoveAction);
    ui->LayerTree->setDragDropOverwriteMode(false);
    //ui->LayerTree->setDragDropMode(QAbstractItemView::InternalMove);
    ui->LayerTree->setDragEnabled(true);
    ui->LayerTree->setAcceptDrops(true);
    //ui->LayerTree->setDragDropMode(QAbstractItemView::DragDrop);
    ui->LayerTree->setSelectionMode(QAbstractItemView::SingleSelection);
    //ui->LayerTree->setDragDropMode(QAbstractItemView::InternalMove);
    ui->LayerTree->setDragDropMode(QAbstractItemView::DragDrop);
    ui->LayerTree->setDropIndicatorShown(true);

    LIPWidgetManager::getInstance().setMainWindow(this);
    //LIPWidgetManager::getInstance().showMessage("Был успешно открыт новый проект", 2000, messageStatus::Success);

    //LIPXYZConnection* con = new LIPXYZConnection;


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
    double xMin = -20037508.34;
    double yMax = 20037508.34;
    double width = 40075016.68;
    double height = 20037508.34;

    // Установка сцены в пределах EPSG:3857 координат
    ui->graphicsView->scale(1,-1);
    //ui->graphicsView->scene()->setSceneRect(xMin, yMax, width, height);

    ui->graphicsView->setSceneRect(xMin*2, 2*yMax, 2*width, -4*height);
    //ui->graphicsView->setFixedWidth(100);

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
    Q_UNUSED(item);
    Q_UNUSED(column);
}




void MainWindow::redrawNeeded(double f)
{
    Q_UNUSED(f);
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
                        ui->LayerTree->takeTopLevelItem(index);
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
                    ui->LayerTree->takeTopLevelItem(index);
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

        }
        delete form;


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
    Q_UNUSED(layer);
    Q_UNUSED(item);
}






void MainWindow::scenePos(QPointF p) //получение координат мыши на сцене
{
    if (LIPProject::getInstance().getProjectCRS()->getOGRSpatialRef()->IsGeographic())
    {
        QString xCoord = QString::number(p.x(), 'f', 5); // Форматируем координату x с двумя знаками после запятой
        QString yCoord = QString::number(p.y(), 'f', 5); // Форматируем координату y с двумя знаками после запятой
        QString coords = QString("%1  %2").arg(xCoord, yCoord); // Соединяем координаты в одну строку с пробелом между ними
        ui->lineEditCoordinates->setText(coords);
    }
    else
    {
        QString xCoord = QString::number(p.x(), 'f', 2);
        QString yCoord = QString::number(p.y(), 'f', 2);
        QString coords = QString("%1  %2").arg(xCoord, yCoord);
        ui->lineEditCoordinates->setText(coords);
    }


    //ui->lineEdit->setText(QString::number(p.x()) + " "+QString::number(p.y()));
    //ui->graphicsView->centerOn(p);
}

void MainWindow::centerScene() //срабатывает при изменении координат мыши сцены пользователем
{
    QStringList coords = ui->lineEditCoordinates->text().split(" ");
    ui->graphicsView->centerOn(QPointF(coords.at(0).toDouble(), coords.at(1).toDouble()));
}

void MainWindow::applyScale()
{
    LIPMapCalculations *calculator = new LIPMapCalculations();
    calculator->setDpi(QGuiApplication::primaryScreen()->logicalDotsPerInch());
    int scale = ui->lineEditScale->text().toInt();
    QRectF visibleRect = ui->graphicsView->mapToScene(ui->graphicsView->viewport()->rect()).boundingRect();
    double scaleFactor = calculator->calculateScaleFactor(scale, visibleRect, ui->graphicsView->width());
    ui->graphicsView->scale(scaleFactor, scaleFactor);
    ui->graphicsView->centerOn(visibleRect.center());
    emit scaleFactorChanged(ui->graphicsView->transform().m11()); //для обновления граф. элементов сцены
    delete calculator;
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
    ui->lineEditScale->setText(QString::number(static_cast<int>(scale)));
    qDebug()<<"map scale is "+ QString::number(scale);
    qDebug()<< visibleRect;
    emit tileLoadNeeded(scale, visibleRect);

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



//void MainWindow::test(QPointF point)
//{
//    const int dot_per_millimeter = qRound(qApp->primaryScreen()->physicalDotsPerInch() / 25.40);
//    QPen mPen;
//    QBrush mBrush;
//    mBrush.setColor(Qt::blue);
//    mBrush.setStyle(Qt::SolidPattern);
//    mPen.setWidthF(1.0);
//    mPen.setColor(Qt::blue);
//    double scaledSize = 4.0;

//    // calculate necessary image size for the cache
//    double pw = (( mPen.widthF() == 0 ? 1 : mPen.widthF() ) + 1 ) / 2 * 2; // make even (round up); handle cosmetic pen
//    int imageSize = (( int ) scaledSize + pw ) / 2 * 2 + 1; //  make image width, height odd; account for pen width

//    double center = (( double ) imageSize / 2 ) + 0.5; // add 1/2 pixel for proper rounding when the figure's coordinates are added

//    QImage mCache = QImage( QSize( 30, 30 ), QImage::Format_ARGB32_Premultiplied );
//    mCache.fill( 0 );
//    double sdg=mCache.devicePixelRatio();
//    //mCache.setDevicePixelRatio(200);
//    QPainter p;
//    p.begin( &mCache );
//    p.setRenderHint( QPainter::Antialiasing );
//    p.setBrush( mBrush );




//    //p.translate( QPointF( center, center ) );

//    int dpm = 1000 / 0.0254; // ~300 DPI
//    //кол-во точек в 1 мм
//    //mPen.setWidthF(mm * 10); //
//    p.setPen(5);
//    mCache.setDotsPerMeterX(dpm);
//    mCache.setDotsPerMeterY(dpm);
//    qDebug()<<mCache.physicalDpiX();


//    //допустим 10 милиметров


//    //pixel to mm
//    const int dots_per_millimeter = qRound(qApp->primaryScreen()->physicalDotsPerInch() / 25.40);

//    p.drawEllipse(QRectF(0,0,3.77*5,3.77*5));
//    //    if (mCache.save("/home/doger/Pictures/abf.png"))
//    //    {
//    //        QMessageBox::warning(this,"","");
//    //    }

//    pa.drawImage(QPointF(point.x()-mCache.width()/2,point.y()-mCache.height()/2),mCache);

//    ui->graphicsView->scene()->update();


//    // Construct the selected version of the Cache




//    // Check that the selected version is different.  If not, then re-render,
//    // filling the background with the selection color and using the normal
//    // colors for the symbol .. could be ugly!





//}

void MainWindow::wheelEvent(QWheelEvent *event)
{
    Q_UNUSED(event);
    recalculateScale();
    qDebug()<<ui->graphicsView->transform().m11();
    emit scaleFactorChanged(ui->graphicsView->transform().m11());
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
        case LIPGeometryType::LIPUnknown:
        {
            LIPWidgetManager::getInstance().showMessage("Неподдерживаемый тип геометрии",1000, Error);
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
    auto prevActiveLayer = LIPProject::getInstance().getActiveLayer();
    if (prevActiveLayer)
        prevActiveLayer->deselectItems();
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
    Q_UNUSED(checked);
}



bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    Q_UNUSED(watched);
    Q_UNUSED(event);
    return true;
}

void MainWindow::on_actionConnect_to_PostGIS_triggered()
{
    LIPPostGisConnectionForm* form = new LIPPostGisConnectionForm();
    form->exec();
    GDALDataset* dS = form->returnDataSet();
    delete form;
    if (dS==nullptr)
        return;
    auto pgDs = LIPProject::getInstance().getPostGISDataSet();
    if (pgDs!=nullptr)
    {
        for (auto layer: LIPProject::getInstance().getVectorLayers())
        {
            if (layer->getDataSet()==pgDs)
            {


                for (int i = 0; i < ui->LayerTree->topLevelItemCount(); i++)
                {
                    QTreeWidgetItem *item = ui->LayerTree->topLevelItem(i);

                    // Получаем tooltip текст элемента
                    QString itemTooltipText = item->toolTip(0);

                    // Проверяем совпадение tooltip текста
                    if (itemTooltipText == layer->returnFileName())
                    {

                        QTreeWidgetItem *parent = item->parent();
                        int index;
                        if (parent) {
                            index = parent->indexOfChild(item);
                            delete parent->takeChild(index);
                        }
                        else {
                            index = ui->LayerTree->indexOfTopLevelItem(item);
                            ui->LayerTree->takeTopLevelItem(index);
                        }

                        LIPProject::getInstance().deleteVectorByPath(layer->returnFileName());
                        break;
                    }
                }


            }
        }
    }
    GDALClose(pgDs);
    LIPProject::getInstance().setActivePostGISConnection(dS);
    QVector<OGRLayer*> vect = LIPVectorReader::readLayersFromDataset(dS);
    ui->DBLayerTree->clear();
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





void MainWindow::on_actionGeosTriangulation_triggered()
{
    LIPTriangulationGeosForm *form = new LIPTriangulationGeosForm;
    form->exec();
    auto layer=form->getTriangulationLayer();
    if (layer!=nullptr)
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
    ui->graphicsView->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
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
    Q_UNUSED(index);
    LIPCoordinateSystemLibrary *lib = LIPProject::getInstance().getCRSLibrary();
    LIPProject::getInstance().setProjectCRS(
                lib->getCRSbyName(ui->crsComboBox->currentText()));
    recalculateScale();
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
                    ui->LayerTree->takeTopLevelItem(index);
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
                    ui->LayerTree->takeTopLevelItem(index);
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
        LIPVectorLayer* aL = LIPProject::getInstance().getActiveLayer();
        if (aL!=nullptr)
            aL->deselectItems();
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
        recalculateScale();
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
        recalculateScale();

    }

}


void MainWindow::on_pushButton_clicked()
{
    LIPXYZConnection *connection = new LIPXYZConnection;
    connect(this, &MainWindow::tileLoadNeeded, connection, &LIPXYZConnection::onViewportChanged);

}

