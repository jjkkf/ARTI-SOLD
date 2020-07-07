#ifndef SINGLEGAME_H
#define SINGLEGAME_H

#include "board.h"
#include <QPushButton>

class SingleGame : public Board
{
    Q_OBJECT
public:
    SingleGame();
    void click(QPoint p);

    void computerMove();
    Step* getBestStep();
    void getAllStep(pieceType type, QVector <Step*> &v);
    int cacMaxScore(int level, int curMinScore);
    int cacMinScore(int level, int curMaxScore);
    void fakeMove(Step* &s);
    void unfakeMove(Step* &s);
    int cacScore();
    int getArtiFreeCnt();
    int getSoldCnt();

    int _level;
    QPushButton *_p;
    QVector <Step*> _vAlls;

public slots:
    void action();
};

#endif // SINGLEGAME_H
