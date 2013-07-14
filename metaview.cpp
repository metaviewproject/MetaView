#include <QDebug>
#include "metaview.h"

MetaView::MetaView(const QString& path, QWidget *parent) : QWidget(parent)
{
  workingDirectoryPath = path;
  findMetapostFiles();
  constructDepGraph();

  QFile mvfile(workingDirectoryPath + "/METAVIEW");

  if (mvfile.open(QFile::ReadOnly | QFile::Text)) {
    while (!mvfile.atEnd()) {
      QByteArray line = mvfile.readLine();
      QString forbiddenFile(line);
      forbiddenFile.chop(1);
      forbiddenFiles.insert(workingDirectoryPath + "/" + forbiddenFile);
    }
  }

  scaleFactor = 1.0;
  metapostView = NULL;
}

void MetaView::updateView(QString epsFile)
{
  if (epsFile == "" && activeEpsFile == "")
    return;
  else {
    if (epsFile != "" && activeEpsFile != "") {
      if (epsFile != activeEpsFile) {
        activeEpsFile = epsFile;
        scaleFactor = 1.0;
      }
    } else if (epsFile == "" && activeEpsFile != "") {
      epsFile = activeEpsFile;
    } else {
      activeEpsFile = epsFile;
    }

    QList<QString> arguments;
    arguments.append(epsFile);
    arguments.append(workingDirectoryPath);
    arguments.append(QString::number(scaleFactor));
    
    QProcess *process = new QProcess(this);
    process->start(QCoreApplication::applicationDirPath() + "/epspdf.py", arguments);
    process->waitForFinished();

    QString pdfFile = epsFileNameToPdfFileName(epsFile);
 
    QImage metapostImage;
    metapostImage = loadImage(pdfFile);

    if (metapostView == NULL) {
      metapostScene = new QGraphicsScene(this);
      pixmapItem = metapostScene->addPixmap(QPixmap::fromImage(metapostImage));

      metapostView = new QGraphicsView(metapostScene);

      layout = new QVBoxLayout(this);
      layout->addWidget(metapostView);
      setLayout(layout);
    } else {
      horizontalScrollBarPos = metapostView->horizontalScrollBar()->value();
      verticalScrollBarPos = metapostView->verticalScrollBar()->value();
     
      pixmapItem->setPixmap(QPixmap::fromImage(metapostImage));
      metapostScene->setSceneRect(pixmapItem->sceneBoundingRect());
      metapostView->horizontalScrollBar()->setValue(horizontalScrollBarPos);
      metapostView->verticalScrollBar()->setValue(verticalScrollBarPos);
    }
  }
}

QImage MetaView::loadImage(const QString& path)
{
  Poppler::Document *pdfDocument;
  pdfDocument = Poppler::Document::load(path);
  pdfDocument->setRenderHint(Poppler::Document::Antialiasing);
  pdfDocument->setRenderHint(Poppler::Document::TextAntialiasing);

  Poppler::Page *pdfPage;
  pdfPage = pdfDocument->page(0);

  QImage metapostImage;
  metapostImage = pdfPage->renderToImage(144.0, 144.0, -1, -1, -1, -1);

  return metapostImage;
}

QString MetaView::epsFileNameToPdfFileName(QString epsFileName)
{
  QString pdfFileName;
  pdfFileName = workingDirectoryPath + QString("/.metaview/");
  
  int pos;
  pos = epsFileName.size() - 1;
  while (epsFileName[pos] != '/')
    pos--;
  
  epsFileName.remove(0, pos + 1);
  epsFileName.remove(epsFileName.size() - 4, 4);
  pdfFileName += epsFileName + QString(".pdf");

  return pdfFileName;
}

QList<QString> MetaView::findEpsFiles()
{
  epsFiles = findFilesByExtension(workingDirectoryPath, "*.eps");

  return epsFiles;
}

void MetaView::findMetapostFiles()
{
  metapostFiles = findFilesByExtension(workingDirectoryPath, "*.mp");
  modificationTimes = QList<QDateTime>();
  mapMpModTimes = QMap<QString, QDateTime>();
  visited = QMap<QString, int>();

  for (int i = 0; i < metapostFiles.size(); i++) {
    QFileInfo info(metapostFiles[i]);
    modificationTimes.append(info.lastModified());
    mapMpModTimes.insert(metapostFiles[i], modificationTimes[i]);
    visited.insert(metapostFiles[i], -1);
  }
}

