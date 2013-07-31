#ifndef INSTANTVIEWERWINDOW_H
#define INSTANTVIEWERWINDOW_H

#include <QAction>
#include <QByteArray>
#include <QComboBox>
#include <QDebug>
#include <QDir>
#include <QDockWidget>
#include <QIcon>
#include <QMainWindow>
#include <QMap>
#include <QTextEdit>
#include <QTimer>
#include <QToolBar>

#include "adddialog.h"
#include "pdfview.h"

class InstantViewerWindow : public QMainWindow
{
  Q_OBJECT

public:

  InstantViewerWindow(void);
  ~InstantViewerWindow(void);

public slots:

  void addPdfView(void);
  void chooseActivePdfFile(const QString& pdfFileName);
  void chooseActivePdfView(const QString& pdfViewName);
  void findActivePdfViewWorkingDirectory(void);
  void update(void);

private:
  void createActions(void);
  void createDockWidget(void);
  void createTimer(void);
  void createToolBars(void);

  bool error;
  AddDialog *addDialog;
  PdfView *activePdfView;
  QAction *addPdfViewAction;
  QAction *chooseDirectoryAction;
  QComboBox *pdfFilesComboBox;
  QComboBox *pdfViewComboBox;
  QDockWidget *errorDockWidget;
  QMap<QString, PdfView *> pdfViews;
  QString activePdfViewName;
  QTextEdit *errorView;
  QTimer *timer;
  QToolBar *configToolBar;
  QToolBar *pdfViewToolBar;
};

#endif
