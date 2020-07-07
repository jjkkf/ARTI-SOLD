#include "board.h"
#include <QPainter>
#include <QPoint>
#include <QMouseEvent>
#include <QPushButton>

Board::Board(QWidget *parent)
    : QWidget(parent)
{
    init();
    Pick(SOLD);
    regretApp();
}
Board::~Board()
{
}

void Board::init()
{

    this->_selectP = NULL;
    this->_row = 6;
    this->_col = 6;
    this->_size = 50;
    this->_artiTurn = true;
    this->_maxPiece_r = _size / 3;
    resize(_size * (_row+2), _size * (_col+2));

    for(int i = 0; i < 20; i++)
    {
        Piece *p = new Piece;
        p->_pieceType = SOLD;
        p->_name = "兵";
        p->_crow = i / _row;
        p->_ccol = i % _row;
        p->_r = this->_size / 4;

        if(i == 18)
        {
            p->_pieceType = ARTI;
            p->_name = "炮";
            p->_crow = 4;
            p->_ccol = 2;
            p->_r = this->_size / 3;
        }
        if(i == 19)
        {
            p->_pieceType = ARTI;
            p->_name = "炮";
            p->_crow = 4;
            p->_ccol = 3;
            p->_r = this->_size / 3;
        }
        _vp.push_back(p);
    }
}

void Board::Pick(pieceType type)
{
    if(SOLD == type)
    {
        for(ITP it = _vp.begin(); it != _vp.end(); it++)
        {
            (*it)->_crow = _row - 1 - (*it)->_crow;
            (*it)->_ccol = _col - 1 - (*it)->_ccol;
        }
    }
}

void Board::regretApp()
{
    QPushButton *p = new QPushButton(this);
    p->setText("悔棋");
    p->resize(50, 50);
    p->move(_size * (_row+1), _size * (_col+2) / 2 - _size / 2);

    connect(p, &QPushButton::released, this, &Board::back);
}

void Board::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    QPen pen;
    p.setPen(pen);
    p.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    drawBoard(p, _row - 1, _col - 1);
    drawStone(p);
}

void Board::drawBoard(QPainter &p, int n, int m)
{
    for(int i = 0; i <= n; i++)
    {
        p.drawLine(pointToPos(i, 0), pointToPos(i, m));
    }
    for(int i = 0; i <= m; i++)
    {
        p.drawLine(pointToPos(0, i), pointToPos(n, i));
    }
}



QPoint Board::pointToPos(int row, int col)
{
    QPoint point;
    point.rx() = ((col + 1) * _size);
    point.ry() = ((row + 1) * _size);
    return point;
}


void Board::drawStone(QPainter& p)
{
    for(ITP it = _vp.begin(); it != _vp.end(); it++)
    {
        if(*it == _selectP)
        {
            p.setBrush(QBrush(QColor(0, 255, 255)));
        }
        else
        {
            p.setBrush(QBrush(QColor(255, 255, 0)));
        }

        if(SOLD == (*it)->_pieceType)
        {
            p.setPen(Qt::red);

        }
        else
        {
            p.setPen(Qt::black);
        }

        Piece *piece = *it;

        QRect rect = QRect(pointToPos(piece->_crow, piece->_ccol).x() - piece->_r,
                           pointToPos(piece->_crow, piece->_ccol).y() - piece->_r,
                           piece->_r * 2, piece->_r * 2);
        p.drawEllipse(pointToPos(piece->_crow, piece->_ccol), piece->_r, piece->_r);
        p.drawText(rect, piece->_name, QTextOption(Qt::AlignCenter));
    }
}


void Board::mouseReleaseEvent(QMouseEvent *event)
{
    click(event->pos());
    update();
}

void Board::click(QPoint p)
{
    if(outSide(p)) return;

    if(NULL == _selectP)
    {
        _selectP = trySlect(p);
    }
    else
    {
        if(tryMove(_selectP, p)) _selectP = NULL;
    }
}

bool Board::changePiece(int row, int col)
{
    Piece *piece = pointToPiece(row, col);
    if(NULL != piece && _selectP->_pieceType == piece->_pieceType)
    {
        _selectP = piece;
        return true;
    }
    return false;
}

bool Board::tryMove(Piece* &piece, QPoint p)
{
    int row;
    int col;
    posToPoint(p, row, col);
    if(!changePiece(row, col))
    {
        Move(piece, row, col);
        _artiTurn = !_artiTurn;
        return true;
    }
    return false;
}

