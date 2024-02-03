#ifndef LIPFILEVALIDATOR_H
#define LIPFILEVALIDATOR_H
#include <QFileInfo>
#include <QDir>

class LIPFileValidator
{
public:
    LIPFileValidator();
    static bool isPathValid(QString path);
};

#endif // LIPFILEVALIDATOR_H
