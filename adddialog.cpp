#include "adddialog.h"

AddDialog::AddDialog(QWidget *parent) : QWidget(parent, Qt::Window)
{
  nameLabel = new QLabel(tr("Name:"));
  fileExtensionsLabel = new QLabel(tr("File Extensions:"));
  compilerPathLabel = new QLabel(tr("Compiler Path:"));
  directoryPathLabel = new QLabel(tr("Directory Path:"));

  nameLineEdit = new QLineEdit;
  fileExtensionsLineEdit = new QLineEdit;
  compilerPathLineEdit = new QLineEdit;
  directoryPathLineEdit = new QLineEdit;

  findFileExtensionsPushButton = new QPushButton("Find");
  connect(findFileExtensionsPushButton, SIGNAL(clicked()), this, SLOT(findFileExtensions(void)));
  
  findCompilerPathPushButton = new QPushButton("Find");
  connect(findCompilerPathPushButton, SIGNAL(clicked()), this, SLOT(findCompilerPath(void)));
  
  findDirectoryPathPushButton = new QPushButton("Find");
  connect(findDirectoryPathPushButton, SIGNAL(clicked()), this, SLOT(findDirectoryPath(void)));

  addPushButton = new QPushButton("Add");
  connect(addPushButton, SIGNAL(clicked()), this, SLOT(add(void)));
  
  cancelPushButton = new QPushButton("Cancel");
  connect(cancelPushButton, SIGNAL(clicked()), this, SLOT(close()));

  hBoxLayout = new QHBoxLayout;
  hBoxLayout->addWidget(addPushButton);
  hBoxLayout->addWidget(cancelPushButton);

  gridLayout = new QGridLayout;
  gridLayout->addWidget(nameLabel, 0, 0);
  gridLayout->addWidget(nameLineEdit, 0, 1, 1, 2);
  gridLayout->addWidget(fileExtensionsLabel, 1, 0);
  gridLayout->addWidget(fileExtensionsLineEdit, 1, 1, 1, 1);
  gridLayout->addWidget(findFileExtensionsPushButton, 1, 2, 1, 1);
  gridLayout->addWidget(compilerPathLabel, 2, 0);
  gridLayout->addWidget(compilerPathLineEdit, 2, 1, 1, 1);
  gridLayout->addWidget(findCompilerPathPushButton, 2, 2, 1, 1);
  gridLayout->addWidget(directoryPathLabel, 3, 0);
  gridLayout->addWidget(directoryPathLineEdit, 3, 1, 1, 1);
  gridLayout->addWidget(findDirectoryPathPushButton, 3, 2, 1, 1);

  vBoxLayout = new QVBoxLayout;
  vBoxLayout->addLayout(gridLayout);
  vBoxLayout->addLayout(hBoxLayout);

  setLayout(vBoxLayout);
}

void AddDialog::add(void)
{
  name = nameLineEdit->displayText();
  compilerPath = compilerPathLineEdit->displayText();
  directoryPath = directoryPathLineEdit->displayText();
  
  fileExtensions = QList<QString>();
  QString fileExtension;
  for (int i = 0; i < selectedExtensions.size(); ++i) {
    if (selectedExtensions[i] != ',' && selectedExtensions[i] != ' ') {
      fileExtension.append(selectedExtensions[i]);
    } else if (selectedExtensions[i] == ',') {
      if (!fileExtensions.contains(fileExtension))
        fileExtensions.append(fileExtension); 
      fileExtension = QString();
    } else {
      continue;
    }
  }
  if (!fileExtensions.contains(fileExtension))
    fileExtensions.append(fileExtension);

  close();
  emit added();
}

void AddDialog::findFileExtensions(void)
{
  QString filePath;
  filePath = QFileDialog::getOpenFileName(this, QString(), QString(), QString());

  QString fileExtension;
  int i = filePath.size() - 1;
  
  for (; filePath[i] != '.'; i--);
  fileExtension = "*" + filePath.mid(i);

  if (selectedExtensions.size() == 0)
    selectedExtensions = fileExtension;
  else
    selectedExtensions += ", " + fileExtension;

  fileExtensionsLineEdit->setText(selectedExtensions);
  findFileExtensionsPushButton->setText("More");
}

void AddDialog::findCompilerPath(void)
{
  compilerPathLineEdit->setText(QFileDialog::getOpenFileName(this, QString(), QString(), QString()));
}

void AddDialog::findDirectoryPath(void)
{
  directoryPathLineEdit->setText(QFileDialog::getExistingDirectory(this, QString(), QString(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks));
}
