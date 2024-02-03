#ifndef LIPTYPES_H
#define LIPTYPES_H

enum class LIPGeometryType{
    LIPPoint,
    LIPLineString,
    LIPPolygon
};

enum class LIPStyleUnit{
    Millimeters,
    Inches,
    Pixels
};

enum class LIPAttributeType{
    INT32,
    String,
    Real
};

class LIPTypes
{
public:
    enum class LIPCrs {};

    enum class LIPDrivers{
        ESRIShapefile
    };

    LIPTypes();
};

#endif // LIPTYPES_H
