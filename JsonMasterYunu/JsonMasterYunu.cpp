#include <QtCore>
#include <QtGui>
#include <fstream>
#include <string>
#include <regex>
#include <sstream>
#include "JsonMasterYunu.h"

using namespace std;

JsonMasterYunu::JsonMasterYunu(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    QObject::connect(&fileSystemWatcher, &QFileSystemWatcher::fileChanged, [this]() { OnFileModified(); });
    QObject::connect(&directoryWatcher, &QFileSystemWatcher::directoryChanged, [this]() { LoadJsonPath(selectedDirectory.toStdWString()); });
}

JsonMasterYunu::~JsonMasterYunu()
{
}

void JsonMasterYunu::ReadIni()
{
    ifstream file;
    const string filename = "setting.ini";
    file.open(filename);
    if (file.fail())
        return;

    string fileString;
    stringstream sstream;
    sstream << file.rdbuf();
    fileString = sstream.str();

    smatch match;
    regex_match(fileString, match, regex(".*filename\\s*=\\s*\"(.*?)\";.*"));
    if (match.size() >= 2)
    {
        QString qstring = QString::fromStdString(match[1].str());
        ui.pathTextEdit->setMarkdown(qstring);
        LoadJsonPath(qstring.toStdWString());
    }
    file.close();
}

void JsonMasterYunu::WriteIni()
{
    fstream file;
    const string filename = "setting.ini";
    file.open(filename, fstream::out | fstream::trunc);
    file << "filename = \"" + ui.pathTextEdit->toPlainText().toStdString() + "\";";
    file.close();
}

void JsonMasterYunu::LoadJsonPath(wstring path)
{
    ui.filenameList->clear();
    itemPathMap.clear();

    if (!filesystem::exists(filesystem::current_path() / path))
        return;
    for (auto& eachFile : filesystem::directory_iterator(filesystem::current_path() / path))
    {
        if (eachFile.path().extension() == ".json")
        {
            QListWidgetItem* item = new QListWidgetItem();
            QString itemName = QString::fromWCharArray(eachFile.path().filename().c_str());
            item->setText(itemName);
            itemPathMap[itemName] = eachFile.path().c_str();
            ui.filenameList->addItem(item);
        }
    }
    selectedDirectory = QString::fromStdWString(path);
    directoryWatcher.removePaths(directoryWatcher.files());
    directoryWatcher.addPath(QString::fromStdWString((filesystem::current_path() / path)));
    WriteIni();
}

void JsonMasterYunu::LoadJsonFile()
{
    LoadJsonFile(selectedItem->text().toStdWString());
}
void JsonMasterYunu::LoadJsonFile(wstring path)
{
    fstream file;
    const wstring filename = path;
    file.open(filename);
    if (file.fail())
        return;

    stringstream sstream;
    sstream << file.rdbuf();

    ui.jsonBrowser->setMarkdown(QString(sstream.str().c_str()));
    ui.warningLabel->setText("");
    file.close();
    fileSystemWatcher.removePaths(fileSystemWatcher.files());
    fileSystemWatcher.addPath(QString::fromStdWString(path.c_str()));
}

void JsonMasterYunu::OnFileModified()
{
    ui.warningLabel->setText("file modified from outside, click reload button to reload");
}

void JsonMasterYunu::on_reloadButton_clicked()
{
    //LoadJsonFile(ui.filenameList->selectedItems()toMarkdown().toStdString());
}
void JsonMasterYunu::on_filepathSubmitButton_clicked()
{
    LoadJsonPath(ui.pathTextEdit->toPlainText().toStdWString());
}
void JsonMasterYunu::on_filenameList_itemDoubleClicked(QListWidgetItem* item)
{
    LoadJsonFile(itemPathMap[item->text()]);
}
