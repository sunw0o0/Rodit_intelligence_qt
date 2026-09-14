#include "pagegame.h"
#include <QPixmap>
#include <QDir>
#include <QTransform>
#include <cmath>

PageGame::PageGame(Ui::MainWindow *uiOwner, QObject *parent)
    : QObject(parent), ui(uiOwner)
{
    score = 0;
    startX = 470;
    startY = 140;
    currentWindSpeed = 0.0;

    // 1. 바닥선 라벨 동적 생성
    floorLabel = new QLabel(ui->pageGame);
    int floorY = static_cast<int>(startY) + ui->label->height();
    floorLabel->setGeometry(0, floorY, 800, 4);
    floorLabel->setStyleSheet("background-color: black; border-radius: 2px;");
    floorLabel->lower();
    floorLabel->show();

    // 2. 성공 암전 레이어 동적 생성 (클릭 방해 없는 속성 적용)
    fadeOverlay = new QLabel(ui->pageGame);
    fadeOverlay->setGeometry(0, 0, 800, 550);
    fadeOverlay->setStyleSheet("background-color: black;");
    // 마우스 클릭 이벤트가 아래 버튼으로 전달되도록 설정
    fadeOverlay->setAttribute(Qt::WA_TransparentForMouseEvents);

    QGraphicsOpacityEffect *fadeEffect = new QGraphicsOpacityEffect(fadeOverlay);
    fadeEffect->setOpacity(0.0); // 평소엔 완전 투명
    fadeOverlay->setGraphicsEffect(fadeEffect);
    fadeOverlay->raise();
    fadeOverlay->show();

    // 3. 타이머 생성 및 생성자 초기화
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &PageGame::animateBottle);

    sliderTimer = new QTimer(this);
    connect(sliderTimer, &QTimer::timeout, this, &PageGame::updateSliders);

    dirHigh = 1;
    dirWspeed = 1;
    highStopped = false;
    wspeedStopped = false;
}

// 게임 진입 시 시작 상태 세팅
void PageGame::prepareGameStart()
{
    resetBottleToStart();
    highStopped = false;
    wspeedStopped = false;
    dirHigh = 1;
    dirWspeed = 1;

    // 새로운 게임 시작 시 바람 새로 생성 및 UI 표시
    generateNewWind();

    sliderTimer->start(10); // 슬라이더 자동 왕복 시작
}

// 화면 이탈 및 그만두기 시 타이머 멈춤
void PageGame::stopGame()
{
    if (timer->isActive()) timer->stop();
    if (sliderTimer->isActive()) sliderTimer->stop();
}

// 랜덤 바람 생성 함수 (-10.0 m/s ~ +10.0 m/s)
void PageGame::generateNewWind()
{
    // -100 ~ 100 사이의 임의의 정수를 뽑은 뒤 10.0으로 나누어 소수점 1자리 수치 생성
    int rawValue = QRandomGenerator::global()->bounded(201) - 100;
    currentWindSpeed = rawValue / 10.0;

    // 바람 UI 갱신
    updateWindUI();
}

// 바람 UI 라벨 (windDirection, windStrength) 갱신 함수
void PageGame::updateWindUI()
{
    if (ui->windDirection == nullptr || ui->windStrength == nullptr) return;

    // 1. 바람 방향 (windDirection) 표시: 양수 -> (오른쪽), 음수 <- (왼쪽)
    if (currentWindSpeed > 0.0)
    {
        ui->windDirection->setText("->");
        ui->windDirection->setStyleSheet("color: blue; font-weight: bold;");
    }
    else if (currentWindSpeed < 0.0)
    {
        ui->windDirection->setText("<-");
        ui->windDirection->setStyleSheet("color: red; font-weight: bold;");
    }
    else
    {
        ui->windDirection->setText("-");
        ui->windDirection->setStyleSheet("color: black;");
    }

    // 2. 바람 세기 (windStrength) 표시: 절댓값 m/s 단위 표기
    double absSpeed = std::abs(currentWindSpeed);
    QString strengthText = QString::number(absSpeed, 'f', 1) + " m/s";
    ui->windStrength->setText(strengthText);
}

// 높이 & 회전속도 슬라이더 자동 이동 연산
void PageGame::updateSliders()
{
    // 높이 슬라이더 왕복
    if (!highStopped && ui->high_slider != nullptr)
    {
        int valH = ui->high_slider->value();
        if (valH >= ui->high_slider->maximum()) dirHigh = -1;
        if (valH <= ui->high_slider->minimum()) dirHigh = 1;
        ui->high_slider->setValue(valH + (dirHigh * 3));
    }

    // 회전속도 슬라이더 왕복
    if (!wspeedStopped && ui->wspeed_slider != nullptr)
    {
        int valW = ui->wspeed_slider->value();
        if (valW >= ui->wspeed_slider->maximum()) dirWspeed = -1;
        if (valW <= ui->wspeed_slider->minimum()) dirWspeed = 1;
        ui->wspeed_slider->setValue(valW + (dirWspeed * 5));
    }
}

