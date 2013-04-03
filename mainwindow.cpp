#include <QtGui>
#include <poppler-qt4.h>
#include "mainwindow.h"
#include <iostream>
#include <QProcess>

MainWindow::MainWindow()
{
  QDir().mkdir(".metaview");
  createToolBar();
  
  //std::cout << QCoreApplication::applicationDirPath().toLocal8Bit().constData() << std::endl;

  textEdit = new QTextEdit(this);
  QVBoxLayout *layout = new QVBoxLayout;
  layout->addWidget(textEdit);
  QWidget *widget = new QWidget;
  widget->setLayout(layout);
  setCentralWidget(widget);

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
  //image = pdfPage->renderToImage(144.0, 144.0, -1, -1, -1, -1);
  image = pdfPage->renderToImage(200.0, 200.0, -1, -1, -1, -1);

  return image;
}

void MainWindow::createView(const QString& path)
{
  if (path != "") {
    QImage image;
    image = loadImage(path);

    QPixmap pixmap;
    pixmap.convertFromImage(image);

    scene = new QGraphicsScene;
    scene->addPixmap(pixmap);

    view = new QGraphicsView(scene);
  
    textEdit = new QTextEdit(this);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(view);
    layout->addWidget(textEdit);
    QWidget *widget = new QWidget;
    widget->setLayout(layout);
    setCentralWidget(widget);
  }
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
    QFileInfo info(listOfMetapostFiles.at(i));
    modificationTimes << info.lastModified();
  }
  for (int i = 0; i < listOfMetapostFiles.size(); ++i) {
    QFileInfo info(listOfMetapostFiles[i]);
    QDateTime currentTime = info.lastModified();
    if (currentTime > modificationTimes[i]) {
      QStringList arguments;
      arguments << listOfMetapostFiles[i];
      QProcess *process = new QProcess(this);
      process->start(QCoreApplication::applicationDirPath() + "/mpeps.py", arguments);
      process->waitForFinished();
      if (process->exitCode() == 0) {
        QDir currentDir;
        listOfEpsFiles = currentDir.entryList(QStringList("*.eps"));
        for (int j = 0; j < listOfEpsFiles.size(); ++j)
          listOfEpsFiles[j] = listOfEpsFiles[j].remove(listOfEpsFiles[j].size()-4, 4);
        mpFiles->clear();
        for (int k = 0; k < listOfEpsFiles.size(); ++k)
          mpFiles->addItem(listOfEpsFiles[k]);
        QStringList arguments2;
        arguments2 << (activeEpsFile + ".eps");
        process->start(QCoreApplication::applicationDirPath() + "/epspdf.py", arguments2);
        process->waitForFinished();
        reloadView(activeEpsFile);
        modificationTimes[i] = currentTime;
      } else {
        QString fileName = listOfMetapostFiles[i];
        QFile file(fileName.remove(fileName.size()-3, 3) + ".log");
        if (file.open(QFile::ReadOnly | QFile::Text)) {
          QByteArray a = file.readAll();
          QString s(a);
          textEdit->setPlainText(s);
        }
        modificationTimes[i] = currentTime;
      }  
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
    process->start(QCoreApplication::applicationDirPath() + "/epspdf.py", arguments);
    process->waitForFinished();
    QString activePdfFile = ".metaview/" + activeEpsFile + ".pdf";
    createView(activePdfFile);
  }
}
