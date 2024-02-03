#include "lipfilevalidator.h"

LIPFileValidator::LIPFileValidator()
{

}

bool LIPFileValidator::isPathValid(QString path)
{
    QFileInfo fileInfo(path);
    QDir dir;
    if(dir.isAbsolutePath(path))
        return true;
    return false;

}
