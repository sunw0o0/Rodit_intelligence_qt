#ifndef BOTTLEPHYSICS_H
#define BOTTLEPHYSICS_H

// 물병의 포물선 및 풍압 물리를 담당하는 클래스
class BottlePhysics
{
public:
    BottlePhysics();

    // 물병 상태 초기화 (위치 X, Y)
    void reset(double x, double y);

    // 슬라이더 수치 및 풍속(wind)을 전달받아 던지기 시작
    void launch(double hVal, double wVal, double wind = 0.0);

    // 1프레임(35ms) 마다 물리 위치 갱신 연산
    void updateNextFrame();

    // 바닥 착지 및 화면 이탈 판정 함수
    bool isLanding(double landY) const;
    bool isOutOfBounds(double limitY) const;
    bool checkSuccess(double angle) const;

    // 키워드를 떼어낸 깔끔한 수치 조회 함수
    double currentX() const { return m_currentX; }
    double currentY() const { return m_currentY; }
    double currentAngle() const { return m_currentAngle; }
    bool isEvaluated() const { return m_evaluated; }
    void setEvaluated(bool state) { m_evaluated = state; }

    //맴버변수와 매개변수의 구분을 분명히 하고자 변수명 앞에 m을 붙임
private:
    // 시작 위치 좌표
    double m_startX;
    double m_startY;

    // 현재 위치 좌표
    double m_currentX;
    double m_currentY;

    // 이동 속도 및 회전 변수
    double m_speedX;
    double m_speedY;
    double m_currentAngle;
    double m_angularSpeed;

    // 기본 물리 파라미터
    double m_time;
    double m_gravity;
    bool m_evaluated;

    // 바람 및 풍압 관련 물리 변수
    double m_windSpeed; // 풍속 (m/s)
    double m_windAccX;  // 바람에 의해 발생한 X축 가속도 (m/s^2)
};

#endif // BOTTLEPHYSICS_H
