#ifndef STUDENTAI_H
#define STUDENTAI_H
#include "AI.h"
#include "Board.h"
#pragma once

//The following part should be completed by students.
//Students can modify anything except the class name and exisiting functions and varibles.
class StudentAI :public AI
{
public:
    Board board;
    StudentAI(int col, int row, int p);
    virtual Move GetMove(Move board);
    int evaluation(Move mymove);
    pair<int , Move> maxValue(int depth);
    pair<int , Move> minValue(int depth);
    Move MiniMax();

};

#endif //STUDENTAI_H
