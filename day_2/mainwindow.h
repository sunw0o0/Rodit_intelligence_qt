#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "pagemodeselect.h"
#include "pagegame.h"
#include "pageaigame.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// MainWindow 클래스
// 역할: 최상위 최상단 관리자 (페이지 전환 및 버튼 연결 통로)
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    // UI 버튼 연결
    void on_GameStart_btn_clicked();
    void on_AiGameStart_btn_clicked();
    void on_high_btn_clicked();
    void on_wspeed_btn_clicked();
    void on_GameThrow_btn_clicked();
    void on_GameStop_btn_clicked();
    void on_RLStop_btn_clicked();

private:
    void switchToMainPage(); // 메인 화면 복귀 제어

    Ui::MainWindow *ui;

    // 분리된 3개 화면 컨트롤러 포인터
    PageModeSelect *pageModeSelectController;
    PageGame *pageGameController;
    PageAIGame *pageAiGameController;
};

#endif // MAINWINDOW_H
