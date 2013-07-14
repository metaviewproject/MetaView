#include "mainwindow.h"

MainWindow::MainWindow()
{
  metaView = new MetaView;
  createTimer();
  createActions();
  createMenus();
  createToolBar();
  createErrorTabs();
}

void MainWindow::createTimer()
{
  timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(update()));
  timer->start(1000);
}

void MainWindow::createActions()
{
  newFolderAct = new QAction(QIcon(":/images/new-folder.png"), tr("&Open..."), this);
  newFolderAct->setShortcuts(QKeySequence::Open);
  newFolderAct->setStatusTip(tr(":/images/Choose an existing directory"));
  connect(newFolderAct, SIGNAL(triggered()), this, SLOT(open()));
  
  zoomInAct = new QAction(QIcon(":/images/zoom-in.png"), tr("Zoom In"), this);
  zoomInAct->setStatusTip(tr("Zoom in"));
  connect(zoomInAct, SIGNAL(triggered()), this, SLOT(zoomIn()));
  
  zoomOutAct = new QAction(QIcon(":/images/zoom-out.png"), tr("Zoom Out"), this);
  zoomOutAct->setStatusTip(tr("Zoom out"));
  connect(zoomOutAct, SIGNAL(triggered()), this, SLOT(zoomOut()));
  
  showErrorAct = new QAction(QIcon(":/images/error.png"), tr("Show Error"), this);
  showErrorAct->setCheckable(true);
  showErrorAct->setStatusTip(tr("Zoom out"));
  
  closeAct = new QAction(tr("E&xit"), this);
  closeAct->setShortcuts(QKeySequence::Quit);
  closeAct->setStatusTip(tr("Exit the application"));
  connect(closeAct, SIGNAL(triggered()), this, SLOT(close()));
}

void MainWindow::zoomIn()
{
  metaView->zoomIn();
}

void MainWindow::zoomOut()
{
  metaView->zoomOut();
}

void MainWindow::createMenus()
{
  fileMenu = menuBar()->addMenu(tr("&File"));
  fileMenu->addAction(newFolderAct);
  fileMenu->addAction(showErrorAct);
  fileMenu->addSeparator();
  fileMenu->addAction(closeAct);
 
  editMenu = menuBar()->addMenu(tr("&Edit"));
  editMenu->addAction(zoomInAct);
  editMenu->addAction(zoomOutAct);
}

void MainWindow::createToolBar()
{
  epsFiles = new QComboBox(this);
  connect(epsFiles, SIGNAL(activated(const QString&)), this, SLOT(change(const QString&)));

  fileToolBar = addToolBar("File");
  fileToolBar->addAction(newFolderAct);
  fileToolBar->addWidget(epsFiles);
  fileToolBar->addAction(showErrorAct);

  editToolBar = addToolBar("Edit");
  editToolBar->addAction(zoomInAct);
  editToolBar->addAction(zoomOutAct);
}

void MainWindow::createErrorTabs()
{
  errorWindow = new QDockWidget(tr("Error Window"), this);

  errorTabLog = new ErrorTab(this);
  errorTabMp = new ErrorTab(this);
  errorTabMpx = new ErrorTab(this);

  errorTabs = new QTabWidget(this);
  errorTabs->addTab(errorTabLog, tr("log"));
  errorTabs->addTab(errorTabMp, tr("mp"));
  errorTabs->addTab(errorTabMpx, tr("mpx"));

  errorWindow->setWidget(errorTabs);
  errorWindow->setVisible(false);
  addDockWidget(Qt::RightDockWidgetArea, errorWindow);
  connect(showErrorAct, SIGNAL(toggled(bool)), errorWindow, SLOT(setVisible(bool)));
}

void MainWindow::change(const QString& fileName)
{
  metaView->updateView(fileName);
}

void MainWindow::open()
{
  workingDirPath = QFileDialog::getExistingDirectory(
      this, tr("Choose Directory"), "/home",
      QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
  
  QDir(workingDirPath).mkdir(".metaview");

  metaView = new MetaView(workingDirPath, this);
  setCentralWidget(metaView);
}

void MainWindow::update()
{
  QList<QString> filesWithError;
  filesWithError = metaView->compileMetapostFiles();
 
  // TODO
  if (filesWithError.size() == 0) {
    // TODO
    epsFiles->clear();
    listOfEpsFiles = metaView->findEpsFiles();
    for (int i = 0; i < listOfEpsFiles.size(); i++)
      epsFiles->addItem(listOfEpsFiles[i]);
  } else {
    QString fileName = filesWithError[0];
    fileName.remove(fileName.size() - 3, 3);
    QFile logFile(fileName + ".log");
    QFile mpFile(filesWithError[0]);
    QFile mpxFile(fileName + ".mpx");
    errorTabLog->displayFile(logFile);
    errorTabMp->displayFile(mpFile);
    errorTabMpx->displayFile(mpxFile);
    showErrorAct->trigger();
  }
}
