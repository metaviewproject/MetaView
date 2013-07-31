TEMPLATE = app
TARGET = 
DEPENDPATH += . obs
INCLUDEPATH += .

# Input
HEADERS += instantviewerwindow.h pdfview.h adddialog.h
SOURCES += main.cpp instantviewerwindow.cpp pdfview.cpp adddialog.cpp
INCLUDEPATH += /usr/include/poppler/qt4
LIBS += -L/usr/lib -lpoppler-qt4
RESOURCES += icons.qrc
