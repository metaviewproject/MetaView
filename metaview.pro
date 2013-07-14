TEMPLATE = app
TARGET = 
DEPENDPATH += . obs
INCLUDEPATH += .

# Input
HEADERS += mainwindow.h metaview.h metaerror.h
SOURCES += main.cpp mainwindow.cpp metaview.cpp metaerror.cpp
INCLUDEPATH += /usr/include/poppler/qt4
LIBS += -L/usr/lib -lpoppler-qt4
RESOURCES += icons.qrc
