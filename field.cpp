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

    int row  = 0, column = 0;
     for(int i = 0; i < size; ++i) {
         for(int j = 0; j< size; ++j) {
             if(&m_container[i][j] == sender()) {
                 row = i; column = j;
                 break;
             }
         }
     }



    if(!m_container[row][column].isBomb()) {
        int value = calcNearBomb(row, column);
        m_container[row][column].setNearBomb(value);

        if(value == 0)
            recursiveWalk(row,column);

    } else {

    }
}

int Field::calcNearBomb(int row, int column)
{
   if(row <= size - 1 && row >= 0 && column >=0 && column <= size - 1)
       return 0;
   int nearBomb = 0;
   nearBomb += isValid(row+1, column);
   nearBomb += isValid(row+1, column+1);
   nearBomb += isValid(row+1, column-1);
   nearBomb += isValid(row, column +1);
   nearBomb += isValid(row, column -1);
   nearBomb += isValid(row-1, column+1);
   nearBomb += isValid(row-1, column);
   nearBomb += isValid(row-1, column-1);
   return nearBomb;
}

bool Field::isValid(int row, int column)
{
    if(row <= size - 1 && row >= 0 && column <= size - 1 && column >=0)
        return m_container[row][column].isBomb();
    else return false;
}

void Field::recursiveWalk(int row, int column)
{
      if(moves.contains(QPair<int,int>(row,column)))
          return ;
      moves << QPair<int,int>(row, column);
      int result = calcNearBomb(row,column);
      if( result != 0) { // base case
          return;
      } else m_container[row][column].setNearBomb(result);


      recursiveWalk(row+1, column);
      recursiveWalk(row-1, column);
      recursiveWalk(row, column+1);
      recursiveWalk(row, column-1);
}

