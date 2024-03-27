#include "lipvectorunion.h"

LIPVectorUnion::LIPVectorUnion()
    :resultLayer{nullptr}
{

}

QVector<QVector<QPointF> > LIPVectorUnion::getUnion(LIPVectorLayer *inputLayer, LIPVectorLayer *unionLayer)
{
    //проверяем геометрию исходного слоя
    LIPPointLayer* pointLayer=dynamic_cast<LIPPointLayer*>(inputLayer);
    if (pointLayer!=nullptr) //если  слой точечный
    {
        //приводим также к полигону так как типы геометрий должны совпадать
        LIPPointLayer* pointUnionLayer=dynamic_cast<LIPPointLayer*>(unionLayer);
        if (pointUnionLayer==nullptr)
            return QVector<QVector<QPointF>>();
        //переходим к геометрии мультиточки GEOS
        auto geosPoints = LIPVectorConvertor::vectorPointstoGeosPoints(pointLayer->returnCords());

        //auto geosPoints2 = LIPVectorConvertor::(pointLayer->returnCords());
        qDebug()<<"num geom" + QString::number(geosPoints->getNumGeometries());
        QVector<QVector<QPointF>> resultVector;
        QVector<QPointF> points;
        std::unique_ptr<geos::geom::Geometry> unionResult;
        try {
            auto inter = LIPVectorConvertor::vectorPointstoGeosPoints(pointUnionLayer->returnCords());
            unionResult=geosPoints->Union(inter.get());
            for(std::size_t i=0; i<unionResult->getNumGeometries(); i++)
            {
                auto pG = geosPoints->getGeometryN(i);
                points.append(QPointF(pG->getX(), pG->getY()));
            }
            resultVector.append(points);
            return resultVector;

        } catch ( std::runtime_error& e) {
            // Обработка исключения типа TopologyException
            //std::cerr << "Ошибка топологии GEOS: " << e.what() << std::endl;
            qDebug()<<e.what();
            return QVector<QVector<QPointF>>();
        }

    }

    LIPLineLayer* lineLayer=dynamic_cast<LIPLineLayer*>(inputLayer);
    if(lineLayer!=nullptr) //если линия
    {
        //приводим также к линии так как типы геометрий должны совпадать
        LIPLineLayer* lineUnionLayer=dynamic_cast<LIPLineLayer*>(unionLayer);
        if (lineUnionLayer==nullptr)
            return QVector<QVector<QPointF>>();
        //переходим к геометрии мультиполигона GEOS
        auto geosLines = LIPVectorConvertor::vectorPointstoGeosLine(lineLayer->returnCords());
        QVector<QVector<QPointF>> resultVector;
        std::unique_ptr<geos::geom::Geometry> unionResult;
        try {
            auto inter = LIPVectorConvertor::vectorPointstoGeosLine(lineUnionLayer->returnCords());
            unionResult=geosLines->Union(inter.get());
        } catch ( std::runtime_error& e) {
            // Обработка исключения типа TopologyException
            //std::cerr << "Ошибка топологии GEOS: " << e.what() << std::endl;
            qDebug()<<e.what();
            return QVector<QVector<QPointF>>();
        }
            unionResult.get()->getGeometryN(0)->getCoordinates();
            for(std::size_t i=0;i<unionResult->getNumGeometries(); i++)
            {
                auto line = unionResult->getGeometryN(i); //получаем геометрию
                QVector<QPointF> lineCords;
                auto geosCords = line->getCoordinates();
                for (std::size_t i=0; i<geosCords->size(); i++)
                {
                    lineCords.append(QPointF(geosCords->getAt(i).x, geosCords->getAt(i).y));
                }
                resultVector.append(lineCords);
            }
            return resultVector;
    }

    LIPPolygonLayer* polyLayer=dynamic_cast<LIPPolygonLayer*>(inputLayer);
    if(polyLayer!=nullptr) //если полигон
    {
        //приводим также к полигону так как типы геометрий должны совпадать
        LIPPolygonLayer* polyUnionLayer=dynamic_cast<LIPPolygonLayer*>(unionLayer);
        if (polyUnionLayer==nullptr)
            return QVector<QVector<QPointF>>();
        //переходим к геометрии мультиполигона GEOS
        auto geosPoly = LIPVectorConvertor::vectorPointstoGeosPolygon(polyLayer->returnCords());
        QVector<QVector<QPointF>> resultVector;
        std::unique_ptr<geos::geom::Geometry> unionResult;
        try {
            auto inter = LIPVectorConvertor::vectorPointstoGeosPolygon(polyUnionLayer->returnCords());
            qDebug()<<":idk";
            unionResult=geosPoly->Union(inter.get());

        } catch ( std::runtime_error& e) {
            // Обработка исключения типа TopologyException
            //std::cerr << "Ошибка топологии GEOS: " << e.what() << std::endl;
            qDebug()<<e.what();
            return QVector<QVector<QPointF>>();


        }
            for(std::size_t i=0;i<unionResult->getNumGeometries(); i++)
            {
                auto polygon = unionResult->getGeometryN(i); //получаем геометрию
                QVector<QPointF> polyCords;
                auto geosCords = polygon->getCoordinates();
                for (std::size_t i=0; i<geosCords->size(); i++)
                {
                    if (i==geosCords->size()-1)
                        break;
                    polyCords.append(QPointF(geosCords->getAt(i).x, geosCords->getAt(i).y));
                }
                resultVector.append(polyCords);
            }
            return resultVector;


    }
}
