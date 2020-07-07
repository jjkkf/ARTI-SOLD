#include "singlegame.h"
#include "step.h"
#include <QVector>

SingleGame::SingleGame()
{
    this->_level = 3;

    _p = new QPushButton(this);
    _p->setText("开始");
    _p->resize(50, 50);
    _p->move(_size * (_row+1), _size * (_col+2) / 2 - 2 * _size);
    connect(_p, &QPushButton::released, this, &SingleGame::action);
}
void SingleGame::action()
{

    click(QPoint(2,3));
}

void SingleGame::click(QPoint p)
{
    static int a = 0;
    if(a)
    {
        if(_artiTurn) return;
    }
    a = 1;


    if(!_artiTurn)
    {
        Board::click(p);
    }

    if(_artiTurn)
    {
        computerMove();
        _artiTurn = !_artiTurn;
    }

}

void SingleGame::computerMove()
{
    Step *s = getBestStep();
    Move(s->_pmove, s->_drow, s->_dcol);

    for(QVector <Step*>::iterator it = _vAlls.begin(); it != _vAlls.end(); it++)
    {
        delete *it;
    }
    _vAlls.clear();
}


Step* SingleGame::getBestStep()
{
    int maxScore = -100000;
    int score;
    Step *step;

    getAllStep(ARTI, _vAlls);

    for(QVector <Step*>::iterator it = _vAlls.begin(); it != _vAlls.end(); it++)
    {
        fakeMove(*it);
        score= cacMinScore(_level, maxScore);
        unfakeMove(*it);

        if(score > maxScore)
        {
            maxScore = score;
            step = *it;
        }
    }

    return step;
}


int SingleGame::cacMaxScore(int level, int curMinScore)
{
    if(0 == level) return cacScore();

    int maxScore = -100000;
    int score;
    QVector <Step*> v;

    getAllStep(ARTI, v);

    for(QVector <Step*>::iterator it = v.begin(); it != v.end(); it++)
    {
        fakeMove(*it);
        score= cacMinScore(level - 1, maxScore);
        unfakeMove(*it);

        if(score > curMinScore) goto Cut;

        if(score > maxScore)
        {
            maxScore = score;
        }
    }

    for(QVector <Step*>::iterator it = v.begin(); it != v.end(); it++)
    {
        delete *it;
    }
    v.clear();

    return maxScore;

Cut:
    for(QVector <Step*>::iterator it = v.begin(); it != v.end(); it++)
    {
        delete *it;
    }
    v.clear();

    return score;
}

int SingleGame::cacMinScore(int level, int curMaxScore)
{
    if(0 == level) return cacScore();

    int minScore = 100000;
    int score;
    QVector <Step*> v;

    getAllStep(SOLD, v);

    for(QVector <Step*>::iterator it = v.begin(); it != v.end(); it++)
    {
        fakeMove(*it);
        score= cacMaxScore(level - 1, minScore);
        unfakeMove(*it);

        if(score < curMaxScore) goto Cut;

        if(score < minScore)
        {
            minScore = score;
        }
    }

    for(QVector <Step*>::iterator it = v.begin(); it != v.end(); it++)
    {
        delete *it;
    }
    v.clear();

    return minScore;

Cut:
    for(QVector <Step*>::iterator it = v.begin(); it != v.end(); it++)
    {
        delete *it;
    }
    v.clear();

    return score;
}

/*
 * 记分分两种情况：1按剩余小兵数量，2按炮的自由度
 * 1，一个小兵-4分。
 * 2，最多四个自由度，一个自由度记1分。
*/
int SingleGame::cacScore()
{
    return getArtiFreeCnt() - 4 * getSoldCnt();
}

int SingleGame::getArtiFreeCnt()
{
    int freeCnt = 0;
    for(ITP it = _vp.begin(); it != _vp.end(); it++)
    {
        // 电脑用炮temp
        if(ARTI != (*it)->_pieceType) continue;

        for(int row = 0; row < _row; row++)
        {
            for(int col = 0; col < _col; col++)
            {
                if(canMove(*it, row, col))
                {
                    freeCnt++;
                }
            }
        }
    }

    return freeCnt;
}

int SingleGame::getSoldCnt()
{
    int soldCnt = 0;
    for(ITP it = _vp.begin(); it != _vp.end(); it++)
    {
        if(SOLD == (*it)->_pieceType)
        {
            soldCnt++;
        }
    }
    return soldCnt;
}

void SingleGame::fakeMove(Step* &s)
{
    if(NULL != s->_pkill)
    {
        for(ITP it = _vp.begin(); it != _vp.end(); it++)
        {
            if(*it == s->_pkill)
            {
                _vp.erase(it);
                break;
            }
        }
    }

    // 移动
    s->_pmove->_crow = s->_drow;
    s->_pmove->_ccol = s->_dcol;
}

void SingleGame::unfakeMove(Step* &s)
{
    s->_pmove->_crow = s->_srow;
    s->_pmove->_ccol = s->_scol;

    if(NULL != s->_pkill)
    _vp.push_back(s->_pkill);
}

void SingleGame::getAllStep(pieceType type, QVector <Step*> &v)
{
    for(ITP it = _vp.begin(); it != _vp.end(); it++)
    {
        // 电脑用炮temp
        if(type != (*it)->_pieceType) continue;

        for(int row = 0; row < _row; row++)
        {
            for(int col = 0; col < _col; col++)
            {
                if(canMove(*it, row, col))
                {
                    Piece *p = pointToPiece(row, col);
                    saveOneStep(*it, row, col, p, v);
                }
            }
        }
    }
}









