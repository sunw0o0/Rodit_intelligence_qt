#ifndef PAGEMODESELECT_H
#define PAGEMODESELECT_H

#include <QObject>
#include <QTimer>
#include "bottlephysics.h"
#include "ui_mainwindow.h"

// PageModeSelect 클래스
// - 역할: 타이틀 메인 화면 제어 (배경 포물선 물리 연출, 화면 전환 요청)
class PageModeSelect : public QObject
{
    Q_OBJECT

public:
    explicit PageModeSelect(Ui::MainWindow *uiOwner, QObject *parent = nullptr);
    void startAnimation(); // 메인 화면 물병 회전 애니메이션 시작
    void stopAnimation();  // 게임 진입 시 애니메이션 정지

private slots:
    void animateBottle();  // 예시용 무한 포물선 연출 슬롯

private:
    Ui::MainWindow *ui;     // 메인 UI 포인터 참조
    QTimer *timer;          // 메인 화면 연출용 타이머
    BottlePhysics physics;  // 예시 연출용 물리 객체

    double startX;
    double startY;
};

#endif // PAGEMODESELECT_H
