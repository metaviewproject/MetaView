#ifndef METAVIEW_H
#define METAVIEW_H

#include <QMainWindow>
#include <QFileSystemWatcher>
#include <QGraphicsScene>
#include <QGraphicsView>

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow();

private slots:
  void addDirToWatch();
  void addFileToWatch();
  void fileWasChanged(const QString&);
  void dirWasChanged(const QString&);
  void quit();

private:
  void createActions();
  void createMenus();
  QImage loadImage(const QString&);
  void createView(const QString&);
  void createWatcher();

  QGraphicsScene *scene;
  QGraphicsView *view;

  QMenu *fileMenu;
  QAction *watchFileAction;
  QAction *watchDirAction;
  QAction *exitAction;

  QFileSystemWatcher *watcher;
};

#endif
