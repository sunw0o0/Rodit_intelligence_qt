#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>     // QSerial 기능 핵심 클래스
#include <QSerialPortInfo> // 시리얼 포트 정보 검색 클래스
#include <QTimer>          // 점멸 연출 및 NONE 출력 타이머
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_open_btn_clicked();   // OPEN / CLOSE 버튼
    void on_write_btn_clicked();  // WRITE 버튼
    void on_clear_btn_clicked();  // CLEAR 버튼
    void readData();              // 시리얼 수신(Rx) 처리 슬롯
    void onPollTimeout();         // 데이터 미수신 시 NONE 출력 슬롯
    void resetIndicatorStyle();   // 점멸 연출 스타일 복원 슬롯

private:
    void blinkIndicator(bool isTx);

    Ui::MainWindow *ui;
    QSerialPort *serial;          // QSerialPort 객체 포인터
    QTimer *pollTimer;            // NONE 출력 제어 타이머

    QWidget *targetWidget;
    QString originalStyle;
};

#endif // MAINWINDOW_H
