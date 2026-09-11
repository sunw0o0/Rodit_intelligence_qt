#include "armcanvaswidget.h"
#include <QPainter>
#include <QtMath>

ArmCanvasWidget::ArmCanvasWidget(QWidget* parent) : QWidget(parent)
{
}

void ArmCanvasWidget::setLinks(double link1, double link2, double link3)
{
    m_link1 = link1;
    m_link2 = link2;
    m_link3 = link3;
}

void ArmCanvasWidget::setJoints(int joint1, int joint2, int joint3)
{
    m_joint1 = joint1;
    m_joint2 = joint2;
    m_joint3 = joint3;
    update();
}

void ArmCanvasWidget::setArmGenerated(bool generated)
{
    m_armGenerated = generated;
    update();
}

void ArmCanvasWidget::paintEvent(QPaintEvent* event)
{
    QWidget::paintEvent(event);

    if (!m_armGenerated)
        return;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    double t1 = qDegreesToRadians(static_cast<double>(m_joint1));
    double t2 = qDegreesToRadians(static_cast<double>(m_joint2));
    double t3 = qDegreesToRadians(static_cast<double>(m_joint3));

    double x0 = m_originX, y0 = m_originY;

    double x1 = x0 + m_link1 * m_scale * qCos(t1);
    double y1 = y0 - m_link1 * m_scale * qSin(t1);

    double x2 = x1 + m_link2 * m_scale * qCos(t1 + t2);
    double y2 = y1 - m_link2 * m_scale * qSin(t1 + t2);

    double x3 = x2 + m_link3 * m_scale * qCos(t1 + t2 + t3);
    double y3 = y2 - m_link3 * m_scale * qSin(t1 + t2 + t3);

    painter.setPen(QPen(Qt::cyan, 4));
    painter.drawLine(QPointF(x0, y0), QPointF(x1, y1));
    painter.drawLine(QPointF(x1, y1), QPointF(x2, y2));
    painter.drawLine(QPointF(x2, y2), QPointF(x3, y3));

    painter.setBrush(Qt::yellow);
    painter.setPen(Qt::NoPen);
    for (const QPointF& p : {QPointF(x0, y0), QPointF(x1, y1), QPointF(x2, y2), QPointF(x3, y3)})
        painter.drawEllipse(p, 6, 6);
}
