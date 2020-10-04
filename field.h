#ifndef Field_H
#define Field_H

#include <QMainWindow>
#include <QDebug>
#include <QDebug>
#include <QMessageBox>

#include "cell.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Field; }
QT_END_NAMESPACE

class Field : public QMainWindow
{
    Q_OBJECT

public:
    Field(QWidget *parent = nullptr);
    ~Field();

private Q_SLOTS:

    void initiateGame     ();
    void generateBombs    ();
    void handleMouseClick ();
    int calcNearBomb (int row, int column);
    bool valueAtIndex(int row, int column);
    void recursiveWalk (int row, int column);
    void restartGame();
    bool isWin();
    void questionForRestart(const QString title, const QString text);
private:
    Ui::Field *ui;

    int size = 10;
    int bombCounter = 10;
    Cell ** m_container;

    QVector < QPair<int, int>> moves;
};
#endif // Field_H
