#ifndef METAVIEW_H
#define METAVIEW_H

#include <poppler-qt4.h>
#include <QAbstractSlider>
#include <QByteArray>
#include <QCoreApplication>
#include <QDateTime>
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
#include <QSet>
#include <QString>
#include <QVBoxLayout>
#include <QWidget>

class MetaView : public QWidget
{
  Q_OBJECT

public:
  
  MetaView(const QString& = "", QWidget *parent = 0);

  QList<QString> compileMetapostFiles();
  QList<QString> findEpsFiles();
  void updateView(QString epsFileName = "");
  void zoomIn();
  void zoomOut();

private:
  QString epsFileNameToPdfFileName(QString);
  QImage loadImage(const QString&);
  QList<QString> findFilesByExtension(QString, QString);
  void constructDepGraph();
  int checkDepGraph(QString);
  void findMetapostFiles();
  
  double scaleFactor;
  int horizontalScrollBarPos;
  int verticalScrollBarPos;
  QGraphicsPixmapItem *pixmapItem;
  QGraphicsScene *metapostScene;
  QGraphicsView *metapostView;
  QList<QDateTime> modificationTimes;
  QList<QString> epsFiles;
  QList<QString> metapostFiles;
  QMap<QString, int> visited;
  QMap<QString, QDateTime> mapMpModTimes;
  QMap<QString, QList<QString> > depGraph;
  QSet<QString> forbiddenFiles;
  QString activeEpsFile;
  QString workingDirectoryPath;
  QVBoxLayout *layout;
};

#endif
