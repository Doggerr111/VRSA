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
    if (dynamic_cast<LIPPolygonLayer*>(layer))
        return false;
    return true;
}
