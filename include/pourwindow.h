#ifndef POURWINDOW_H
#define POURWINDOW_H

#include <QDialog>

namespace Ui {
class PourWindow;
}

class PourWindow : public QDialog
{
    Q_OBJECT

public:
    explicit PourWindow(QWidget *parent = nullptr);
    ~PourWindow();

private:
    Ui::PourWindow *ui;
    QString objectA;
    QString objectB;

    /**
     * Emits a signal that the window has been closed
     * @param event - Close event (unused)
     */
    void closeEvent(QCloseEvent *event) override;


public slots:
    /**
     * If pouring is confirmed, this will send a signal to pour the drink.
     */
    void pourConfirmed();

    /**
     * Gets the 2 objects being collided with.
     * @param object1 - one of the object's in the collision.
     * @param object2 - the other object in the collision.
     */
    void getStringsFromView(QString object1, QString object2);

signals:
    /**
     * Emitted when pouring has been confirmed.
     */
    void pourDrink(QString, QString);

    /**
     * Emitted when the window is closed
     */
    void windowClosed();
};


#endif // POURWINDOW_H