QList<QString> MetaView::compileMetapostFiles()
{
  bool somethingWasCompiled = false;
  QList<QString> filesWithError;
  
  for (int i = 0; i < metapostFiles.size(); i++) {
    QFileInfo info(metapostFiles[i]);
    QDateTime currentTime = info.lastModified();

    int cdg = checkDepGraph(metapostFiles[i]);
    if (!forbiddenFiles.contains(metapostFiles[i]) && cdg > 0) {
      somethingWasCompiled = true;
      
      QList<QString> arguments;
      arguments.append(metapostFiles[i]);
      
      QProcess *process = new QProcess(this);
      process->start(QCoreApplication::applicationDirPath() + "/mpeps.py", arguments);
      process->waitForFinished();

      if (process->exitCode() != 0) {
        filesWithError.append(metapostFiles[i]);
        somethingWasCompiled = false;
      }
    }
  }

  QSet<QString>::iterator it = forbiddenFiles.begin();

  while (it != forbiddenFiles.end()) {
    QFileInfo info(*it);
    mapMpModTimes[*it] = info.lastModified();
    ++it;
  }
  
  if (somethingWasCompiled) {
    /*QSet<QString>::iterator it = forbiddenFiles.begin();
  
    while (it != forbiddenFiles.end()) {
      QFileInfo info(*it);
      mapMpModTimes[*it] = info.lastModified();
      ++it;
    }*/

    updateView();
    findMetapostFiles();
    constructDepGraph();
  }

  return filesWithError;
}

QList<QString> MetaView::findFilesByExtension(QString directoryPath, QString extension)
{
  QDir directory(directoryPath);

  QList<QString> extensionFiles;
  extensionFiles = directory.entryList(QStringList(extension));
  
  for (int i = 0; i < extensionFiles.size(); i++)
    extensionFiles[i] = directoryPath + "/" + extensionFiles[i];

  directory.setFilter(QDir::AllDirs | QDir::NoDotAndDotDot);

  QList<QString> subdirectories;
  subdirectories = directory.entryList();

  if (subdirectories.size() == 0)
    return extensionFiles;
  else {
    for (int i = 0; i < subdirectories.size(); i++)
      extensionFiles += findFilesByExtension(directoryPath + "/" + subdirectories[i], extension);
    return extensionFiles;
  }
}

void MetaView::zoomIn()
{
  scaleFactor += 0.25;
  
  updateView();
}

void MetaView::zoomOut()
{
  scaleFactor -= 0.25;
  
  updateView();
}

void MetaView::constructDepGraph()
{
  for (int i = 0; i < metapostFiles.size(); i++) {
    QFile mpfile(metapostFiles[i]);
    QList<QString> inputFiles;
    
    if (mpfile.open(QFile::ReadOnly | QFile::Text)) {
      while (!mpfile.atEnd()) {
        QByteArray byteArray = mpfile.readLine();
        QString line(byteArray);
        
        if (line.left(5) == "input") {
          line.remove(0, 6);
          line.chop(2);

          QString path = metapostFiles[i];
          int j = path.size() - 1;
          int pos = 0;
          while (path[j] != '/') {
            j--;
            pos++;
          }
          path.chop(pos);

          QFile inputfile(path + "/" + line + ".mp");
          QFileInfo inputfileInfo(inputfile);
          inputFiles.append(inputfileInfo.absoluteFilePath());
        }
      }
    }

    depGraph.insert(metapostFiles[i], inputFiles);
  }
}

int MetaView::checkDepGraph(QString metapostFile)
{
  QFileInfo info(metapostFile);
  QDateTime currentTime = info.lastModified();
  QDateTime lastTime = mapMpModTimes[metapostFile];

  QList<QString> inputFiles;
  inputFiles = depGraph[metapostFile];

  if (!forbiddenFiles.contains(metapostFile))
    mapMpModTimes[metapostFile] =  currentTime;

  if (inputFiles.size() == 0) {
    visited[metapostFile] = currentTime > lastTime;
    return currentTime > lastTime;
  } else {
    int nmodified = 0;

    for (int i = 0; i < inputFiles.size(); i++) {
      int wasVisited = visited[inputFiles[i]];
      
      if (wasVisited == -1)
        nmodified += checkDepGraph(inputFiles[i]);
      else
        nmodified += wasVisited;
    }

    if (currentTime > lastTime)
      nmodified++;

    visited[metapostFile] = nmodified > 0;
    return nmodified > 0;
  }
}
