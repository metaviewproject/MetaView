#include "instantviewerwindow.h"

InstantViewerWindow::InstantViewerWindow(void)
{
  addDialog = new AddDialog(this);
  connect(addDialog, SIGNAL(added(void)), this, SLOT(addPdfView(void)));

  pdfViewComboBox = new QComboBox(this);
  connect(pdfViewComboBox, SIGNAL(activated(const QString&)), SLOT(chooseActivePdfView(const QString&)));
 
  pdfFilesComboBox = new QComboBox(this);
  connect(pdfFilesComboBox, SIGNAL(activated(const QString&)), SLOT(chooseActivePdfFile(const QString&)));
 
  activePdfView = NULL;
  error = false;

  createActions();
  createDockWidget();
  createToolBars();
  createTimer();

  QFile pdfViewsFile("/home/pizet/.instantviewer/pdfviews");
  if (pdfViewsFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
    while (!pdfViewsFile.atEnd()) {
      QString name = QString(pdfViewsFile.readLine());
      name.chop(1);
      QString compilerPath = QString(pdfViewsFile.readLine());
      compilerPath.chop(1);
      QString fileExtensions = QString(pdfViewsFile.readLine());
      fileExtensions.chop(1);
      QList<QString> fileExtensionList = fileExtensions.split(" ");
      QString workingDirectoryPath = QString(pdfViewsFile.readLine());
      workingDirectoryPath.chop(1);
      PdfView *value = new PdfView(compilerPath, fileExtensionList, workingDirectoryPath, this);

      pdfViews.insert(name, value);
      pdfViewComboBox->addItem(name);
    }
  }
}

InstantViewerWindow::~InstantViewerWindow(void)
{
  if (!QDir("/home/pizet/.instantviewer").exists())
    QDir().mkdir("/home/pizet/.instantviewer");

  QFile pdfViewsFile("/home/pizet/.instantviewer/pdfviews");
  pdfViewsFile.open(QIODevice::WriteOnly | QIODevice::Text);
  QTextStream out(&pdfViewsFile);

  QMap<QString, PdfView *>::const_iterator cit = pdfViews.constBegin();
  while (cit != pdfViews.constEnd()) {
    out << cit.key() << endl;
    out << (cit.value())->compilerPath << endl;
    for (int i = 0; i < (cit.value())->fileExtensions.size() - 1; ++i)
      out << (cit.value())->fileExtensions[i] << " ";
    out << (cit.value())->fileExtensions.last() << endl;
    out << (cit.value())->workingDirectoryPath << endl;
    ++cit;
  }

  pdfViewsFile.close();
}

void InstantViewerWindow::addPdfView(void)
{
  if (!pdfViews.contains(addDialog->name)) {
    QString key = addDialog->name;
    QString compilerPath = addDialog->compilerPath;
    QList<QString> fileExtensions = addDialog->fileExtensions;
    QString directoryPath = addDialog->directoryPath;
    PdfView *value = new PdfView(compilerPath, fileExtensions, directoryPath, this);

    pdfViews.insert(key, value);
    pdfViewComboBox->addItem(key);
  }
}

void InstantViewerWindow::chooseActivePdfView(const QString& pdfViewName)
{
  activePdfView = pdfViews[pdfViewName];

  QList<QString> pdfFileNames;
  pdfFileNames = activePdfView->getPdfFileNames();

  pdfFilesComboBox->clear();
  for (int i = 0; i < pdfFileNames.size(); i++)
    pdfFilesComboBox->addItem(pdfFileNames[i]);

  setCentralWidget(activePdfView);
}

void InstantViewerWindow::chooseActivePdfFile(const QString& pdfFileName)
{
  activePdfView->setPdfView(activePdfView->pdfFilePaths[pdfFileName]);
}

void InstantViewerWindow::findActivePdfViewWorkingDirectory(void)
{
  activePdfView->setWorkingDirectoryPath(QFileDialog::getExistingDirectory(this, QString(), QString(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks));
}

void InstantViewerWindow::update(void)
{
  if (activePdfView != NULL) {
    activePdfView->compile();
    
    if (activePdfView->somethingWasChanged) {
      if (!activePdfView->errorOccured) {
        QList<QString> pdfFileNames;
        pdfFileNames = activePdfView->getPdfFileNames();

        pdfFilesComboBox->clear();
        for (int i = 0; i < pdfFileNames.size(); i++)
          pdfFilesComboBox->addItem(pdfFileNames[i]);

        if (error) {
          errorDockWidget->setVisible(false);
          setCentralWidget(activePdfView);
          error = false;
        }
      } else {
        error = true;
        errorView->setText("");
        errorView->textCursor().insertText(activePdfView->errorMessage);
        errorView->setReadOnly(true);
        errorDockWidget->setVisible(true);
      }
    }
  }
}

void InstantViewerWindow::createActions(void)
{
  addPdfViewAction = new QAction(tr("Add"), this);
  connect(addPdfViewAction, SIGNAL(triggered()), addDialog, SLOT(show()));

  chooseDirectoryAction = new QAction(tr("Dir"), this);
  connect(chooseDirectoryAction, SIGNAL(triggered()), this, SLOT(findActivePdfViewWorkingDirectory(void)));
}

void InstantViewerWindow::createDockWidget(void)
{
  errorDockWidget = new QDockWidget(tr("Error Window"), this);

  errorView = new QTextEdit(this);

  errorDockWidget->setWidget(errorView);
  errorDockWidget->setVisible(false);
  addDockWidget(Qt::RightDockWidgetArea, errorDockWidget);
}

void InstantViewerWindow::createTimer(void)
{
  timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(update()));
  timer->start(1000);
}

void InstantViewerWindow::createToolBars(void)
{
  configToolBar = addToolBar("Config");
  configToolBar->addAction(addPdfViewAction);
  configToolBar->addWidget(pdfViewComboBox);

  pdfViewToolBar = addToolBar("PdfView");
  pdfViewToolBar->addAction(chooseDirectoryAction);
  pdfViewToolBar->addWidget(pdfFilesComboBox);
}