// 높이 버튼 누름 -> 높이 슬라이더 고정
void PageGame::on_high_btn_clicked()
{
    highStopped = true;
}

// 회전속도 버튼 누름 -> 회전속도 슬라이더 고정
void PageGame::on_wspeed_btn_clicked()
{
    wspeedStopped = true;
}

// 던지기 버튼 누름 -> 두 슬라이더가 멈춰 있을 때만 발사!
void PageGame::on_GameThrow_btn_clicked()
{
    if (timer->isActive()) return; // 이미 비행 중이면 중복 실행 방지

    if (highStopped && wspeedStopped)
    {
        sliderTimer->stop();

        double hVal = ui->high_slider->value();
        double wVal = ui->wspeed_slider->value();

        // 현재 생성된 랜덤 풍속(currentWindSpeed)을 물리 엔진으로 전달
        physics.launch(hVal, wVal, currentWindSpeed);
        timer->start(35); // 물리 비행 시작
    }
}

// 프레임 단위 비행 연산 및 판정
void PageGame::animateBottle()
{
    physics.updateNextFrame();
    double landY = startY + 8.0;

    // 착지 지점에 도착했을 때 판정
    if (physics.isLanding(landY))
    {
        physics.setEvaluated(true); // 이번 던지기 시도 판정 완료

        // 멤버 함수(physics.currentAngle()) 사용
        if (physics.checkSuccess(physics.currentAngle()))
        {
            timer->stop();
            score++;
            ui->GameScore->display(score); // QLCDNumber 점수 반영

            // 위치 이동
            ui->label->move(static_cast<int>(physics.currentX()), static_cast<int>(landY));

            QString imagePath = QDir::homePath() + "/keyboard_project/images/bottle.png";
            QPixmap srcPixmap(imagePath);
            if (!srcPixmap.isNull()) ui->label->setPixmap(srcPixmap);

            // 성공 시 암전 연출 실행
            triggerSuccessEffect();

            // 1.5초 대기 후 원위치 및 다음 판 바람 새로 설정
            QTimer::singleShot(1500, this, [this]() {
                resetBottleToStart();
                highStopped = false;
                wspeedStopped = false;
                generateNewWind(); // 다음 판을 위해 바람 새로 생기기
                sliderTimer->start(10);
            });
            return;
        }
    }

    // 실패해서 바닥 밑으로 빠지는 경우 리셋
    if (physics.isOutOfBounds(600.0))
    {
        timer->stop();
        resetBottleToStart();
        highStopped = false;
        wspeedStopped = false;
        generateNewWind(); // 실패 후 재시도 시에도 바람 새로 생기기
        sliderTimer->start(10);
        return;
    }

    // 공중 회전 화면 렌더링
    renderBottleSprite();
}

// 물병 회전 이미지 출력 함수
void PageGame::renderBottleSprite()
{
    //physics.currentX(), physics.currentY() 형태로 호출
    ui->label->move(static_cast<int>(physics.currentX()), static_cast<int>(physics.currentY()));

    QString imagePath = QDir::homePath() + "/keyboard_project/images/bottle.png";
    QPixmap srcPixmap(imagePath);
    if (!srcPixmap.isNull())
    {
        QTransform transform;
        //physics.currentAngle() 형태로 호출
        transform.rotate(physics.currentAngle());
        QPixmap rotatedPixmap = srcPixmap.transformed(transform, Qt::SmoothTransformation);

        ui->label->setAlignment(Qt::AlignCenter);
        ui->label->setPixmap(rotatedPixmap);
    }
}

// 성공 시 서서히 어두워지는 암전 애니메이션 연출
void PageGame::triggerSuccessEffect()
{
    if (fadeOverlay == nullptr) return;

    fadeOverlay->raise();
    QGraphicsOpacityEffect *effect = qobject_cast<QGraphicsOpacityEffect*>(fadeOverlay->graphicsEffect());
    if (effect != nullptr)
    {
        QPropertyAnimation *animation = new QPropertyAnimation(effect, "opacity", this);
        animation->setDuration(1500); // 총 1.5초
        animation->setKeyValueAt(0.0, 0.0);  // 시작: 투명
        animation->setKeyValueAt(0.5, 0.85); // 0.75초: 어두움
        animation->setKeyValueAt(1.0, 0.0);  // 1.5초: 투명 복귀
        animation->setEasingCurve(QEasingCurve::InOutQuad);
        animation->start(QAbstractAnimation::DeleteWhenStopped);
    }
}

// 물병 원래 자리로 복귀
void PageGame::resetBottleToStart()
{
    physics.reset(startX, startY);

    QString path = QDir::homePath() + "/keyboard_project/images/bottle.png";
    QPixmap pix(path);
    if (!pix.isNull()) ui->label->setPixmap(pix);

    ui->label->move(static_cast<int>(startX), static_cast<int>(startY));
}
