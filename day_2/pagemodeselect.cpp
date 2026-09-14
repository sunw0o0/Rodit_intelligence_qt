#include "pagemodeselect.h"
#include <QPixmap>
#include <QDir>
#include <QTransform>

PageModeSelect::PageModeSelect(Ui::MainWindow *uiOwner, QObject *parent)
    : QObject(parent), ui(uiOwner)
{
    startX = ui->label->x();
    startY = ui->label->y();

    physics.reset(startX, startY);
    physics.launch(45.0, 25.0);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &PageModeSelect::animateBottle);
}

void PageModeSelect::startAnimation()
{
    physics.reset(startX, startY);
    physics.launch(45.0, 25.0);
    timer->start(35);
}

void PageModeSelect::stopAnimation()
{
    if (timer->isActive()) timer->stop();
}

void PageModeSelect::animateBottle()
{
    physics.updateNextFrame();

    if (physics.isOutOfBounds(550.0))
    {
        physics.reset(startX, startY);
        physics.launch(45.0, 25.0);
    }

    ui->label->move(static_cast<int>(physics.currentX()), static_cast<int>(physics.currentY()));

    QString imagePath = QDir::homePath() + "/keyboard_project/images/bottle.png";
    QPixmap srcPixmap(imagePath);
    if (!srcPixmap.isNull())
    {
        QTransform transform;
        transform.rotate(physics.currentAngle());

        QPixmap rotatedPixmap = srcPixmap.transformed(transform, Qt::SmoothTransformation);

        ui->label->setAlignment(Qt::AlignCenter);
        ui->label->setPixmap(rotatedPixmap);
    }
}
