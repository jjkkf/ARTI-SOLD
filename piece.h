#ifndef PIECE_H
#define PIECE_H

#include <QPoint>
#include <QString>

class Piece
{
public:
    enum pieceType{SOLD, ARTI};
    pieceType _pieceType; // 0=pao,1=bing
    int _r;
    QString _name;
    /* 当前坐标 */
    int _crow;
    int _ccol;
    //bool Move(int drow, int dcol);
};

#endif // PIECE_H
