#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // QStackedWidget 크기 조정
    ui->stackedWidget->setGeometry(0, 0, 800, 550);

    // 3개 화면 컨트롤러 인스턴스 생성
    pageModeSelectController = new PageModeSelect(ui, this);
    pageGameController = new PageGame(ui, this);
    pageAiGameController = new PageAIGame(ui, this);

    // 초기 화면(메인 메뉴) 세팅
    switchToMainPage();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 버튼 이벤트 중계 (각 페이지 컨트롤러로 신호 전달)

// 게임 시작 버튼 누름 -> 일반 게임 화면 전환
void MainWindow::on_GameStart_btn_clicked()
{
    pageModeSelectController->stopAnimation();

    ui->stackedWidget->setCurrentWidget(ui->pageGame);

    // 물병 위젯을 pageGame으로 부모 이동
    ui->label->setParent(ui->pageGame);
    ui->label->move(470, 140);
    ui->label->show();
    ui->label->raise();

    pageGameController->prepareGameStart();
}

// 학습 게임 시작 버튼 누름 -> AI 게임 화면 전환
void MainWindow::on_AiGameStart_btn_clicked()
{
    pageModeSelectController->stopAnimation();
    ui->stackedWidget->setCurrentWidget(ui->pageAIGame);
}

// 높이 버튼
void MainWindow::on_high_btn_clicked()
{
    pageGameController->on_high_btn_clicked();
}

// 속도 버튼
void MainWindow::on_wspeed_btn_clicked()
{
    pageGameController->on_wspeed_btn_clicked();
}

// 던지기 버튼
void MainWindow::on_GameThrow_btn_clicked()
{
    pageGameController->on_GameThrow_btn_clicked();
}

// 일반 게임 그만하기 버튼 -> 메인 화면 복귀
void MainWindow::on_GameStop_btn_clicked()
{
    pageGameController->stopGame();
    switchToMainPage();
}

// AI 학습 그만하기 버튼 -> 메인 화면 복귀
void MainWindow::on_RLStop_btn_clicked()
{
    switchToMainPage();
}

// 메인 메뉴 화면으로 복귀 처리
void MainWindow::switchToMainPage()
{
    pageGameController->stopGame();

    // 물병 위젯 부모를 pageModeSelect로 이관
    ui->label->setParent(ui->pageModeSelect);
    ui->label->move(470, 140);
    ui->label->show();
    ui->label->raise();

    QString path = QDir::homePath() + "/keyboard_project/images/bottle.png";
    QPixmap pix(path);
    if (!pix.isNull()) ui->label->setPixmap(pix);

    ui->stackedWidget->setCurrentWidget(ui->pageModeSelect);
    pageModeSelectController->startAnimation();
}
