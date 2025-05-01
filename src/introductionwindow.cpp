#include "introductionwindow.h"
#include "ui_introductionwindow.h"

IntroductionWindow::IntroductionWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::IntroductionWindow)
{
    ui->setupUi(this);
    QString introText = "How To Play \n"
                        " - Start by picking a recipe from the dropdown menu. \n"
                        " - You can use the arrows to look through the steps for the selected recipe. \n"
                        " - To get the best score follow the recipe steps in the order they appear. \n"
                        " - To select the ingredient, you can click on the drink that you want to select. This will bring it up on the table. \n"
                        " - You can hover over a drink button to see exactly what type of ingredient it is. \n"
                        " - To measure a drink, you will drag the bottle to collide with the jigger(looks like an hour glass). You have to pour \n the drink here before pouring into the mixer. \n"
                        " - You will enter the amount you want to pour. \n"
                        " - When the jigger lights up blue, you can pour it into the mixer. You can only have one drink in the jigger at a time. \n"
                        " - To mix/shake the drink, click and hold on the mixer and manually shake it on the screen. \n"
                        " - The mixer will turn blue when it is fully shaken. \n"
                        " - You can drag the mixer onto the glass to pour the mixture into the glass. It will ask you if you want to confirm. \n"
                        " - Press confirm if you want to continue or close the window if you don’t want to pour yet. \n"
                        " - To add a garnish to the drink, click on a garnish button. The garnish will appear on the table. \n You can then drag it onto the glass to add it to the drink. \n"
                        " - The reset objects button will bring the mixer and the jigger back to their original positions but  \n not reset the contents in them. \n"
                        " - Restarting the game will allow you to pick a new recipe and start over. \n"
                        " - Press the submit drink button to get a score for your drink.";
    QString reasonText = "What you learn \n"
                        "   Our goal is to teach you how to learn to make a good drink. We will walk you through all of the steps to making \ncertain drinks. "
                        "Once you have finished making your drink and sumbit the drink, we wil give you feedback based \non how well you made the drink. "
                        "You'll get detailed explinations describing any of the mistakes you might have \nmade along the way. Once you feel confident enough, there will be an option to hide the recipe so you can test \nhow well you know how to make the drink.";
    ui->howToPlay->setText(introText);
    ui->whatYouLearn->setText(reasonText);

    connect(ui->continueButton, &QPushButton::clicked, this, &IntroductionWindow::closeWindow);
}

IntroductionWindow::~IntroductionWindow()
{
    delete ui;
}

void IntroductionWindow::closeWindow(){
    this->hide();
}
