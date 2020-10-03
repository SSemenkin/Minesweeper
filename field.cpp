#include "Field.h"
#include "ui_Field.h"

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
    int row , column;
     for(int i = 0; i < size; ++i) {
         for(int j = 0; j< size; ++j) {
             if(&m_container[i][j] == sender()) {
                 row = i; column = j;
                 break;
             }
         }
     }

    if(!m_container[row][column].isBomb()) {
        m_container[row][column].setNearBomb(calcNearBomb(row, column));
    } else {

    }
}

int Field::calcNearBomb(int row, int column)
{
    int nearBomb = 0;
    if(row + 1 < size - 1) {
       if(column + 1 < size - 1) {
            if(m_container[row + 1][column + 1].isBomb()) nearBomb ++;
       }
       if(column - 1 >= 0) {
            if(m_container[row + 1][column - 1].isBomb()) nearBomb ++;
       }
       if(m_container[row+1][column].isBomb()) nearBomb ++;
    }
    if(row - 1 >= 0) {
        if(column + 1 < size - 1) {
           if(m_container[row - 1][column + 1].isBomb()) nearBomb ++;
        }
        if(column - 1 >= 0) {
           if(m_container[row - 1][column - 1].isBomb()) nearBomb ++;
        }
        if(m_container[row -1][column].isBomb()) nearBomb ++;
    }
    if(column + 1  < size - 1) {
        if(m_container[row][column + 1].isBomb()) nearBomb ++;
    }
    if(column - 1 >= 0) {
        if(m_container[row][column - 1].isBomb()) nearBomb ++;
    }


    return nearBomb;
}

