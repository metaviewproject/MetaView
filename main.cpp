#include <QApplication>

#include "instantviewerwindow.h"

int main(int argc, char **argv)
{
  QApplication app(argc, argv);

  InstantViewerWindow instantViewer;
  instantViewer.resize(400, 400);
  instantViewer.show();

  return app.exec();
}
