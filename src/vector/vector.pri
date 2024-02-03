QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        $$PWD/lipattribute.cpp \
        $$PWD/lipgraphicsitem.cpp \
        $$PWD/liplinegraphicsitem.cpp \
        $$PWD/liplinelayer.cpp \
        $$PWD/lippointgraphicsitem.cpp \
        $$PWD/lippolygongraphicsitem.cpp \
        $$PWD/lippolygonlayer.cpp \
        $$PWD/liptemporarygraphicsitem.cpp \
        $$PWD/lipvectorlayer.cpp \
        $$PWD/lipvectorstyle.cpp \
        $$PWD/lipvectorconvertor.cpp \
        $$PWD/lipvectortypechecker.cpp \
        vector/lippointlayer.cpp \
        vector/lippoint.cpp

HEADERS += \
        $$PWD/lipattribute.h \
        $$PWD/lipgraphicsitem.h \
        $$PWD/liplinegraphicsitem.h \
        $$PWD/liplinelayer.h \
        $$PWD/lippointgraphicsitem.h \
        $$PWD/lippolygongraphicsitem.h \
        $$PWD/lippolygonlayer.h \
        $$PWD/liptemporarygraphicsitem.h \
        $$PWD/lipvectorlayer.h \
        $$PWD/lipvectorstyle.h \
        $$PWD/lipvectorconvertor.h \
        $$PWD/lipvectortypechecker.h \
        vector/lippointlayer.h \
        vector/lippoint.h



INCLUDEPATH += /usr/include/gdal
INCLUDEPATH += /usr/include/geos
LIBS += /usr/lib/libgdal.so
#LIBS += /lib/x86_64-linux-gnu/libgeos.so
#LIBS += -lgeos
#LIBS += /lib/x86_64-linux-gnu/libgeos.so.3.10.2
#LIBS += /lib/x86_64-linux-gnu/libgeos_c.so.1
#LIBS += /lib/x86_64-linux-gnu/libgeos_c.so
#LIBS += /lib/x86_64-linux-gnu/libgeos_c.so.1.16.0
# Default rules for deployment.e1e
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
