#include "JsonMasterYunu.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    JsonMasterYunu w;
    w.show();
    w.ReadIni();
    return a.exec();
}
