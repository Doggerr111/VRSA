#ifndef LIPPOINTSYMBOL_H
#define LIPPOINTSYMBOL_H


enum PointSymbolType{
    Square,
    Circle,
    Triangle,
    Rhombus
};
class LIPPointSymbol
{
public:
    LIPPointSymbol();
    PointSymbolType mType;
    int mSizeMM;

};

#endif // LIPPOINTSYMBOL_H
