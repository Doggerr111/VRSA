QT += testlib
QT += gui
QT       += core sql
QT += core widgets gui
CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app


#include($$PWD/../src/src.pro)

#INCLUDEPATH += $$PWD/../src
INCLUDEPATH += ../src/vector/
#INCLUDEPATH += $$PWD/../src/geoprocessing/
INCLUDEPATH += /usr/include/gdal
INCLUDEPATH += /usr/include/geos
LIBS += /usr/lib/libgdal.so

#LIBS += /lib/x86_64-linux-gnu/libgeos.so
#LIBS += /lib/x86_64-linux-gnu/libgeos.so.3.10.2
LIBS += /home/doger/Documents/geosBuildgit/build/lib/libgeos.so
LIBS += /home/doger/Documents/geosBuildgit/build/lib/libgeos.so.3.13.0
HEADERS += \
    liptest_test.h \
    test.h \
    testlipvectorlayer.h
SOURCES +=  \
    liptest_test.cpp \
    testlipvectorlayer.cpp\
    tst_testlipvectorconvertor.cpp
