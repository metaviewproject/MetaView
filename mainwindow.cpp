#include <QtGui>
#include <poppler-qt4.h>
#include "mainwindow.h"
#include <iostream>
#include <QProcess>

MainWindow::MainWindow()
{
  QDir().mkdir(".metaview");
  createToolBar();
  createTimer();

  QDir currentDir(".");
  listOfMetapostFiles = currentDir.entryList(QStringList("*.mp"));

  for (int i = 0; i < listOfMetapostFiles.size(); ++i) {
    QFileInfo info(listOfMetapostFiles.at(i));
    modificationTimes << info.lastModified();
  }

  setWindowTitle("MetaView");
}

QImage MainWindow::loadImage(const QString& path)
{
  Poppler::Document *pdfDocument;
  pdfDocument = Poppler::Document::load(path);

  Poppler::Page *pdfPage;
  pdfPage = pdfDocument->page(0);

  QImage image;
  image = pdfPage->renderToImage(72.0, 72.0, -1, -1, -1, -1);

  return image;
}

void MainWindow::createView(const QString& path)
{
  QImage image;
  image = loadImage(path);

  QPixmap pixmap;
  pixmap.convertFromImage(image);

  scene = new QGraphicsScene;
  scene->addPixmap(pixmap);

  view = new QGraphicsView(scene);
}

void MainWindow::createToolBar()
{
  toolBar = addToolBar("main toolbar");
  mpFiles = new QComboBox;
  QDir currentDir;
  listOfEpsFiles = currentDir.entryList(QStringList("*.eps"));
  for (int i = 0; i < listOfEpsFiles.size(); ++i) {
    listOfEpsFiles[i] = listOfEpsFiles[i].remove(listOfEpsFiles[i].size()-4, 4);
    mpFiles->addItem(listOfEpsFiles[i]);
  }
  toolBar->addWidget(mpFiles);
  connect(mpFiles, SIGNAL(activated(const QString&)), this, SLOT(reloadView(const QString&)));
}

void MainWindow::createTimer()
{
  timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(update()));
  timer->start(1000);
}

void MainWindow::update()
{
  QDir currentDir(".");
  listOfMetapostFiles = currentDir.entryList(QStringList("*.mp"));
  for (int i = 0; i < listOfMetapostFiles.size(); ++i) {
    QFileInfo info(listOfMetapostFiles[i]);
    QDateTime currentTime = info.lastModified();
    if (currentTime > modificationTimes[i]) {
      QStringList arguments;
      arguments << listOfMetapostFiles[i];
      QProcess *process = new QProcess(this);
      process->start("./mpeps.py", arguments);
      process->waitForFinished();
      QDir currentDir(".metaview");
      listOfEpsFiles = currentDir.entryList(QStringList("*.eps"));
      for (int j = 0; j < listOfEpsFiles.size(); ++j)
        listOfEpsFiles[j] = listOfEpsFiles[j].remove(listOfEpsFiles[j].size()-4, 4);
      mpFiles->clear();
      for (int k = 0; k < listOfEpsFiles.size(); ++k)
        mpFiles->addItem(listOfEpsFiles[k]);
      QStringList arguments2;
      arguments2 << (activeEpsFile + ".eps");
      process->start("./epspdf.py", arguments2);
      process->waitForFinished();
      reloadView(activeEpsFile);
      modificationTimes[i] = currentTime;
    }
  }
}

void MainWindow::reloadView(const QString& file)
{
  if (file != "") {
    activeEpsFile = file;
    QProcess *process = new QProcess(this);
    QStringList arguments;
    arguments << (activeEpsFile + ".eps");
    process->start("./epspdf.py", arguments);
    process->waitForFinished();
    QString activePdfFile = ".metaview/" + activeEpsFile + ".pdf";
    createView(activePdfFile);
    setCentralWidget(view);
  }
}
