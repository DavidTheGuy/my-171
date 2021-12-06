#include "StudentAI.h"
#include <random>
#include <cmath>
#include <math.h>

//The following part should be completed by students.
//The students can modify anything except the class name and exisiting functions and varibles.

StudentAI::StudentAI(int col,int row,int p)
	:AI(col, row, p)
{
    list<Node> MCTSinfo;
    Node* current_node = nullptr;
    Node* actualStateNode = nullptr;
    beginGameTime = chrono::steady_clock::now();
    board = Board(col,row,p);
    board.initializeGame();
    player = 2;
}
Move StudentAI::MCTSsearch() {
    chrono::time_point<chrono::steady_clock> beginLoop = chrono::steady_clock::now();
    double howLong = chrono::duration<double>(beginLoop-beginFindMove).count();
    while(howLong < 10.0){
        select_and_expand();
        bool result = simulate();
        Back_Propagate(result);

        beginLoop = chrono::steady_clock::now();
        howLong = chrono::duration<double>(beginLoop-beginFindMove).count();//update time elapsed
    }
    return bestMove();
}
Move StudentAI::bestMove() {
    float score = -10.0;
    Node* myleaf;
    forward_list<Node*> toErase = actualStateNode->children;


    for (Node* kid: actualStateNode->children) {
        float childScore = kid->u_and_n.first/kid->u_and_n.second;
        if (childScore > score) {
            score = childScore;
            myleaf = kid;
        }
    }

    toErase.remove(myleaf); //remove chosen child from toErase

    //remove all children that weren't chosen from children of actual state node, and from Nodes that exist
    for(Node* child: toErase) {
        destroySubTree(child);//destroy subtree of child
        actualStateNode->children.remove(child);//remove pointer to child from actual state Node
    }
    current_node = myleaf;
    actualStateNode = current_node;

    return current_node->theMove;
}

bool StudentAI::destroySubTree(Node* root) {
    if (root->children.empty()){
        for (auto i = MCTSinfo.begin(); i != MCTSinfo.end(); i++) {
            if (root == &*i){
                MCTSinfo.erase(i);
                return true;
            }
        }
    }
    else{
        for(Node* kid: root->children) {
            if (destroySubTree(kid)){
                root->children.remove(kid);
            }
        }
        for (auto i = MCTSinfo.begin(); i != MCTSinfo.end(); i++) {
            if (root == &*i){
                MCTSinfo.erase(i);
                return true;
            }
        }
    }
    return false;
}

void StudentAI::select_and_expand() {
    //select
    while (current_node->children.empty() == true?false:true){
        float UCTscore = -10.0;
        float childScore = -10.0;
        Node* leaf;
        for (Node* kid: current_node->children){
            childScore = UCT(kid);
            if (childScore > UCTscore) {
                UCTscore = childScore;
                leaf = kid;
            }
        }
        current_node = leaf;
        board.makeMove(current_node->theMove, current_node->turn);
    }

    //expand
    float UCTscore = -10.0;
    int turn = current_node->turn == 1?2:1;
    vector<vector<Move>> moves = board.getAllPossibleMoves(turn);
    Node *chosenOne = current_node;
    if(!moves.empty()) {
        for (int myrow = 0; myrow < moves.size(); myrow++) {
            for (int mycol = 0; mycol < moves[myrow].size(); mycol++) {
                Move mymove = moves[myrow][mycol];
                Node child = Node();
                child.u_and_n = {0, 0};
                child.theMove = mymove;
                child.parent = current_node;
                child.turn = turn;
                MCTSinfo.push_front(child);
                current_node->children.push_front(&MCTSinfo.front());
                float childScore = UCT(&child);
                if (childScore > UCTscore) {
                    UCTscore = childScore;
                    chosenOne = &MCTSinfo.front();
                }
            }
        }
        current_node = chosenOne;
        board.makeMove(current_node->theMove, current_node->turn);
    }
    else{
        current_node = chosenOne;
    }
}

