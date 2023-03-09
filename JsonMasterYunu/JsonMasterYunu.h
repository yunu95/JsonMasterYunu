#pragma once

#include <QtWidgets/QMainWindow>
#include <qfilesystemwatcher.h>
#include "ui_JsonMasterYunu.h"
using namespace std;

class JsonMasterYunu : public QMainWindow
{
    Q_OBJECT

public:
    JsonMasterYunu(QWidget* parent = nullptr);
    ~JsonMasterYunu();

    void ReadIni();
private:
    Ui::JsonMasterYunuClass ui;
    void WriteIni();
    void LoadJsonPath(wstring path);
    void LoadJsonFile(wstring path);
    void LoadJsonFile();
    void OnFileModified();
    map<QString, wstring> itemPathMap;
    QFileSystemWatcher fileSystemWatcher;
    QFileSystemWatcher directoryWatcher;
    QListWidgetItem* selectedItem = nullptr;
    QString selectedDirectory;
private slots:
    void on_reloadButton_clicked();
    void on_filepathSubmitButton_clicked();
    void on_filenameList_itemDoubleClicked(QListWidgetItem* item);

};
