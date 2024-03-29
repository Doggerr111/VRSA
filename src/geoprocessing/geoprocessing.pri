QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
INCLUDEPATH += $$PWD/../customWidgets
INCLUDEPATH += ../customWidgets
SOURCES += \
    $$PWD/lipcircle.cpp \
    $$PWD/lipcutlayerform.cpp \
    $$PWD/lipedge.cpp \
    $$PWD/liptriangle.cpp \
    $$PWD/liptriangulation.cpp \
    $$PWD/liptriangulationgeos.cpp \
    $$PWD/lipvectorclipper.cpp \
    $$PWD/lipvectorintersection.cpp \
    $$PWD/lipvectorunion.cpp \
    $$PWD/lipvectorunionform.cpp \
    $$PWD/lipvoronoidiagram.cpp \
    $$PWD/lipvoronoidiagramform.cpp




HEADERS += \
    $$PWD/lipcircle.h \
    $$PWD/lipcutlayerform.h \
    $$PWD/lipedge.h \
    $$PWD/liptriangle.h \
    $$PWD/liptriangulation.h \
    $$PWD/liptriangulationgeos.h \
    $$PWD/lipvectorclipper.h \
    $$PWD/lipvectorintersection.h \
    $$PWD/lipvectorunion.h \
    $$PWD/lipvectorunionform.h \
    $$PWD/lipvoronoidiagram.h \
    $$PWD/lipvoronoidiagramform.h




FORMS += \
    $$PWD/lipcutlayerform.ui \
    $$PWD/lipvectorunionform.ui \
    $$PWD/lipvoronoidiagramform.ui



INCLUDEPATH += /usr/include/gdal
LIBS += /usr/lib/libgdal.so

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
