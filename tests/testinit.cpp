#include "testinit.h"

testInit::testInit()
{
    testLIPVectorConvertor tVC;
    QTest::qExec(&tVC);
}
