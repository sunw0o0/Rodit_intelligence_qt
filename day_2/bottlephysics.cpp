#include "bottlephysics.h"
#include <cmath>

BottlePhysics::BottlePhysics()
{
    m_startX = 0.0;
    m_startY = 0.0;
    m_currentX = 0.0;
    m_currentY = 0.0;
    m_speedX = 0.0;
    m_speedY = 0.0;
    m_currentAngle = 0.0;
    m_angularSpeed = 0.0;
    m_time = 0.0;
    m_gravity = 1.8;
    m_evaluated = false;

    m_windSpeed = 0.0;
    m_windAccX = 0.0;
}

// 물병의 위치와 상태 초기화
void BottlePhysics::reset(double x, double y)
{
    m_startX = x;
    m_startY = y;
    m_currentX = x;
    m_currentY = y;
    m_currentAngle = 0.0;
    m_time = 0.0;
    m_evaluated = false;

    m_windSpeed = 0.0;
    m_windAccX = 0.0;
}

// 높이, 회전속도, 풍속 값을 전달받아 초기 발사 수치 계산
void BottlePhysics::launch(double hVal, double wVal, double wind)
{
    // 1. 초기 비행 속도 계산 (높이 슬라이더 수치 반영)
    m_speedY = -15.0 - (hVal * 0.2); // 위쪽 방향 초기 속도
    m_speedX = -5.0;                 // 왼쪽 방향 기본 속도
    m_angularSpeed = 10.0 + (wVal * 0.2); // 회전 각속도

    // 2. 풍압 -> 힘 -> 가속도 물리 변환 로직
    m_windSpeed = wind;

    // 물리 상수 설정
    double airDensity = 1.225; // 공기 밀도 rho (kg/m^3)
    double area = 0.03;       // 물병 단면적 A (m^2)
    double dragCoeff = 0.8;    // 저항 계수 Cd
    double mass = 0.5;         // 물병 질량 m (kg)

    // 동압/풍압 연산: P = 0.5 * rho * v^2
    double pressure = 0.5 * airDensity * (m_windSpeed * m_windSpeed);

    // 풍압력(F) 연산: F = P * A * Cd
    double force = pressure * area * dragCoeff;

    // 바람 방향 설정 (음수: 왼쪽, 양수: 오른쪽)
    double windDirection = (m_windSpeed >= 0.0) ? 1.0 : -1.0;
    double signedForce = force * windDirection;

    // 가속도 연산: a = F / m
    // (게임 프레임 연산에 맞춰 조정계수 0.1을 곱해 화면 밖으로 급격히 튀는 것 방지)
    double realAccX = signedForce / mass;
    m_windAccX = realAccX * 0.1;

    // 바람이 세면 회전 각속도에도 약간의 영향
    m_angularSpeed += (m_windSpeed * 0.3);

    m_time = 0.0;
    m_evaluated = false;
}

// 후의 물병 위치 연산
void BottlePhysics::updateNextFrame()
{
    m_time += 1.0;

    // X축 위치: 등속 운동 + 바람 가속도 운동 (x = x0 + v*t + 1/2*a*t^2)
    double windDistanceX = 0.5 * m_windAccX * m_time * m_time;
    m_currentX = m_startX + (m_speedX * m_time) + windDistanceX;

    // Y축 위치: 중력 가속도 포물선 운동
    double gravityDistance = 0.5 * m_gravity * m_time * m_time;
    m_currentY = m_startY + (m_speedY * m_time) + gravityDistance;

    // 회전각 연산
    m_currentAngle = m_angularSpeed * m_time;
}

// 바닥선 통과 시점 판정
bool BottlePhysics::isLanding(double landY) const
{
    double currentVelocityY = m_speedY + (m_gravity * m_time);
    bool isFalling = (currentVelocityY > 0);
    bool isPastFloor = (m_currentY >= landY);

    return (!m_evaluated && isPastFloor && isFalling);
}

// 화면 아래로 탈락했는지 판정
bool BottlePhysics::isOutOfBounds(double limitY) const
{
    return (m_currentY > limitY);
}

// 똑바로 서서 착지했는지 성공 조건 판정
bool BottlePhysics::checkSuccess(double angle) const
{
    int finalAngle = static_cast<int>(angle) % 360;
    if (finalAngle < 0) finalAngle += 360;

    bool isVertical = (finalAngle <= 15 || finalAngle >= 345);
    bool hasRotatedEnough = (angle >= 300.0);

    return (isVertical && hasRotatedEnough);
}
