#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    // Qt 애플리케이션 생성 및 설정
    QApplication a(argc, argv);

    // 메인 화면( 개 화면 관리자) 객체 생성
    MainWindow w;

    // 화면에 화면 창 띄우기
    w.show();

    // 애플리케이션 이벤트 루프 시작 (창이 닫힐 때까지 대기)
    return a.exec();
}
