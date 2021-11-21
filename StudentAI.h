#ifndef STUDENTAI_H
#define STUDENTAI_H
#include "AI.h"
#include "Board.h"
#include <chrono>
#include <list>
#include <forward_list>

#pragma once

//The following part should be completed by students.
//Students can modify anything except the class name and exisiting functions and varibles.
class StudentAI :public AI
{
public:
    struct Node{
        int turn;
        pair<float,float> u_and_n;
        Move theMove;
        Node* parent;
        forward_list<Node*> children;
    };
    Board board;
    Node* current_node;
    Node* actualStateNode;
    Node* backTraceNode;
    int gameDepth;
    list<Node> MCTSinfo;
    chrono::time_point<chrono::steady_clock> beginFindMove;
    chrono::time_point<chrono::steady_clock> beginGameTime;
    StudentAI(int col, int row, int p);
	virtual Move GetMove(Move board);

    float UCT(Node myNode);
    void Back_Propagate(bool won);
    bool simulate();
    Move MCTSsearch();
    void select_and_expand();
    bool makeCurrentIfExist(Move move);
    Move bestMove();

};

#endif //STUDENTAI_H
