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

#include "measureDialog.h"
#include "ui_measureDialog.h"

MeasureDialog::MeasureDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MeasureDialog)
{
    ui->setupUi(this);

    //Connect buttons to slots
    connect(ui->confirmButton, &QDialogButtonBox::accepted, this, &MeasureDialog::confirmButtonClicked);
    connect(ui->confirmButton, &QDialogButtonBox::rejected, this, &MeasureDialog::rejectedButton);
    connect(ui->ozInput, &QLineEdit::textChanged, this, &MeasureDialog::badNumText);
    ui->invalidNumber->hide();
}

MeasureDialog::~MeasureDialog()
{
    delete ui;
}

void MeasureDialog::confirmButtonClicked(){

    QString input = ui->ozInput->text();

    bool isFloat;
    float numberInput = input.toFloat(&isFloat);

    // Case for invalid input
    if(!isFloat){
        ui->invalidNumber->show();
        return; // An int was not input so do nothing
    }

    // Send the input to to the view!
    emit sendInput(numberInput, ingredientName);
    this->hide();
}

void MeasureDialog::rejectedButton(){
    emit windowClosed();
    this->hide();
}

void MeasureDialog::badNumText(){
    ui->invalidNumber->hide();
}

void MeasureDialog::measureDialogOpened(QString ingredientName){
    ui->title->setText("How much " + ingredientName + " would you like to pour?");
    this->ingredientName = ingredientName;
}

void MeasureDialog::closeEvent(QCloseEvent *event){
    emit windowClosed();
}

