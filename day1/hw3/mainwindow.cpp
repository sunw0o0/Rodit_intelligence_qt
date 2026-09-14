#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    serial(new QSerialPort(this)),
    pollTimer(new QTimer(this)),
    targetWidget(nullptr)
{
    this->ui->setupUi(this);

    // 1. 시리얼 장치 자동 검색 (ttyS 예약 포트 제외, USB/ACM/pts/COM 포함)
    this->ui->device_comboBox->clear();
    const auto &ports = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &port : ports) {
        if (!port.portName().startsWith("ttyS")) {
            this->ui->device_comboBox->addItem(port.portName());
        }
    }

    // 포트 목록이 없는 경우 NONE 표시
    if (this->ui->device_comboBox->count() == 0)
    {
        this->ui->device_comboBox->addItem("NONE");
    }

    // 2. 보드레이트 옵션 목록 구성
    this->ui->baudrate_comboBox->clear();
    this->ui->baudrate_comboBox->addItems({"9600", "115200", "57600", "19200", "38400"});

    // 3. 수신(readyRead) 신호 및 NONE 출력 타이머 슬롯 연결
    connect(this->serial, &QSerialPort::readyRead, this, &MainWindow::readData);
    connect(this->pollTimer, &QTimer::timeout, this, &MainWindow::onPollTimeout);
}

MainWindow::~MainWindow()
{
    if (this->pollTimer->isActive())
    {
        this->pollTimer->stop();
    }
    if (this->serial != nullptr && this->serial->isOpen())
    {
        this->serial->close();
    }
    delete this->serial;
    delete this->ui;
}

// OPEN / CLOSE 버튼 동작
void MainWindow::on_open_btn_clicked()
{
    if (!this->serial->isOpen())
    {
        QString portName = this->ui->device_comboBox->currentText();
        if (portName.isEmpty() || portName == "NONE")
        {
            QMessageBox::warning(this, "경고", "연결 가능한 유효 시리얼 장치가 없습니다!");
            return;
        }

        this->serial->setPortName(portName);
        this->serial->setBaudRate(this->ui->baudrate_comboBox->currentText().toInt());
        this->serial->setDataBits(QSerialPort::Data8);
        this->serial->setParity(QSerialPort::NoParity);
        this->serial->setStopBits(QSerialPort::OneStop);
        this->serial->setFlowControl(QSerialPort::NoFlowControl);

        if (this->serial->open(QIODevice::ReadWrite)) {
            this->ui->open_btn->setText("CLOSE");
            // 포트 열림 성공 시 1초 주기 NONE 출력 타이머 동작
            this->pollTimer->start(1000);
        } else {
            QMessageBox::critical(this, "오류", "시리얼 포트를 열 수 없습니다!");
        }
    } else {
        this->pollTimer->stop();
        this->serial->close();
        this->ui->open_btn->setText("OPEN");
    }
}

// WRITE (송신 Tx) 버튼 동작
void MainWindow::on_write_btn_clicked()
{
    if (!this->serial->isOpen())
    {
        QMessageBox::warning(this, "경고", "먼저 시리얼 포트를 오픈하세요!");
        return;
    }

    QString sendStr = this->ui->lineEdit->text();
    if (sendStr.isEmpty()) return;

    this->serial->write(sendStr.toUtf8());

    // 송신 점멸 (파란색)
    this->blinkIndicator(true);

    // Clear when write 옵션 적용
    if (this->ui->CWW_checkBox->isChecked())
    {
        this->ui->lineEdit->clear();
    }
}

// 수신 데이터 처리
void MainWindow::readData()
{
    QByteArray data = this->serial->readAll();
    QString strData = QString::fromUtf8(data).trimmed();

    if (!strData.isEmpty())
    {
        // 과제 예시 형태 (READ: hi)
        this->ui->textEdit->append("READ: " + strData);

        // Rx 수신 점멸 (초록색)
        this->blinkIndicator(false);

        // Auto Scroll 옵션 적용
        if (this->ui->AS_checkBox->isChecked())
        {
            this->ui->textEdit->moveCursor(QTextCursor::End);
        }
    }
}

// 데이터 미수신 시 periodic NONE 출력
void MainWindow::onPollTimeout()
{
    if (this->serial->isOpen())
    {
        if (this->serial->bytesAvailable() == 0)
        {
            this->ui->textEdit->append("NONE");
            if (this->ui->AS_checkBox->isChecked())
            {
                this->ui->textEdit->moveCursor(QTextCursor::End);
            }
        }
    }
}

// CLEAR 버튼 동작
void MainWindow::on_clear_btn_clicked()
{
    this->ui->textEdit->clear();
}

// Rx/Tx 시각적 점멸 연출 (Tx: 파란색, Rx: 초록색)
void MainWindow::blinkIndicator(bool isTx)
{
    this->targetWidget = isTx ? static_cast<QWidget*>(this->ui->write_btn) : static_cast<QWidget*>(this->ui->textEdit);
    this->originalStyle = this->targetWidget->styleSheet();

    if (isTx) {
        this->targetWidget->setStyleSheet("background-color: blue; color: white;");
    } else {
        this->ui->textEdit->setStyleSheet("background-color: #002B00; color: #00FF00; font-family: Monospace;");
    }

    QTimer::singleShot(100, this, &MainWindow::resetIndicatorStyle);
}

// 스타일 원복
void MainWindow::resetIndicatorStyle()
{
    if (this->targetWidget != nullptr)
    {
        this->targetWidget->setStyleSheet(this->originalStyle);
    }
}
