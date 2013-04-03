#include <QApplication>
#include "mainwindow.h"

int main(int argc, char **argv)
{
  QApplication app(argc, argv);

  MainWindow mainWindow;
  mainWindow.resize(600, 700);
  mainWindow.setWindowTitle("MetaView");
  mainWindow.show();

  return app.exec();
}