bool StudentAI::simulate() {
    int moveCount = 0;//need this to know how many moves have been made
    bool win = false;
    int turn = current_node->turn == 1?2:1;
    while (board.isWin(turn==1?2:1) == 0) {
        vector<vector<Move> > moves = board.getAllPossibleMoves(turn);
        int i = rand() % (moves.size());
        vector<Move> checker_moves = moves[i];
        int j = rand() % (checker_moves.size());
        Move res = checker_moves[j];
        board.makeMove(res,turn);//update board
        moveCount++;
        turn = turn==1?2:1;//switch whose turn it is
    }
    if (board.isWin(turn==1?2:1) == player){
        win = true;
    }
    for (int k = 0; k < moveCount; k++){
        //use moveCount to undo the move made by that many number of times
        //makes it so that the last move made correlates with currentNode
        board.Undo();
    }
    return win;
}
void StudentAI::Back_Propagate(bool won){
    backTraceNode = current_node;
    if(won){
        while(backTraceNode != nullptr){
            backTraceNode->u_and_n.first += 1;
            backTraceNode->u_and_n.second += 1;
            backTraceNode = backTraceNode->parent;
        }
    }
    else{
        while(backTraceNode != nullptr){
            backTraceNode->u_and_n.second += 1;
            backTraceNode = backTraceNode->parent;
        }
    }
    //set board back to original state of actual gameplay
    while (current_node != actualStateNode){
        board.Undo();
        current_node = current_node->parent;
    }
}

float StudentAI::UCT(Node* myNode) {
    float value = 0.0;
    if (myNode->u_and_n.second == 0){
        value += myNode->u_and_n.first / 1.0;
        if (myNode->parent->u_and_n.second > 1){
            value += 1.4 * sqrt((log(myNode->parent->u_and_n.second) / 1));
        }
        else {
            value += 1.4 * sqrt((log(1) / 1));
        }
    }
    else{
        value += myNode->u_and_n.first / myNode->u_and_n.second;
        value += 1.4 * sqrt((log(myNode->parent->u_and_n.second) / myNode->u_and_n.second));
    }
    return value;

}

bool StudentAI::makeCurrentIfExist(Move move){
    if (MCTSinfo.empty()){
        return false;
    }
    else {
        for (Node *kid: actualStateNode->children) {
            if (move.seq[0][0] == kid->theMove.seq[0][0] && move.seq[0][1] == kid->theMove.seq[0][1]) {
                if (move.seq[1][0] == kid->theMove.seq[1][0] && move.seq[1][1] == kid->theMove.seq[1][1]) {
                    actualStateNode = kid;
                    current_node = actualStateNode;
                    return true;

                }
            }
        }
        return false;
    }
}

Move StudentAI::GetMove(Move move)
{
    if (move.seq.empty())
    {
        player = 1;
        Node root = Node();
        root.u_and_n = {0,0};
        root.theMove = Move();
        root.parent = nullptr;
        root.turn = 2;
        MCTSinfo.push_back(root);
        current_node = &MCTSinfo.back();
        actualStateNode = &MCTSinfo.back();
    }else{
        board.makeMove(move,player == 1?2:1);
        if(!makeCurrentIfExist(move)){
            Node opponent = Node();
            opponent.u_and_n = {0,0};
            opponent.theMove = move;
            opponent.parent = current_node;
            opponent.turn = player == 1?2:1;
            MCTSinfo.push_front(opponent);
            actualStateNode = &MCTSinfo.front();
            current_node = &MCTSinfo.front();

        }
    }

   /* int i = rand() % (moves.size());
    vector<Move> checker_moves = moves[i];
    int j = rand() % (checker_moves.size());
    Move res = checker_moves[j];
    board.showBoard();
    board.makeMove(res,player);
    board.showBoard();
    return res;*/
    beginFindMove = chrono::steady_clock::now();
    Move finalMove = MCTSsearch();
    board.makeMove(finalMove, player);
    return finalMove;
}

