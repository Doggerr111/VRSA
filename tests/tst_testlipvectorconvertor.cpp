#include <QtTest>

// add necessary includes here

class TestLIPVectorConvertor : public QObject
{
    Q_OBJECT

public:
    TestLIPVectorConvertor();
    ~TestLIPVectorConvertor();

private slots:
    void test_case1();

};

TestLIPVectorConvertor::TestLIPVectorConvertor()
{

}

TestLIPVectorConvertor::~TestLIPVectorConvertor()
{

}

void TestLIPVectorConvertor::test_case1()
{

}

QTEST_APPLESS_MAIN(TestLIPVectorConvertor)

#include "tst_testlipvectorconvertor.moc"