void Board::Move(Piece* &piece, int row, int col)
{
    if(canMove(piece, row, col))
    {
        ITP it = pointToPieceIT(row, col);
        if(NULL != it)
        {
            saveOneStep(piece, row, col, *it, _vs);
            _vp.erase(it);
        } else {
            saveOneStep(piece, row, col, NULL, _vs);
        }

        // 移动
        piece->_crow = row;
        piece->_ccol = col;

        update();
        piece = NULL;
    }
}

bool Board::canMove(Piece* &piece, int drow, int dcol)
{
    if(SOLD == piece->_pieceType)
    {
        if(NULL != pointToPiece(drow, dcol)) return false;

        int ret = qAbs(piece->_crow-drow)*10 + qAbs(piece->_ccol-dcol);
        if(ret != 1 && ret != 10) return false;
        return true;
    }
    else
    {
        int ret = qAbs(piece->_crow-drow)*10 + qAbs(piece->_ccol-dcol);
        if(ret != 2 && ret != 20) return false;
        if(ret != 2 && ret != 20) return false;

        int midrow = piece->_crow + (drow - piece->_crow) / 2;
        int midcol = piece->_ccol + (dcol - piece->_ccol) / 2;
        if(NULL != pointToPiece(midrow, midcol)) return false;

        return true;
    }
}

Piece *Board::pointToPiece(int row, int col)
{
    for(ITP it = _vp.begin(); it != _vp.end(); it++)
    {
        if(row==(*it)->_crow && col==(*it)->_ccol)
        {
            return *it;
        }
    }
    return NULL;
}

ITP Board::pointToPieceIT(int row, int col)
{
    for(ITP it = _vp.begin(); it != _vp.end(); it++)
    {
        if(row==(*it)->_crow && col==(*it)->_ccol)
        {
            return it;
        }
    }
    return NULL;
}

Piece *Board::trySlect(QPoint p)
{
    Piece *piece = clickedPosPiece(p);
    if(NULL != piece)
    {
        if(_artiTurn != (int)piece->_pieceType) return NULL;
    }
    return piece;
}

Piece *Board::clickedPosPiece(QPoint p)
{

    int row;
    int col;
    posToPoint(p, row, col);
    int ret = posDistanceWithPoint(p, row, col);
    for(ITP it = _vp.begin(); it != _vp.end(); it++)
    {
        if(ret<(*it)->_r && row==(*it)->_crow && col==(*it)->_ccol)
        {
            return *it;
        }
    }
    return NULL;
}

void Board::posToPoint(QPoint p, int &row, int &col)
{
    int rowint = p.y() / _size;
    int rowrem = p.y() % _size;
    int colint = p.x() / _size;
    int colrem = p.x() % _size;

    if(rowrem > _size / 2) rowint++;
    if(colrem > _size / 2) colint++;
    row = rowint - 1;
    col = colint - 1;
}

int Board::posDistanceWithPoint(QPoint p, int row, int col)
{
    int dx = (p.x() > (col+1)*_size) ? (p.x() - (col+1)*_size) : ((col+1)*_size - p.x());
    int dy = (p.y() > (row+1)*_size) ? (p.y() - (row+1)*_size) : ((row+1)*_size - p.y());
    return sqrt(dx*dx + dy*dy);
}

bool Board::outSide(QPoint p)
{
    if(p.x()<(_size-_maxPiece_r) || p.x()>(_col*_size+_maxPiece_r) ||
       p.y()<(_size-_maxPiece_r) || p.y()>(_row*_size+_maxPiece_r))
        return true;

    return false;
}

void Board::saveOneStep(Piece* pmove, int drow, int dcol, Piece* pkill, QVector <Step*> &v)
{
    Step *s = new Step;
    s->_pmove = pmove;
    s->_srow = pmove->_crow;
    s->_scol = pmove->_ccol;
    s->_drow = drow;
    s->_dcol = dcol;
    s->_pkill = pkill;
    v.push_back(s);
}

void Board::backOneStep(QVector <Step*> &v)
{
    if(v.empty()) return;

    _artiTurn = !_artiTurn;

    QVector <Step*>::iterator it = v.end();
    Step *s = *(--it);
    s->_pmove->_crow = s->_srow;
    s->_pmove->_ccol = s->_scol;
    if(NULL != s->_pkill) _vp.push_back(s->_pkill);
    delete s;
    v.pop_back();
    update();
}

void Board::back()
{
    backOneStep(_vs);
}







