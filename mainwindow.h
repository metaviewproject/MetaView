#ifndef METAVIEW_H
#define METAVIEW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDateTime>
#include <QStringList>
#include <QComboBox>
#include <QTextEdit>

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow();

public slots:
  void update();
  void reloadView(const QString&);

private:
  void createToolBar();
  QImage loadImage(const QString&);
  void createView(const QString&);
  void createTimer();

  QToolBar *toolBar;
  QComboBox *mpFiles;
  QString activeEpsFile;
  QStringList listOfEpsFiles;
  QTextEdit *textEdit;

  QGraphicsScene *scene;
  QGraphicsView *view;

  QTimer *timer;

  QStringList listOfMetapostFiles;
  QList<QDateTime> modificationTimes;
};

#endif
