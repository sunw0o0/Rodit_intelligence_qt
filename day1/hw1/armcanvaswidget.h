#ifndef ARMCANVASWIDGET_H
#define ARMCANVASWIDGET_H

#include <QWidget>

class ArmCanvasWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ArmCanvasWidget(QWidget* parent = nullptr);

    void setLinks(double link1, double link2, double link3);
    void setJoints(int joint1, int joint2, int joint3);
    void setArmGenerated(bool generated);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    double m_link1 = 0;
    double m_link2 = 0;
    double m_link3 = 0;

    int m_joint1 = 0;
    int m_joint2 = 0;
    int m_joint3 = 0;

    bool m_armGenerated = false;

    const double m_originX = 200;
    const double m_originY = 350;
    const double m_scale = 50; // 1 단위 = 50픽셀
};

#endif // ARMCANVASWIDGET_H
