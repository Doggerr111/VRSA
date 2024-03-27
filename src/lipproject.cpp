#include "lipproject.h"
#include "QDebug"

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
    if (activeLayer!=nullptr)
        return activeLayer;
    else
    {
        return nullptr;
    }
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
    if (ds!=nullptr)
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
}

void LIPProject::setAntiAliasing(bool flag)
{
    mAntiAliasingFlag = flag;
    LIPWidgetManager::getInstance().getView()->setRenderHint(QPainter::Antialiasing, mAntiAliasingFlag);


}

QColor LIPProject::getMapCanvasColor()
{
    return mMapCanvasColor;
}

bool LIPProject::isAntiAliasingOn()
{
    return mAntiAliasingFlag;
}





void LIPProject::redrawNeeded(double f)
{

}
