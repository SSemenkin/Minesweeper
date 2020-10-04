#include "field.h"
#include "ui_field.h"

Field::Field(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Field)
{
    ui->setupUi(this);
    srand(time(NULL));
    ui->gridLayout->setSpacing(3);
    initiateGame();
}

Field::~Field()
{
    for ( int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            ui->gridLayout->removeWidget(&m_container[i][j]);
        }
    }

    for (int i = 0; i < size; i++)
        delete [] m_container[i];

    delete [] m_container;

    delete ui;
}

void Field::initiateGame()
{
    m_container = new Cell*[size];

    for (int i = 0; i < size; ++i)
        m_container[i] = new Cell[size];

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            ui->gridLayout->addWidget(&m_container[i][j], i, j);
            QObject::connect( &m_container[i][j], &Cell::clicked, this, &Field::handleMouseClick);
        }
    }

    generateBombs();

}

void Field::generateBombs()
{
    QList <int> indexes;

    for (int i = 0; i < bombCounter; i++) {
        int index = rand() % (size * size - 1);
        while (indexes.contains(index)) {
             index = rand() % (size * size - 1);
        }

        indexes << index;
        m_container[index/size][index%size].setBomb(true);
    }
}

void Field::handleMouseClick()
{
    int row  = 0, column = 0;
     for(int i = 0; i < size; ++i) {
         for(int j = 0; j< size; ++j) {
             if(&m_container[i][j] == sender()) {
                 row = i; column = j;
                 break;
             }
         }
     }

     qDebug() << row << column;
    if(!m_container[row][column].isBomb()) {
        int value = calcNearBomb(row, column);
        m_container[row][column].setNearBomb(value);

        if(value == 0) recursiveWalk (row, column);
        moves.clear ();

        if(isWin ()) {
            questionForRestart ("Victory", "You Win!\nDo you want to start new game?");
        }
    } else {
            questionForRestart ("Defeat", "You Lose!\nDo you want to start new game?");
    }
}

int Field::calcNearBomb(int row, int column)
{
   int nearBomb = 0;
   nearBomb += valueAtIndex(row+1, column);
   nearBomb += valueAtIndex(row+1, column+1);
   nearBomb += valueAtIndex(row+1, column-1);
   nearBomb += valueAtIndex(row, column +1);
   nearBomb += valueAtIndex(row, column -1);
   nearBomb += valueAtIndex(row-1, column+1);
   nearBomb += valueAtIndex(row-1, column);
   nearBomb += valueAtIndex(row-1, column-1);
   return nearBomb;
}

bool Field::valueAtIndex(int row, int column)
{
    return (row <= size - 1 && row >= 0 && column <= size - 1 && column >=0)  ?
         m_container[row][column].isBomb() :  false;
}

void Field::recursiveWalk(int row, int column)
{
    if(row > size - 1 || row < 0 || column < 0 || column > size - 1 || moves.contains (QPair<int, int> (row, column)))
        return;


    moves << QPair<int, int> (row, column);
    int result = calcNearBomb (row,column);
    if(m_container[row][column].isBomb ()) {
        return;
    } else if (!m_container[row][column].isBomb () && result !=0){
        m_container[row][column].setNearBomb (result);
        return;
    } else m_container[row][column].setNearBomb (result);

    recursiveWalk (row + 1, column);
    recursiveWalk (row - 1, column);
    recursiveWalk (row, column + 1);
    recursiveWalk (row, column - 1);
    recursiveWalk (row + 1, column + 1);
    recursiveWalk (row + 1, column - 1);
    recursiveWalk (row - 1, column + 1);
    recursiveWalk (row - 1, column - 1);
}

void Field::restartGame()
{
    moves.clear ();

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            ui->gridLayout->removeWidget (&m_container[i][j]);
        }
    }

    for (int i = 0; i < size; i ++)
        delete [] m_container[i];

    delete [] m_container;

    initiateGame ();
}

bool Field::isWin()
{
    int cellsThatNeedToShow = size * size - bombCounter;
    int counterShows = 0;
    for (int i = 0; i < size; i ++) {
        for (int j = 0; j < size; j++) {
            if(!m_container[i][j].isHidden ()) counterShows ++;
        }
    }
    return counterShows - cellsThatNeedToShow == 0 ? true : false;
}

void Field::questionForRestart(const QString title, const QString text)
{
    int result = QMessageBox::question (this, title, text);

    switch (result) {
    case QMessageBox::StandardButton::Yes:
        restartGame ();
        break;
    case QMessageBox::StandardButton::No:
        QCoreApplication::quit ();
        break;
    }
}

