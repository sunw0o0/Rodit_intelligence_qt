#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void onGenerateArm();
    void onReset();
    void onJoint1Changed(int value);
    void onJoint2Changed(int value);
    void onJoint3Changed(int value);

private:
    Ui::MainWindow* ui;

    int m_joint1 = 0;
    int m_joint2 = 0;
    int m_joint3 = 0;
};
#endif // MAINWINDOW_H
