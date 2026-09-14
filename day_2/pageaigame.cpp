#include "pageaigame.h"

PageAIGame::PageAIGame(Ui::MainWindow *uiOwner, QObject *parent)
    : QObject(parent), ui(uiOwner)
{
    score = 0;
    ui->RL_score->display(score);
}
