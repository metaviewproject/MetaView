#ifndef ADDDIALOG_H
#define ADDDIALOG_H

#include <QDebug>
#include <QFileDialog>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QString>
#include <QVBoxLayout>
#include <QWidget>

class AddDialog : public QWidget
{
  Q_OBJECT

public:

  AddDialog(QWidget *parent = 0);

  QList<QString> fileExtensions;
  QString compilerPath;
  QString directoryPath;
  QString name;

public slots:

  void add(void);
  void findFileExtensions(void);
  void findCompilerPath(void);
  void findDirectoryPath(void);

signals:

  void added(void);

private:

  QGridLayout *gridLayout;
  QHBoxLayout *hBoxLayout;
  QLabel *compilerPathLabel;
  QLabel *directoryPathLabel;
  QLabel *fileExtensionsLabel;
  QLabel *nameLabel;
  QLineEdit *compilerPathLineEdit;
  QLineEdit *directoryPathLineEdit;
  QLineEdit *fileExtensionsLineEdit;
  QLineEdit *nameLineEdit;
  QPushButton *addPushButton;
  QPushButton *cancelPushButton;
  QPushButton *findCompilerPathPushButton;
  QPushButton *findDirectoryPathPushButton;
  QPushButton *findFileExtensionsPushButton;
  QString selectedExtensions;
  QVBoxLayout *vBoxLayout;
};

#endif
