#include "lipattribute.h"

LIPAttribute::LIPAttribute(QString n, LIPAttributeType t, QVariant v)
    : mName{n},
      mType{t},
      mValue{v}
{

}

QString LIPAttribute::getName() const
{
    return mName;
}

LIPAttributeType LIPAttribute::getType() const
{
    return mType;
}

QVariant LIPAttribute::getValue() const
{
    return mValue;
}

void LIPAttribute::setName(QString name)
{
    mName=name;
}

void LIPAttribute::setType(LIPAttributeType type)
{
    mType=type;
}

void LIPAttribute::setValue(QVariant value)
{
    mValue=value;
}
