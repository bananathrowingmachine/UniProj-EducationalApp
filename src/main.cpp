/**
 * Main function of the program, starts the view and nothing else.
 *
 * 
 *
 * Created by [redacted], [redacted], [redacted], bananathrowingmachine, and [redacted]\
 *
 * Checked by [redacted]
 * April 22, 2025
 **/

#include "starterpage.h"
#include <QApplication>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    StarterPage w;

    QFontDatabase::addApplicationFont(":/fonts/Chalk-Regular.ttf");

    w.show();
    return a.exec();
}
