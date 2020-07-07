#ifndef BOARD_H
#define BOARD_H

#include "step.h"
#include "piece.h"
#include <QWidget>

typedef QVector <Piece*>::iterator  ITP;

class Board : public QWidget , public Piece
{
    Q_OBJECT
public:
    Board(QWidget *parent = 0);
    ~Board();

    void init();
    void Pick(pieceType type);
    void regretApp();

    /* 绘图函数 */
    void paintEvent(QPaintEvent *event);
    void drawBoard(QPainter &p, int n, int m);
    void drawStone(QPainter &p);

    /* 移动函数 */
    void mouseReleaseEvent(QMouseEvent *event);
    virtual void click(QPoint p);
    bool tryMove(Piece* &piece, QPoint p);
    void Move(Piece* &piece,int row, int col);
    bool changePiece(int row, int col);
    Piece *trySlect(QPoint p);
    bool canMove(Piece* &piece, int drow, int dcol);

    /* 工具函数 */
    /* pos:控件坐标 point:棋盘坐标 */
    QPoint pointToPos(int row, int col);
    void posToPoint(QPoint p, int &row, int &col);
    bool outSide(QPoint p);
    Piece *clickedPosPiece(QPoint p);
    Piece *pointToPiece(int row, int col);
    ITP pointToPieceIT(int row, int col);
    int posDistanceWithPoint(QPoint p, int row, int col);

    /* 悔棋函数 */
    void saveOneStep(Piece* pmove, int drow, int dcol, Piece* pkill, QVector <Step*> &v);
    void backOneStep(QVector <Step*> &vs);

protected:
    int _row;
    int _col;
    int _size;
    int _maxPiece_r; // 用于防止越界
    bool _artiTurn;

    Piece* _selectP;
    QVector <Piece*> _vp; // 用于存放棋子
    QVector <Step*> _vs; // 用于存放动作

public slots:
    void back();
};

#endif // BOARD_H
