#include <QAction>
#include <QComboBox>
#include <QDockWidget>
#include <QFileDialog>
#include <QIcon>
#include <QList>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QPushButton>
#include <QString>
#include <QTabWidget>
#include <QToolBar>
#include <QTimer>

#include "metaview.h"
#include "metaerror.h"

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:

  MainWindow();

public slots:

  void change(const QString&);
  void open();
  void update();
  void zoomIn();
  void zoomOut();

private:
  void createActions();
  void createErrorTabs();
  void createMenus();
  void createTimer();
  void createToolBar();

  MetaView *metaView;
  ErrorTab *errorTabLog;
  ErrorTab *errorTabMp;
  ErrorTab *errorTabMpx;
  QComboBox *epsFiles;
  QDockWidget *errorWindow;
  QList<QString> listOfEpsFiles;
  QString workingDirPath;
  QTabWidget *errorTabs;
  QTimer *timer;
  QToolBar *fileToolBar;
  QToolBar *editToolBar;
  QMenu *fileMenu;
  QMenu *editMenu;
  QAction *newFolderAct;
  QAction *zoomInAct;
  QAction *zoomOutAct;
  QAction *showErrorAct;
  QAction *closeAct;
};
