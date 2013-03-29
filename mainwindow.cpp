#include <QtGui>
#include <poppler-qt4.h>
#include "mainwindow.h"

MainWindow::MainWindow()
{
  createActions();
  createMenus();
  createView("test.pdf");
  createWatcher();

  setCentralWidget(view);
  setWindowTitle("MetaView");
}

void MainWindow::createActions()
{
  watchFileAction = new QAction("Watch &File", this);
  connect(watchFileAction, SIGNAL(triggered()),
      this, SLOT(addFileToWatch()));
  
  watchDirAction = new QAction("Watch &Directory", this);
  connect(watchDirAction, SIGNAL(triggered()),
      this, SLOT(addDirToWatch()));
  
  exitAction = new QAction("&Close Window", this);
  connect(exitAction, SIGNAL(triggered()), this, SLOT(quit()));
}

void MainWindow::createMenus()
{
  fileMenu = menuBar()->addMenu(tr("&File"));
  fileMenu->addAction(watchFileAction);
  fileMenu->addAction(watchDirAction);
  fileMenu->addSeparator();
  fileMenu->addAction(exitAction);
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

void MainWindow::createWatcher()
{
  watcher = new QFileSystemWatcher(this);
  connect(watcher, SIGNAL(fileChanged(const QString&)),
      this, SLOT(fileWasChanged(const QString&)));
  connect(watcher, SIGNAL(directoryChanged(const QString&)),
      this, SLOT(dirWasChanged(const QString&)));
}

void MainWindow::addFileToWatch()
{
  QString fileName;
  fileName = QFileDialog::getOpenFileName(this,
      tr("Select a File to Watch"), "", tr("MetaPost Files (*.mp)"));

  if (fileName != "")
    watcher->addPath(fileName);
}

void MainWindow::addDirToWatch()
{
  QString dirName;
  dirName = QFileDialog::getExistingDirectory(this,
      tr("Select a Directory to Watch"), "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

  if (dirName != "")
    watcher->addPath(dirName);
}

void MainWindow::fileWasChanged(const QString&)
{
}

void MainWindow::dirWasChanged(const QString&)
{
}

void MainWindow::quit()
{
  QMessageBox messageBox;
  messageBox.setWindowTitle(tr("MetaView"));
  messageBox.setText(tr("Do you really want to quit"));
  messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
  messageBox.setDefaultButton(QMessageBox::No);
  
  if (messageBox.exec() == QMessageBox::Yes)
    qApp->quit();
}
