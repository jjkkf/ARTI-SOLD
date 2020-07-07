#ifndef STEP_H
#define STEP_H
#include "piece.h"

class Step
{
public:
    Step();
    Piece* _pmove;
    int _srow;
    int _scol;
    int _drow;
    int _dcol;
    Piece* _pkill;
};

#endif // STEP_H
