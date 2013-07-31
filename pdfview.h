#ifndef PDFVIEW_H
#define PDFVIEW_H

#include <poppler-qt4.h>
#include <QByteArray>
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QImage>
#include <QList>
#include <QMap>
#include <QProcess>
#include <QScrollBar>
#include <QString>
#include <QVBoxLayout>
#include <QWidget>

class PdfView : public QWidget
{
  Q_OBJECT

public:

  PdfView(QString compilerPath, QList<QString> fileExtensions, QString workingDirectoryPath, QWidget *parent = 0);
  
  void compile(void);
  QList<QString> getPdfFileNames(void);
  void setPdfView(const QString& pdfFilePath);
  void setWorkingDirectoryPath(const QString& workindDirectoryPath);
  
  QMap<QString, QString> pdfFilePaths;

  bool errorOccured;
  bool somethingWasChanged;
  QString errorMessage;
  QString compilerPath;
  QString workingDirectoryPath;
  QList<QString> fileExtensions;

private:
  
  QImage loadImage(const QString& pdfFilePath);
  QList<QString> findFiles(QString directoryPath);
  void getWatchedFiles(void);

  QGraphicsPixmapItem *pixmapItem;
  QGraphicsScene *scene;
  QGraphicsView *view;
  QList<QDateTime> lastModified;
  QList<QString> watchedFiles;
  QProcess *process;
  QString activePdfFilePath;
  QVBoxLayout *layout;

private slots:
  
  void updatePdfView(void);
};

#endif
