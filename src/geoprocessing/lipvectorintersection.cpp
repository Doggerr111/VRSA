#include "lipvectorintersection.h"


LIPVectorIntersection::LIPVectorIntersection()
    :resultLayer{nullptr}
{

}

QVector<QVector<QPointF>> LIPVectorIntersection::getIntersection(LIPVectorLayer *inputLayer, LIPPolygonLayer *overlayLayer)
{
    //проверяем геометрию исходного слоя
    LIPPointLayer* pointLayer=dynamic_cast<LIPPointLayer*>(inputLayer);
    if (pointLayer!=nullptr) //если  слой точечный
    {
        //переходим к геометрии мультиточки GEOS
        auto geosPoints = LIPVectorConvertor::vectorPointstoGeosPoints(pointLayer->returnCords());

        //auto geosPoints2 = LIPVectorConvertor::(pointLayer->returnCords());
        qDebug()<<"num geom" + QString::number(geosPoints->getNumGeometries());
        QVector<QVector<QPointF>> resultVector;
        QVector<QPointF> points;
        std::unique_ptr<geos::geom::Geometry> interResult;
        try {
            auto inter = LIPVectorConvertor::vectorPointstoGeosPolygon(overlayLayer->returnCords());
            for(std::size_t i=0; i<geosPoints->getNumGeometries(); i++)
            {
                auto pG = geosPoints->getGeometryN(i);
                if (pG->intersects(inter.get()))
                {
                    points.append(QPointF(pG->getX(), pG->getY()));

                }
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
        //переходим к геометрии мультиполигона GEOS
        auto geosLines = LIPVectorConvertor::vectorPointstoGeosLine(lineLayer->returnCords());
        QVector<QVector<QPointF>> resultVector;
        std::unique_ptr<geos::geom::Geometry> interResult;
        try {
            auto inter = LIPVectorConvertor::vectorPointstoGeosPolygon(overlayLayer->returnCords());
            qDebug()<<":idk";
            interResult=geosLines->intersection(inter.get());

        } catch ( std::runtime_error& e) {
            // Обработка исключения типа TopologyException
            //std::cerr << "Ошибка топологии GEOS: " << e.what() << std::endl;
            qDebug()<<e.what();
            return QVector<QVector<QPointF>>();
        }
            interResult.get()->getGeometryN(0)->getCoordinates();
            for(std::size_t i=0;i<interResult->getNumGeometries(); i++)
            {
                auto line = interResult->getGeometryN(i); //получаем геометрию
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
        //переходим к геометрии мультиполигона GEOS
        auto geosPoly = LIPVectorConvertor::vectorPointstoGeosPolygon(polyLayer->returnCords());
        QVector<QVector<QPointF>> resultVector;
        std::unique_ptr<geos::geom::Geometry> interResult;
        try {
            auto inter = LIPVectorConvertor::vectorPointstoGeosPolygon(overlayLayer->returnCords());
            qDebug()<<":idk";
            interResult=geosPoly->intersection(inter.get());

        } catch ( std::runtime_error& e) {
            // Обработка исключения типа TopologyException
            //std::cerr << "Ошибка топологии GEOS: " << e.what() << std::endl;
            qDebug()<<e.what();
            return QVector<QVector<QPointF>>();


        }
            interResult.get()->getGeometryN(0)->getCoordinates();
            for(std::size_t i=0;i<interResult->getNumGeometries(); i++)
            {
                auto polygon = interResult->getGeometryN(i); //получаем геометрию
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




