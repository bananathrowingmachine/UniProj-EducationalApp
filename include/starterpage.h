/**
 * The starting window for this application. Displays gameplay instructions
 * as well as what this application's purpose is.
 *
 * 
 *
 * Created by [redacted], [redacted], [redacted], bananathrowingmachine, and [redacted]
 *
 * Checked by [redacted]
 * April 22, 2025
 **/

#ifndef STARTERPAGE_H
#define STARTERPAGE_H

#include <QMainWindow>
#include "mainWindow.h"

namespace Ui {
class StarterPage;
}

class StarterPage : public QMainWindow
{
    Q_OBJECT

public:
    explicit StarterPage(QWidget *parent = nullptr);
    ~StarterPage();

private:
    Ui::StarterPage *ui;
    MainWindow main;
public slots:
    void closeWindow();
};

#endif // STARTERPAGE_H
