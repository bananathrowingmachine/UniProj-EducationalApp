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

#include "starterpage.h"
#include "ui_starterpage.h"

StarterPage::StarterPage(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::StarterPage)
{
    ui->setupUi(this);
    QPixmap pixmap(":/background/woodBackground.jpg");
    ui->whatYouLearn->setScaledContents(true);
    ui->howToPlay->setScaledContents(true);
    ui->whatYouLearn->setPixmap(pixmap);
    ui->howToPlay->setPixmap(pixmap);
    QString introText = " <b>How To Play</b> <br>"
                        "  - Start by picking a recipe from the dropdown menu. <br>"
                        "  - You can use the arrows to look through the steps for the selected recipe. <br>"
                        "  - To get the best score follow the recipe steps in the order they appear. <br>"
                        "  - To select the ingredient, you can click on the drink that you want to select. This will bring it up on the table. <br>"
                        "  - You can hover over a drink button to see exactly what type of ingredient it is. <br>"
                        "  - To measure a drink, you will drag the bottle to collide with the jigger(looks like an hour glass). You have to pour the drink here before pouring into the mixer. <br>"
                        "  - You will enter the amount you want to pour. <br>"
                        "  - When the jigger lights up blue, you can pour it into the mixer. You can only have one drink in the jigger at a time. <br>"
                        "  - To mix/shake the drink, click and hold on the mixer and manually shake it on the screen. <br>"
                        "  - The mixer will turn blue when it is fully shaken. <br>"
                        "  - You can drag the mixer onto the glass to pour the mixture into the lass. It will ask you if you want to confirm. <br>"
                        "  - Press confirm if you want to continue or close the window if you don’t want to pour yet. <br>"
                        "  - To add a garnish to the drink, click on a garnish button. The garnish will appear on the table. You can then drag it onto the glass to add it to the drink. <br>"
                        "  - The reset objects button will bring the mixer and the jigger back to their original positions but not reset the contents in them. <br>"
                        "  - Restarting the game will let you to pick a new recipe and start over. <br>"
                        "  - Press the submit drink button to get a score for your drink.";
    QString reasonText = " <b>What you learn</b> <br>"
                         "   Our goal is to teach you how to learn to make a good drink. We will walk you through all of the steps to making certain drinks. "
                         "Once you have finished making your drink and sumbit the drink, we wil give you feedback based on how well you made the drink. "
                         "You'll get detailed explinations describing any of the mistakes you might have made along the way. "
                         "Once you feel confident enough, there will be an option to hide the recipe so you can test how well you know how to make the drink.";
    ui->howToPlay->setText(introText);
    ui->whatYouLearn->setText(reasonText);

    connect(ui->startGame, &QPushButton::clicked, this, &StarterPage::closeWindow);
}

StarterPage::~StarterPage()
{
    delete ui;
}

void StarterPage::closeWindow(){
    this->close();
    main.show();
}
