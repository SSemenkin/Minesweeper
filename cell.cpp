#include "cell.h"

Cell::Cell(QWidget *parent) : QAbstractButton(parent), bomb(false), hovered(false), m_hidden(true),
    onClicked(false), onFlag(false)
{
    setSizePolicy(this->sizePolicy().horizontalPolicy(), QSizePolicy::Expanding);
}

void Cell::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::MouseButton::LeftButton ) {
        onClicked = true;
    }
}

void Cell::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::RenderHint::Antialiasing);

    p.setBrush(QColor("#A9A9A9"));
    p.drawRoundedRect(this->rect(), 5, 5);

    if(onFlag && m_hidden) {
        p.drawPixmap(this->rect(), QPixmap(":/icons/flag.png"));
    } else {
        hovered  ?  p.setBrush(QColor("#C0C0C0")) : p.setBrush(QColor("#A9A9A9"));
        p.drawRoundedRect(this->rect(), 5, 5);

        if(!m_hidden) p.setBrush(QColor(220,220,220));
        p.drawRoundedRect(this->rect(), 5, 5);

        if(onClicked)  {
            p.setPen(Qt::blue);
            p.drawRoundedRect(this->rect(), 5, 5);
        }

        if(bomb && !m_hidden) {
            p.drawPixmap(QRect(5,5,this->width()-10,this->height()-10), QPixmap(":/icons/bomb.png"));
            return;
        }

        if (nearBomb > 0 && !m_hidden) {
            switch (nearBomb) {
            case 1:
                p.setPen (Qt::blue);
                break;
            case 2:
                 p.setPen (Qt::green);
                break;
            case 3:
                 p.setPen (Qt::yellow);
                break;
            case 4:
                p.setPen (Qt::darkYellow);
                break;
            case 5:
                p.setPen (Qt::red);
                break;
            case 6:
                p.setPen (Qt::darkRed);
                break;
            case 7:
                p.setPen (Qt::cyan);
                break;
            case 8:
                p.setPen (Qt::darkCyan);
                break;
            case 9:
                p.setPen (Qt::magenta);
                break;

            }
            p.drawText(this->rect(), Qt::AlignCenter, QString::number(nearBomb));
        }
    }
}

void Cell::enterEvent(QEvent *)
{
    hovered = true;
    update();
}

void Cell::leaveEvent(QEvent *)
{
    hovered = false;
    update();
}

void Cell::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->button() == Qt::MouseButton::LeftButton ) {
        if(e->pos().rx() >= 0  && e->pos().rx() < this->width()
                && e->pos().ry() >= 0 && e->pos().ry() < this->height() && !onFlag) {
            m_hidden = false;
            onClicked = false;
            update();
            emit clicked();
        }
        else {
            onClicked = false;
        }
    } else if(e->button() == Qt::MouseButton::RightButton) {
        if(e->pos().rx() >= 0  && e->pos().rx() < this->width()
                && e->pos().ry() >= 0 && e->pos().ry() < this->height()) {
            onFlag = !onFlag;
        }
    }
}

void Cell::mouseMoveEvent(QMouseEvent *e)
{
    if((e->pos().rx() >= 0  && e->pos().rx() < this->width() &&
                                                      e->pos().ry() >= 0 && e->pos().ry() < this->height())) {
            hovered = true;
            onClicked = true;

        } else {
            onClicked = false;
            hovered = false;
        }

        update();
}
