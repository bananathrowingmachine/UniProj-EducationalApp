#ifndef INTRODUCTIONWINDOW_H
#define INTRODUCTIONWINDOW_H

#include <QDialog>

namespace Ui {
class IntroductionWindow;
}

class IntroductionWindow : public QDialog
{
    Q_OBJECT

public:
    explicit IntroductionWindow(QWidget *parent = nullptr);
    ~IntroductionWindow();

private:
    Ui::IntroductionWindow *ui;

public slots:
    void closeWindow();
};

#endif // INTRODUCTIONWINDOW_H
