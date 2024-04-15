QT       += core gui sql
QT += testlib gui
include(vector/vector.pri)
include(treeView/treeView.pri)
include(geoprocessing/geoprocessing.pri)
include(raster/raster.pri)
include(../tests/test.pri)
include(network/network.pri)
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TEMPLATE = app


CONFIG += c++20

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    $$PWD/customWidgets/lipattributetablewidget.cpp \
    $$PWD/customWidgets/lipcombobox.cpp \
    $$PWD/customWidgets/lipcrscombobox.cpp \
    $$PWD/lipaddingitems.cpp \
    $$PWD/lipattributestableform.cpp \
    $$PWD/lipcoordinatesystem.cpp \
    $$PWD/lipcoordinatesystemlibrary.cpp \
    $$PWD/lipfeaturerenderer.cpp \
    $$PWD/lipfilevalidator.cpp \
    $$PWD/lipframe.cpp \
    $$PWD/lipintersectionform.cpp \
    $$PWD/liplayercreator.cpp \
    $$PWD/liplayertreeitem.cpp \
    $$PWD/liplayertreemodel.cpp \
    $$PWD/lipmapcalculations.cpp \
    $$PWD/lipmapholder.cpp \
    $$PWD/lipmapscene.cpp \
    $$PWD/liprasterstyledialog.cpp \
    $$PWD/liptriangulationgeosform.cpp \
    $$PWD/lipwidgetmanager.cpp \
    $$PWD/lipmultipoint.cpp \
    $$PWD/lipnewattrfeatureform.cpp \
    $$PWD/lipnewlinelayerform.cpp \
    $$PWD/lipnewpointlayerform.cpp \
    $$PWD/lippointlayer_1.cpp \
    $$PWD/lippointrenderer.cpp \
    $$PWD/lippostgisconnectionform.cpp \
    $$PWD/lippostgisprovider.cpp \
    $$PWD/lipproject.cpp \
    $$PWD/liptreewidget.cpp \
    $$PWD/liptypes.cpp \
    $$PWD/lipvectorreader.cpp \
    $$PWD/lipvectorstyleform.cpp \
    $$PWD/main.cpp \
    $$PWD/mainwindow.cpp \
    customWidgets/lipbrushtypecombobox.cpp \
    customWidgets/lipcolorpushbutton.cpp \
    customWidgets/lippentypecombobox.cpp \
    customWidgets/liprasterlayerscombobox.cpp \
    lipcutrasterlayerform.cpp \
    lipfeatureattributesform.cpp \
    liprastercontoursform.cpp \
    lipreprojectrasterlayerform.cpp \
    lipsettingsform.cpp \
    lipvectorassignprojectform.cpp \
    lipvectorpropertiesform.cpp \
    lipvectorreprojectform.cpp


HEADERS += \
    $$PWD/customWidgets/lipattributetablewidget.h \
    $$PWD/customWidgets/lipcombobox.h \
    $$PWD/customWidgets/lipcrscombobox.h \
    $$PWD/lipaddingitems.h \
    $$PWD/lipattributestableform.h \
    $$PWD/lipcoordinatesystem.h \
    $$PWD/lipcoordinatesystemlibrary.h \
    $$PWD/lipfeaturerenderer.h \
    $$PWD/lipfilevalidator.h \
    $$PWD/lipframe.h \
    $$PWD/lipintersectionform.h \
    $$PWD/liplayercreator.h \
    $$PWD/liplayertreeitem.h \
    $$PWD/liplayertreemodel.h \
    $$PWD/lipmapcalculations.h \
    $$PWD/lipmapholder.h \
    $$PWD/lipmapscene.h \
    $$PWD/liprasterstyledialog.h \
    $$PWD/liptriangulationgeosform.h \
    $$PWD/lipwidgetmanager.h \
    $$PWD/lipmultipoint.h \
    $$PWD/lipnewattrfeatureform.h \
    $$PWD/lipnewlinelayerform.h \
    $$PWD/lipnewpointlayerform.h \
    $$PWD/lippointlayer_1.h \
    $$PWD/lippointrenderer.h \
    $$PWD/lippostgisconnectionform.h \
    $$PWD/lippostgisprovider.h \
    $$PWD/lipproject.h \
    $$PWD/liptreewidget.h \
    $$PWD/liptypes.h \
    $$PWD/lipvectorreader.h \
    $$PWD/lipvectorstyleform.h \
    $$PWD/mainwindow.h \
    customWidgets/lipbrushtypecombobox.h \
    customWidgets/lipcolorpushbutton.h \
    customWidgets/lippentypecombobox.h \
    customWidgets/liprasterlayerscombobox.h \
    lipcutrasterlayerform.h \
    lipfeatureattributesform.h \
    liprastercontoursform.h \
    lipreprojectrasterlayerform.h \
    lipsettingsform.h \
    lipvectorassignprojectform.h \
    lipvectorpropertiesform.h \
    lipvectorreprojectform.h


FORMS += \
    $$PWD/lipattributestableform.ui \
    $$PWD/lipintersectionform.ui \
    $$PWD/lipnewattrfeatureform.ui \
    $$PWD/lipnewlinelayerform.ui \
    $$PWD/lipnewpointlayerform.ui \
    $$PWD/lippostgisconnectionform.ui \
    $$PWD/liprasterstyledialog.ui \
    $$PWD/liptriangulationgeosform.ui \
    $$PWD/lipvectorstyleform.ui \
    $$PWD/mainwindow.ui \
    lipcutrasterlayerform.ui \
    lipfeatureattributesform.ui \
    liprastercontoursform.ui \
    lipreprojectrasterlayerform.ui \
    lipsettingsform.ui \
    lipvectorassignprojectform.ui \
    lipvectorpropertiesform.ui \
    lipvectorreprojectform.ui

INCLUDEPATH += geoprocessing/
INCLUDEPATH += vector/
INCLUDEPATH += raster/
INCLUDEPATH += ../tests/
INCLUDEPATH += network/

INCLUDEPATH += /usr/include/gdal
INCLUDEPATH += /usr/include/geos
INCLUDEPATH += customWidgets/
LIBS += /usr/lib/libgdal.so.30
#LIBS += /lib/x86_64-linux-gnu/libgeos.so
#LIBS += /lib/x86_64-linux-gnu/libgeos.so.3.10.2
#LIBS += /home/doger/Documents/geosBuildgit/build/lib/libgeos.so
LIBS += /home/doger/Documents/geosBuildgit/build/lib/libgeos.so.3.13.0
#SUBDIRS += tests/ \
 #   tests
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    $$PWD/icons.qrc

DISTFILES += \
    vector/test.pri
