#include "StudentAI.h"
#include <random>

//The following part should be completed by students.
//The students can modify anything except the class name and exisiting functions and varibles.

StudentAI::StudentAI(int col,int row,int p)
	:AI(col, row, p)
{
    board = Board(col,row,p);
    board.initializeGame();
    player = 2;
    me = 'B';
}
int StudentAI::evaluation(Move mymove){
    int result = 0;
    if (player == 1){
        result += board.blackCount - board.whiteCount;
        if (mymove.seq[1][0] == 0 || mymove.seq[1][0] == col - 1){
            result++;
        }
        if (mymove.seq[1][1] == 0 || mymove.seq[1][1] == row - 1){
            result++;
        }
        return result;
    }else{
        result += board.whiteCount - board.blackCount;
        if (mymove.seq[1][0] == 0 || mymove.seq[1][0] == col - 1){
            result++;
        }
        if (mymove.seq[1][1] == 0 || mymove.seq[1][1] == row - 1){
            result++;
        }
        return result;
    };
}
pair<int , Move> StudentAI::maxValue(int depth){
    vector<vector<Move> > moves = board.getAllPossibleMoves(player);
    if (moves.empty()){
        return make_pair(-100, Move());
    }
    int temp = -100;
    pair<int, Move> myPair = make_pair(-100,moves[0][0]);
    if (depth > 0){
        depth --;
        for (int row = 0; row < moves.size(); row++){
            for (int col = 0; col < moves[row].size(); col++){
                Move mymove = moves[row][col];
                board.makeMove(mymove, player);
                if (depth > 0){
                    temp = minValue(depth).first;
                }
                int utility = evaluation(mymove);
                board.Undo();
                if (utility > temp){
                    temp = utility;
                    myPair.first = utility;
                    myPair.second = mymove;
                }
            }
        }
        return myPair;
    }else{
        return myPair;
    }

}
pair<int , Move> StudentAI::minValue(int depth){
    vector<vector<Move> > moves = board.getAllPossibleMoves(player==1?2:1);
    if (moves.empty()){
        return make_pair(-100, Move());
    }
    pair<int, Move> myPair = make_pair(100,moves[0][0]);
    int temp = 100;
    if (depth > 0){
        depth --;
        for (int myrow = 0; row < moves.size(); myrow++){
            for (int mycol = 0; col < moves[myrow].size(); mycol++){
                Move mymove = moves[myrow][mycol];
                board.makeMove(mymove, player==1?2:1);
                if (depth > 0){
                    temp = maxValue(depth).first;
                }
                int utility = evaluation(mymove);
                board.Undo();
                if (utility < temp){
                    temp = utility;
                    myPair.first = utility;
                    myPair.second = mymove;
                }
            }
        }
        return myPair;
    }else{
        return myPair;
    }

}
Move StudentAI::MiniMax(){
    return (maxValue(3)).second;
}
Move StudentAI::GetMove(Move move)
{
    if (move.seq.empty())
    {
        player = 1;
    }else{
        board.makeMove(move,player == 1?2:1);
    }

   /* int i = rand() % (moves.size());
    vector<Move> checker_moves = moves[i];
    int j = rand() % (checker_moves.size());
    Move res = checker_moves[j];
    board.showBoard();
    board.makeMove(res,player);
    board.showBoard();
    return res;*/
   Move mymove = MiniMax();
   board.makeMove(mymove, player);
   return mymove;
}

