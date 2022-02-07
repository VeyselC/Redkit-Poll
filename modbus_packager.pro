#-------------------------------------------------
#
# Project created by QtCreator 2014-03-22T13:34:57
#
#-------------------------------------------------

QT       += core gui serialport

CONFIG += serial port

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = "Redkit Poll"
TEMPLATE = app
RC_FILE = modbus_packager.rc


SOURCES += main.cpp\
        mainwindow.cpp \
    wrightvaluedialog.cpp \
    modbusmaster.cpp \
    setupdialog.cpp \
    qcustomplot.cpp \
    graph_set.cpp

HEADERS  += mainwindow.h \
    wrightvaluedialog.h \
    modbusmaster.h \
    setupdialog.h \
    qcustomplot.h \
    graph_set.h

FORMS    += mainwindow.ui \
    wrightvaluedialog.ui \
    setupdialog.ui \
    graph_set.ui

OTHER_FILES +=

RESOURCES += \
    images.qrc
