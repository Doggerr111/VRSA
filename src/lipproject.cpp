#include "lipproject.h"
#include "QDebug"
#include "mainwindow.h"

LIPProject &LIPProject::getInstance()
{
    static LIPProject instance;
    return instance;
}

void LIPProject::addVectorLayer(LIPVectorLayer *vect)
{
    vectorLayers.append(vect);
}

void LIPProject::addVectorLayers(QVector<LIPVectorLayer *> vects)
{
    for (int i=0; i<vects.size(); i++)
    {
        if (vects.at(i)!=nullptr)
            vectorLayers.append(vects.at(i));
    }
}

void LIPProject::setVectorLayers(QVector<LIPVectorLayer *> vects)
{
    vectorLayers=vects;
}

bool LIPProject::setActiveLayer(LIPVectorLayer *vect)
{
    for (int i=0; i<vectorLayers.size(); i++)
    {
        if (vectorLayers.at(i)==vect)
        {
            activeLayer=vect;
            return true;
        }

    }
    return false;

}

void LIPProject::setActiveLayer(int index)
{
    activeLayer=vectorLayers.at(index);
}

void LIPProject::setActiveLayer(QString name)
{
    for (int i=0; i<vectorLayers.size(); i++)
    {
        if (vectorLayers.at(i)->returnGISName()==name)
        {
            activeLayer=vectorLayers.at(i);
        }
    }
}

void LIPProject::deleteVectorByPath(QString path)
{
    for (int i=0;i<vectorLayers.size();i++)
    {
        if (vectorLayers.at(i)->returnFileName()==path)
        {
            auto lToDel=vectorLayers.at(i);
            vectorLayers.removeAt(i);
            if (lToDel == activeLayer)
                activeLayer=nullptr;
            delete lToDel;


            //
            //vectorLayers.erase(1);
        }
    }
}

QVector<LIPVectorLayer *> LIPProject::getVectorLayers()
{
    return vectorLayers;
}

LIPVectorLayer* LIPProject::getVecorLayerByIndx(int index)
{
    if (vectorLayers.at(index)!=nullptr)
        return vectorLayers.at(index);
    else
    {
        return nullptr;
    }
}

LIPVectorLayer* LIPProject::getVectorLayerByName(QString name)
{
    for (int i=0; i<vectorLayers.size(); i++)
    {
        if (vectorLayers.at(i)->returnGISName()==name)
        {
            return vectorLayers.at(i);
        }
    }
    return nullptr;
}

LIPVectorLayer *LIPProject::getVectorLayerByPath(QString path)
{
    for (int i=0; i<vectorLayers.size(); i++)
    {
        if (vectorLayers.at(i)->returnFileName()==path)
        {
            return vectorLayers.at(i);
        }

    }
    return nullptr;
}

LIPVectorLayer* LIPProject::getActiveLayer()
{
    return activeLayer;
}

void LIPProject::addRasterLayer(LIPRasterLayer *rasterLayer)
{
    rasterLayers.append(rasterLayer);
}

void LIPProject::deleteRasterLayerByPath(QString path)
{
    for (int i=0;i<rasterLayers.size();i++)
    {
        if (rasterLayers.at(i)->getFileName()==path)
        {

            delete rasterLayers.at(i);
            rasterLayers.removeAt(i);

        }
    }
}

void LIPProject::addXYZConnection(LIPXYZConnection *connection)
{
    xyzTiles.append(connection);
}

void LIPProject::deleteXYZConnectionByConnectionName(QString name)
{
    for (int i=0;i<xyzTiles.size();i++)
    {
        if (xyzTiles.at(i)->getURL()==name)
        {
            auto xyz=xyzTiles.at(i);
            xyzTiles.removeAt(i);
            delete xyz;

        }
    }
}

LIPXYZConnection *LIPProject::getXYZConnectionByConnectionName(QString name)
{
    for (int i=0;i<xyzTiles.size();i++)
    {
        if (xyzTiles.at(i)->getURL()==name)
            return xyzTiles.at(i);

    }
}

QVector<LIPRasterLayer *> LIPProject::getRasterLayers()
{
    return rasterLayers;
}

