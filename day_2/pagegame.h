#ifndef PAGEGAME_H
#define PAGEGAME_H

#include <QObject>
#include <QTimer>
#include <QLabel>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QRandomGenerator> // 랜덤 바람 생성을 위한 Qt 난수 헤더
#include "bottlephysics.h"
#include "ui_mainwindow.h"

// PageGame 클래스
// - 역할: 수동 플레이 게임 화면 (슬라이더 자동 왕복, 던지기, 암전 효과, 바람 UI 표시, 점수 관리)
class PageGame : public QObject
{
    Q_OBJECT

public:
    explicit PageGame(Ui::MainWindow *uiOwner, QObject *parent = nullptr);

    void prepareGameStart(); // 게임 진입 시 초기화 및 슬라이더 작동
    void stopGame();         // 타이머 완전 정지

    // 버튼 클릭 핸들러 함수들
    void on_high_btn_clicked();
    void on_wspeed_btn_clicked();
    void on_GameThrow_btn_clicked();

private slots: //QT확장 문법-버튼이나 timer신호를 받고 동작할 수 있음
    void updateSliders(); // 10ms마다 슬라이더 2개 자동 왕복
    void animateBottle(); // 35ms마다 물병 물리 이동 및 착지 판정

private:
    void generateNewWind();      // 판마다 새로운 바람(방향/세기) 생성 함수
    void updateWindUI();        // 바람 UI 라벨(windDirection, windStrength) 갱신 함수
    void triggerSuccessEffect(); // 성공 시 1.5초 화면 그라데이션 암전
    void resetBottleToStart();   // 물병 출발 위치 복귀
    void renderBottleSprite();   // 회전 이미지 정중앙

    Ui::MainWindow *ui;
    QLabel *floorLabel;   // 바닥선
    QLabel *fadeOverlay;  // 성공 암전 연출

    QTimer *timer;        // 비행 물리 타이머
    QTimer *sliderTimer;  // 슬라이더 이동 타이머
    BottlePhysics physics; // 풍압이 적용된 물리 객체

    int dirHigh;          // 높이 슬라이더 왕복 방향
    int dirWspeed;        // 속도 슬라이더 왕복 방향
    bool highStopped;     // 높이 슬라이더 고정
    bool wspeedStopped;   // 속도 슬라이더 고정

    double startX;        // 물병 시작 X 좌표
    double startY;        // 물병 시작 Y 좌표
    int score;            // 현재 점수

    // 현재 판의 바람 데이터 (m/s 수치, 음수: 왼쪽 <-, 양수: 오른쪽 ->)
    double currentWindSpeed;
};

#endif // PAGEGAME_H
