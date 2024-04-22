#include "testinit.h"

testInit::testInit()
{
    testLIPVectorConvertor tVC;
    testLIPXYZConnection tXYZ;
    QTest::qExec(&tVC);
    QTest::qExec(&tXYZ);

}