LIPRasterLayer *LIPProject::getRasterLayerByPath(QString path)
{
    for (int i=0; i<rasterLayers.size(); i++)
    {
        if (rasterLayers.at(i)->getFileName()==path)
        {
            return rasterLayers.at(i);
        }

    }
    return nullptr;
}

void LIPProject::setActivePostGISConnection(GDALDataset *ds)
{
    if (ds==nullptr)
        return;
    //if (mActivePostGISConnection!=nullptr)
        //disconnectPostGISConnection();
    mActivePostGISConnection=ds;


}

void LIPProject::disconnectPostGISConnection()
{

    GDALClose(mActivePostGISConnection);
    mActivePostGISConnection=nullptr;
}

GDALDataset *LIPProject::getPostGISDataSet()
{
    return mActivePostGISConnection;
}

bool LIPProject::isDatasetInUse(GDALDataset *dS)
{

    for (int i=0; dS->GetLayerCount(); i++)
    {
        for (int j=0; j<vectorLayers.size(); j++)
        {
            if (dS->GetLayer(i)==vectorLayers.at(j)->getOGRLayer())
            {
                qDebug()<< dS->GetLayer(i);
                qDebug()<<vectorLayers.at(j)->getOGRLayer();
                if (dS->GetLayer(i)==nullptr)
                    return false;
                return true;
            }
        }
    }
    return false;
}

bool LIPProject::addCoordinateSystem(LIPCoordinateSystem *crs)
{
    //TODO добавить проверку на корректность системы
    CRSystems.append(crs);
    emit newCRS();
    return true;
}

QVector<LIPCoordinateSystem*> LIPProject::getCoordinateSystems()
{
    return CRSystems;
}

bool LIPProject::setProjectCRS(LIPCoordinateSystem *s)
{
    if (s==nullptr)
        return false;
    mProjectCRS=s;
    emit crsChanged();
    return true;
}

LIPCoordinateSystem *LIPProject::getProjectCRS()
{
    return mProjectCRS;
}

bool LIPProject::saveProject(QString fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly))
    {
        LIPWidgetManager::getInstance().showMessage("Невозможно сохранить проект", 1000, Error);
        return false;
    }

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_5_15); // Устанавливаем версию для совместимости
    // Записываем количество векторных слоёв.
    out << static_cast<quint32>(vectorLayers.size());
    // Записываем каждый путь в файл
    for (int i=0; i<vectorLayers.size(); i++)
    {
        out << vectorLayers.at(i)->returnFileName();
    }
    // Записываем количество растровых слоёв.
    out << static_cast<quint32>(rasterLayers.size());
    // Записываем каждый путь в файл
    for (int i=0; i<rasterLayers.size(); i++)
    {
        out << rasterLayers.at(i)->getFileName();
    }

    file.close();
    return true;

}

bool LIPProject::openProject(QString fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
    {
        LIPWidgetManager::getInstance().showMessage("Невозможно прочитать файл проекта", 1000, Error);
        return false;
    }
    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_5_15); // Устанавливаем версию для совместимости


    quint32 vectorNumPaths;
    in >> vectorNumPaths;
    QStringList vectorList;

    for (quint32 i = 0; i < vectorNumPaths; ++i) {
        QString path;
        in >> path;
        //qDebug()<<path;
        vectorList.append(path);
    }
    LIPVectorReader::readLayersFromStringList(vectorList);
    quint32 rasterNumPaths;
    in >> rasterNumPaths;

    for (quint32 i = 0; i < rasterNumPaths; ++i) {
        QString path;
        in >> path;
        //qDebug()<<path;
        new LIPRasterLayer(path);
    }

    file.close();


    return true;
}

void LIPProject::addAction(QAction *action)
{
    mGUISettings.mainToolBarActions.append(action);
    LIPWidgetManager::getInstance().getMainWindow();
    writeSettings();
}

void LIPProject::deleteAction(QAction *delAction)
{
    for(int actIt=0; actIt<mGUISettings.mainToolBarActions.size(); actIt++)
    {
        if (mGUISettings.mainToolBarActions.at(actIt)==delAction)
        {
            QVariant v = delAction->property("associatedButton");
            QPushButton* associatedButton = qobject_cast<QPushButton*>(v.value<QWidget*>());
            if (associatedButton)
                delete associatedButton;

            //delete mGUISettings.mainToolBarActions.at(actIt);
            mGUISettings.mainToolBarActions.removeAt(actIt);

        }
    }
    writeSettings();
}

