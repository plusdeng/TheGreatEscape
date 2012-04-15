#include <QtGui/QApplication>
#include "escapeview.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    EscapeView w;
    w.show();

    return a.exec();
}
