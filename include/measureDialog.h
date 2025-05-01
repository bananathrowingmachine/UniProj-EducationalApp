/**
 * The dialog box for measuring ingredients with the jigger.
 *
 * 
 *
 * Created by [redacted], [redacted], [redacted], bananathrowingmachine, and [redacted]
 *
 * Checked by [redacted]
 * April 22, 2025
 **/

#ifndef MEASUREDIALOG_H
#define MEASUREDIALOG_H

#include <QDialog>

namespace Ui {
class MeasureDialog;
}

class MeasureDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MeasureDialog(QWidget *parent = nullptr);

    ~MeasureDialog();

private:
    Ui::MeasureDialog *ui;

    QString ingredientName;
    /**
     * Emits a signal that the window has been closed.
     * @param event - Close event (unused)
     */
    void closeEvent(QCloseEvent *event) override;

public slots:

    /**
     * Sets the dialogue box title and ingredient to the given ingredient name.
     * @param ingredientName - Ingredient name
     */
    void measureDialogOpened(QString ingredientName);

private slots:

    /**
     * When the confirm button is pressed with a valid input, send the input to the main view and close this page.
     */
    void confirmButtonClicked();

    /**
     * If cancel is selected, close this page and do nothing.
     */
    void rejectedButton();

    /**
     * Hides the text telling the user to input a valid number.
     */
    void badNumText();

signals:

    /**
     * Sends the information entered into this dialog to the main window
     * @param amount - Ingredient amount
     * @param ingredientName - Ingredient name
     */
    void sendInput(float amount, QString ingredientName);

    /**
     * Emitted when the window is closed
     */
    void windowClosed();
};

#endif // F_H

