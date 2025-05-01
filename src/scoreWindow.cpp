/**
 * Window that shows the user's score.
 *
 * 
 *
 * Created by [redacted], [redacted], [redacted], bananathrowingmachine, and [redacted]
 * April 22, 2025
 **/

#include "scoreWindow.h"
#include "ui_scoreWindow.h"

ScoreWindow::ScoreWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ScoreWindow)
{
    ui->setupUi(this);
    connect(ui->restart, &QPushButton::clicked, this, &ScoreWindow::restartGame);
}

ScoreWindow::~ScoreWindow()
{
    delete ui;
}

void ScoreWindow::showScore(int score, QString explination){
    ui->userScore->setText(QString::number(score));
    ui->explinationText->setText(explination);
}

void ScoreWindow::restartGame(){
    emit sendingRestartGame();
}