bool LIPProject::isActionAdded(QAction *action)
{
    for(int actIt=0; actIt<mGUISettings.mainToolBarActions.size(); actIt++)
    {
        if (mGUISettings.mainToolBarActions.at(actIt)==action)
            return true;
    }
    return false;
}

void LIPProject::setActions(QVector<QAction *>)
{

}

void LIPProject::setVectorDataFolder(QString path)
{
    mVectorFolder = path;
    writeSettings();
}

void LIPProject::setRasterDataFolder(QString path)
{
    mRasterFolder = path;
}

QString LIPProject::getVectorDataFolder()
{
    return mVectorFolder;
}

QString LIPProject::getRasterDataFolder()
{
    return mRasterFolder;
}

void LIPProject::setMapCanvasColor(QColor col)
{
    if (col.isValid())
        mMapCanvasColor = col;
    else
        mMapCanvasColor = Qt::white;
    LIPWidgetManager::getInstance().getScene()->setBackgroundBrush(QBrush(mMapCanvasColor));
    writeSettings();
}

void LIPProject::setAntiAliasing(bool flag)
{
    mAntiAliasingFlag = flag;
    LIPWidgetManager::getInstance().getView()->setRenderHint(QPainter::Antialiasing, mAntiAliasingFlag);
    writeSettings();


}

QColor LIPProject::getMapCanvasColor()
{
    return mMapCanvasColor;
}

bool LIPProject::isAntiAliasingOn()
{
    return mAntiAliasingFlag;
}

void LIPProject::setSelectFeatureFlag(bool fl)
{
    mIsSelectingFeatures=fl;
}

bool LIPProject::isSelectingFeatures()
{
    return mIsSelectingFeatures;
}

LIPCoordinateSystemLibrary *LIPProject::getCRSLibrary()
{
    if (mCRSLib==nullptr)
        mCRSLib=new LIPCoordinateSystemLibrary;
    return mCRSLib;
}

void LIPProject::writeSettings()
{
    QFile file("config.bin");
    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        QDataStream out(&file);
        // Записываем данные
        out << mVectorFolder << mRasterFolder << mMapCanvasColor << mAntiAliasingFlag;
        out << mGUISettings.mainToolBarActions.size();
        for (QAction* action: mGUISettings.mainToolBarActions)
            out << action->objectName();
        file.close();
    }
}

void LIPProject::loadSettings()
{
    QFile file("config.bin");
    if (file.open(QIODevice::ReadOnly))
    {
        mGUISettings.mainToolBarActions.clear();
        QDataStream in(&file);
        in >> mVectorFolder >> mRasterFolder >> mMapCanvasColor >> mAntiAliasingFlag;
        int size;
        in >> size;
        setMapCanvasColor(mMapCanvasColor);
        setAntiAliasing(mAntiAliasingFlag);
        MainWindow* mW = LIPWidgetManager::getInstance().getMainWindow();
        QList<QAction *> allActions = mW->findChildren<QAction *>();
        for (int i = 0; i < size; i++)
        {
            QString objName;
            in >> objName;
            for (QAction *action: allActions)
            {
                if (action->objectName() == objName)
                    mGUISettings.mainToolBarActions.append(action);
            }
        }

        foreach (QWidget* child, mW->findChildren<QWidget*>())
        {
            if (child->objectName() == "frameMainToolBar")
            {
                for (QAction* action : mGUISettings.mainToolBarActions)
                {
                    QPushButton* button = new QPushButton(action->icon(), "");
                    button->setStyleSheet("QPushButton { border:none; } QPushButton:pressed { background-color: gray; "
                                          "border: 1px solid } QPushButton:hover { border: 1px solid;"
                                          "border-color: #333; /* Цвет рамки при наведении */  }");
                    button->setIcon(action->icon());
                    button->setMinimumSize(QSize(50, 50));
                    button->setIconSize(QSize(45, 45));
                    connect(button, &QPushButton::clicked, action, &QAction::triggered);
                    child->layout()->addWidget(button);
                    action->setProperty("associatedButton", QVariant::fromValue<QWidget*>(button));
                }

            }
        }
    }
}





void LIPProject::redrawNeeded(double f)
{

}
