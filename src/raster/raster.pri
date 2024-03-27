QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    $$PWD/lipcutrasterlayer.cpp \
    $$PWD/liprasterprocessing.cpp \
    $$PWD/liprasterstyle.cpp \
    $$PWD/liprastertransform.cpp \
   \
  $$PWD/liprasterlayer.cpp

HEADERS += \
    $$PWD/lipcutrasterlayer.h \
    $$PWD/liprasterprocessing.h \
    $$PWD/liprasterstyle.h \
    $$PWD/liprastertransform.h \
  \
 $$PWD/liprasterlayer.h



INCLUDEPATH += /usr/include/gdal
LIBS += /usr/lib/libgdal.so


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
