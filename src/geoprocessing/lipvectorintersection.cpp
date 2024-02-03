#include "lipvectorintersection.h"


LIPVectorIntersection::LIPVectorIntersection()
{

}

LIPVectorLayer *LIPVectorIntersection::getIntersection(LIPVectorLayer *inputLayer, LIPPolygonLayer *overlayLayer)
{
    //проверяем геометрию исходного слоя
    LIPPointLayer* pointLayer=dynamic_cast<LIPPointLayer*>(inputLayer);
    if (pointLayer!=nullptr) //если  слой точечный
    {
        LIPVectorConvertor::getGeosGeometryFromCoordinates(pointLayer->returnCords()).intersection(
                    LIPVectorConvertor::getGeosGeometryFromCoordinates(overlayLayer->returnCords(), LIPGeometryType::LIPPolygon).get());
        //LIPVectorConvertor::getGeosGeometryFromCoordinates(overlayLayer->returnCords(), LIPGeometryType::LIPPolygon);
    }

    LIPLineLayer* lineLayer=dynamic_cast<LIPLineLayer*>(inputLayer);
    if(lineLayer!=nullptr) //если линия
    {

    }

    LIPPolygonLayer* polyLayer=dynamic_cast<LIPPolygonLayer*>(inputLayer);
    if(polyLayer!=nullptr) //если полигон
    {

    }
}




