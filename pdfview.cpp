#include "pdfview.h"

PdfView::PdfView(QString compilerPath, QList<QString> fileExtensions, QString workingDirectoryPath, QWidget *parent) : QWidget(parent)
{
  this->compilerPath = compilerPath;
  this->fileExtensions = fileExtensions;
  this->workingDirectoryPath = workingDirectoryPath;
  view = NULL;
  errorOccured = false;
  somethingWasChanged = false;

  getWatchedFiles();
}

void PdfView::compile(void)
{
  QList<QString> arguments;
  somethingWasChanged = false;

  arguments.append(workingDirectoryPath);
  for (int i = 0; i < watchedFiles.size(); ++i) {
    QFileInfo info(watchedFiles[i]);
    QDateTime currentTime = info.lastModified();

    if (currentTime > lastModified[i]) {
      somethingWasChanged = true;
      arguments.append(watchedFiles[i]);
    }
  }

  if (somethingWasChanged) {
    process = new QProcess(this);
    process->start(compilerPath, arguments);
    process->waitForFinished();

    updatePdfView();
  }
 
  getWatchedFiles();
}

QList<QString> PdfView::getPdfFileNames(void)
{
  return pdfFilePaths.keys();
}

void PdfView::setWorkingDirectoryPath(const QString& workingDirectoryPath)
{
  this->workingDirectoryPath = workingDirectoryPath;
}

QList<QString> PdfView::findFiles(QString directoryPath)
{
  QDir directory(directoryPath);

  QList<QString> newFiles;
  newFiles = directory.entryList(fileExtensions);
  
  for (int i = 0; i < newFiles.size(); ++i)
    newFiles[i] = directoryPath + "/" + newFiles[i];

  directory.setFilter(QDir::AllDirs | QDir::NoDotAndDotDot);

  QList<QString> subdirectoryPaths;
  subdirectoryPaths = directory.entryList();

  if (subdirectoryPaths.size() == 0)
    return  newFiles;
  else {
    for (int i = 0; i < subdirectoryPaths.size(); i++)
      newFiles += findFiles(directoryPath + "/" + subdirectoryPaths[i]);
    return newFiles;
  }
}

QImage PdfView::loadImage(const QString& pdfFilePath)
{
  Poppler::Document *pdfDocument;
  pdfDocument = Poppler::Document::load(pdfFilePath);
  pdfDocument->setRenderHint(Poppler::Document::Antialiasing);
  pdfDocument->setRenderHint(Poppler::Document::TextAntialiasing);

  Poppler::Page *pdfPage;
  pdfPage = pdfDocument->page(0);

  QImage image;
  image = pdfPage->renderToImage(144.0, 144.0, -1, -1, -1, -1);

  return image;
}

void PdfView::setPdfView(const QString& pdfFilePath)
{
  QImage image;
  image = loadImage(pdfFilePath);
  
  if (view == NULL) {
    scene = new QGraphicsScene(this);
    pixmapItem = scene->addPixmap(QPixmap::fromImage(image));

    view = new QGraphicsView(scene);

    layout = new QVBoxLayout(this);
    layout->addWidget(view);
    setLayout(layout);
  } else {
    int horizontalScrollBarPos;
    horizontalScrollBarPos = view->horizontalScrollBar()->value();

    int verticalScrollBarPos;
    verticalScrollBarPos = view->verticalScrollBar()->value();

    pixmapItem->setPixmap(QPixmap::fromImage(image));
    scene->setSceneRect(pixmapItem->sceneBoundingRect());
    view->horizontalScrollBar()->setValue(horizontalScrollBarPos);
    view->verticalScrollBar()->setValue(verticalScrollBarPos);
  }

  activePdfFilePath = pdfFilePath;
}

void PdfView::getWatchedFiles(void)
{
  watchedFiles = findFiles(workingDirectoryPath);
  lastModified = QList<QDateTime>();
  
  for (int i = 0; i < watchedFiles.size(); ++i) {
    QFileInfo info(watchedFiles[i]);
    lastModified.append(info.lastModified());
  }
}

void PdfView::updatePdfView(void)
{
  QByteArray stdoutByteArray = process->readAllStandardOutput();
  QByteArray stderrByteArray = process->readAllStandardError();
  QString standardOutput = QString(stdoutByteArray);
  QString error = QString(stderrByteArray);

  if (standardOutput.size() != 0) {
    errorOccured = false;
    QList<QString> lines = standardOutput.split("\n");
    lines.removeLast();

    for (int i = 0; i < lines.size(); i += 2) {
      QString key = lines[i];
      QString value = lines[i + 1];

      if (!pdfFilePaths.contains(key))
        pdfFilePaths.insert(key, value);

      pdfFilePaths.insert(key, value);

      if (value == activePdfFilePath)
        setPdfView(activePdfFilePath);
    }
  } else {
    errorOccured = true;
    errorMessage = error;
  }
}
