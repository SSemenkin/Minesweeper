#ifndef CELL_H
#define CELL_H

#include <QPaintEvent>
#include <QWidget>
#include <QDebug>
#include <QPainter>
#include <QPushButton>

class Cell : public QAbstractButton
{
    Q_OBJECT
public:
    explicit Cell(QWidget *parent = nullptr);
    void setBomb (bool state) { bomb = state; }
    bool isBomb () const { return bomb;}
    void setNearBomb (int nBomb) { nearBomb = nBomb; if(nearBomb) update();}
    void setHidden (bool hidden) { m_hidden = hidden; update();}

protected:
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
    void paintEvent(QPaintEvent *) override;
    void enterEvent(QEvent *) override;
    void leaveEvent(QEvent *) override;

private:

    bool bomb;
    bool hovered;
    bool m_hidden;
    int nearBomb = 0;
    bool onClicked;
    bool onFlag;
};

#endif // CELL_H
