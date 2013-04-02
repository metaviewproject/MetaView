#include <QApplication>
#include "mainwindow.h"

int main(int argc, char **argv)
{
  QApplication app(argc, argv);

  MainWindow mainWindow;
  mainWindow.resize(500, 500);
  mainWindow.setWindowTitle("QToolBar");
  mainWindow.show();

  return app.exec();
}
