#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "armcanvaswidget.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QIcon icon(":/image/images/icon.png");
    setWindowIcon(icon);

    ui->link1_dsb->setRange(1, 100);
    ui->link2_dsb->setRange(1, 100);
    ui->link3_dsb->setRange(1, 100);

    ui->joint1_hs->setRange(0, 360);
    ui->joint2_hs->setRange(0, 360);
    ui->joint3_hs->setRange(0, 360);

    connect(ui->ga_btn, &QPushButton::clicked, this, &MainWindow::onGenerateArm);
    connect(ui->reset_btn, &QPushButton::clicked, this, &MainWindow::onReset);

    connect(ui->joint1_hs, &QSlider::valueChanged, this, &MainWindow::onJoint1Changed);
    connect(ui->joint2_hs, &QSlider::valueChanged, this, &MainWindow::onJoint2Changed);
    connect(ui->joint3_hs, &QSlider::valueChanged, this, &MainWindow::onJoint3Changed);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onGenerateArm()
{
    ui->centralwidget->setLinks(
        ui->link1_dsb->value(),
        ui->link2_dsb->value(),
        ui->link3_dsb->value());
    ui->centralwidget->setArmGenerated(true);
}

void MainWindow::onReset()
{
    ui->centralwidget->setArmGenerated(false);

    m_joint1 = m_joint2 = m_joint3 = 0;
    ui->joint1_hs->setValue(0);
    ui->joint2_hs->setValue(0);
    ui->joint3_hs->setValue(0);

    ui->joint1_qe->clear();
    ui->joint2_qe->clear();
    ui->joint3_qe->clear();
}

void MainWindow::onJoint1Changed(int value)
{
    m_joint1 = value;
    ui->joint1_qe->setText(QString::number(value));
    ui->centralwidget->setJoints(m_joint1, m_joint2, m_joint3);
}

void MainWindow::onJoint2Changed(int value)
{
    m_joint2 = value;
    ui->joint2_qe->setText(QString::number(value));
    ui->centralwidget->setJoints(m_joint1, m_joint2, m_joint3);
}

void MainWindow::onJoint3Changed(int value)
{
    m_joint3 = value;
    ui->joint3_qe->setText(QString::number(value));
    ui->centralwidget->setJoints(m_joint1, m_joint2, m_joint3);
}
