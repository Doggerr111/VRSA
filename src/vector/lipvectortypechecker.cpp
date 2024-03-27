#include "lipvectortypechecker.h"

LIPVectorTypeChecker::LIPVectorTypeChecker()
{

}

bool LIPVectorTypeChecker::isPointLayer(LIPVectorLayer *layer)
{
    //LIPPointLayer *newLayer = dynamic_cast<LIPPointLayer*>(layer);
    if (dynamic_cast<LIPPointLayer*>(layer)==nullptr)
        return false;
    return true;
}

bool LIPVectorTypeChecker::isLineLayer(LIPVectorLayer *layer)
{
    //LIPLineLayer *newLayer = dynamic_cast<LIPLineLayer*>(layer);
    if (dynamic_cast<LIPLineLayer*>(layer)==nullptr)
        return false;
    return true;
}

bool LIPVectorTypeChecker::isPolygonLayer(LIPVectorLayer *layer)
{
    //LIPPolygonLayer *newLayer = dynamic_cast<LIPPolygonLayer*>(layer);
    if (dynamic_cast<LIPPolygonLayer*>(layer)==nullptr)
        return false;
    return true;
}

LIPGeometryType LIPVectorTypeChecker::getType(LIPVectorLayer *layer)
{
    if (dynamic_cast<LIPPointLayer*>(layer)!=nullptr)
        return LIPGeometryType::LIPPoint;
    if (dynamic_cast<LIPLineLayer*>(layer)!=nullptr)
        return LIPGeometryType::LIPLineString;
    if (dynamic_cast<LIPPolygonLayer*>(layer)!=nullptr)
        return LIPGeometryType::LIPPolygon;
    return LIPGeometryType::LIPUnknown;
}

bool LIPVectorTypeChecker::isSameType(LIPVectorLayer *layer1, LIPVectorLayer *layer2)
{
    if (dynamic_cast<LIPPointLayer*>(layer1)!=nullptr && dynamic_cast<LIPPointLayer*>(layer2)!=nullptr)
        return true;
    if (dynamic_cast<LIPLineLayer*>(layer1)!=nullptr && dynamic_cast<LIPLineLayer*>(layer2)!=nullptr)
        return true;
    if (dynamic_cast<LIPPolygonLayer*>(layer1)!=nullptr && dynamic_cast<LIPPolygonLayer*>(layer2)!=nullptr)
        return true;
    return false;
}
