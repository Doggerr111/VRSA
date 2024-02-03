#ifndef LIPATTRIBUTE_H
#define LIPATTRIBUTE_H
#include "liptypes.h"
#include <QVariant>
class LIPAttribute
{
public:
    LIPAttribute(QString n, LIPAttributeType t, QVariant v);
    QString getName() const;
    LIPAttributeType getType() const;
    QVariant getValue() const;

    void setName(QString name);
    void setType(LIPAttributeType type);
    void setValue(QVariant value);
private:
    QString mName;
    LIPAttributeType mType;
    QVariant mValue;



};

#endif // LIPATTRIBUTE_H
