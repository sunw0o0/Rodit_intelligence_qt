#ifndef PAGEAIGAME_H
#define PAGEAIGAME_H

#include <QObject>
#include "ui_mainwindow.h"

// PageAIGame 클래스
// - 역할: AI 강화학습 모드 화면 제어 (추후 AI 알고리즘 작성 구역)
class PageAIGame : public QObject
{
    Q_OBJECT

public:
    explicit PageAIGame(Ui::MainWindow *uiOwner, QObject *parent = nullptr);

private:
    Ui::MainWindow *ui;
    int score;
};

#endif // PAGEAIGAME_H
