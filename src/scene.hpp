#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>
#include <iomanip>
#include <unistd.h>
#include <cstdlib>

#include "common.hpp"
#include "utils.hpp"

const int g_SIZE = 4;
const int g_CELL_WIDTH = 5;

inline void PrintChessboardBorder();

class ChessBoard
{
private:
    std::vector<std::vector<int>> _chessBoard;
    bool _isWin;
    int _randomSeed24 = 0;

    void PrintChessboard() const;
    void Show();

    bool Up();
    bool Down();
    bool Left();
    bool Right();
    int RandomGenerate(bool&);
    bool Merge(std::vector<int>& vec);

public:
    ChessBoard();
    // ~ChessBoard();

    void Play();


};

inline void PrintChessboardBorder()
{
    Message("+", false);
    for (int i = 0; i < g_SIZE; ++i) {
        Message(std::string(g_CELL_WIDTH, '-'), false);
        Message("+", false);
    }
    Message("");
}

void ChessBoard::PrintChessboard() const{
    PrintChessboardBorder();

    // print the whole chessborad.
    for (int i = 0; i < g_SIZE; ++i) {
        Message("|", false);
        for (int j = 0; j < g_SIZE; ++j) {
            int number = _chessBoard[i][j];
            std::string numberStr = std::to_string(number);
            int numberWidth = numberStr.length();

            // calculate padding.
            int leftPadding = (g_CELL_WIDTH - numberWidth) / 2;
            int rightPadding = g_CELL_WIDTH - numberWidth - leftPadding;

            // print number and mediate
            Message(std::string(leftPadding, ' '), false);
            if("0" == numberStr)
                Message(" ", false);
            else
                Message(numberStr,false);
            Message(std::string(rightPadding, ' '), false);
            Message("|",false);
        }
        Message("");

        PrintChessboardBorder();
    }
}

ChessBoard::ChessBoard()
{
    _isWin = false;
    _chessBoard.resize(g_SIZE, std::vector<int>(g_SIZE,0));
    _chessBoard = {{0,2,0,0},{2,0,0,0},{0,0,0,0},{0,0,0,0}};
}

void ChessBoard::Show()
{
    system("clear");
    PrintChessboard();
}

void ChessBoard::Play()
{
    Show();
    char key = '\0';
    while(1)
    {
        bool isMove = false;
        key = getch();
        if('w' == key)
        {
            if(Up())
                isMove = true;
        }
        else if('s' == key)
        {
            if(Down())
                isMove = true;
        }
        else if('a' == key)
        {
            if(Left())
                isMove = true;
        }
        else if('d' == key)
        {
            if(Right())
                isMove = true;
        }
        else if('q' == key)
        {
            exit(0);
        }
        else{
            Message("Invalid input! Please try again.");
            continue;
        }
        // if(isMove)
        //     Message("move!");
        // else
        //     Message("hold!");
        if(RandomGenerate(isMove))
        {
            Message("~~~~~~~ YOU LOSE! ~~~~~~~");
            exit(0);
        }
        Show();
        if(_isWin)
        {
            Message("******* YOU WIN! ******");
            exit(0);
        }
    }
}

bool ChessBoard::Merge(std::vector<int>& vec)
{
    bool isMove = false;
    int paddingCount = 0;
    for(auto it = vec.begin(); it != vec.end();)
    {
        //case:0
        if(0 == *it)
        {
            //zero between two same non-zero numbers.
            if(it+1 != vec.end() && *(it-1) == *(it+1))
            {
                isMove = true;
                *(it-1) *= 2;
                it = vec.erase(it);
                paddingCount += 2 ;
            }
            //zero before a non-zero number.
            else if(it+1 != vec.end() && 0 != *(it+1))
            {
                isMove = true;
                paddingCount++;
            }
            else
            {
                paddingCount++;
            }
            it = vec.erase(it);
        }
        //case:vec[i] == vec[i+1]-> delete the second & double the first. count++
        else if(it+1 != vec.end() && *it == *(it+1))
        {
            isMove = true;
            *it *= 2;
            if(1024 == *it)
                _isWin = true;
            it = vec.erase(it+1);
            paddingCount++;
        }
        else
        {
            ++it;
        }
    }
    //padding 0
    for(int i = 0; i < paddingCount; ++i)
    {
        vec.push_back(0);
    }
    if(4 == paddingCount)
        isMove = false;
    return isMove;
}

bool ChessBoard::Up()
{
    bool isMove = false;
    //traverse every column
    for(int col = 0; col < g_SIZE; ++col)
    {
        //to vector
        std::vector<int> colVec;
        for(int row = 0 ; row < g_SIZE; ++row)
        {
            colVec.push_back(_chessBoard[row][col]);
        }
        //call Merge
        if(Merge(colVec))
        {
            isMove = true;
        }
        //fill back
        for(int row = 0 ; row < g_SIZE; ++row)
        {
            _chessBoard[row][col] = colVec[row];
        }
    }
    return isMove;
}

bool ChessBoard::Down()
{
    bool isMove = false;
    //traverse every column
    for(int col = 0; col < g_SIZE; ++col)
    {
        //to vector
        std::vector<int> colVec;
        for(int row = g_SIZE-1 ; row > -1; --row)
        {
            colVec.push_back(_chessBoard[row][col]);
        }
        //call Merge
        if(Merge(colVec))
        {
            isMove = true;
        }
        //fill back
        for(int row = g_SIZE-1 ; row > -1; --row)
        {
            _chessBoard[g_SIZE-row-1][col] = colVec[row];
        }
    }
    return isMove;
}

bool ChessBoard::Left()
{
    bool isMove = false;
    //traverse every row
    for(int row = 0; row < g_SIZE; ++row)
    {
        //to vector
        std::vector<int> rowVec;
        for(int col = 0 ; col < g_SIZE; ++col)
        {
            rowVec.push_back(_chessBoard[row][col]);
        }
        //call Merge
        if(Merge(rowVec))
        {
            isMove = true;
        }
        //fill back
        for(int col = 0 ; col < g_SIZE; ++col)
        {
            _chessBoard[row][col] = rowVec[col];
        }
    }
    return isMove;
}

bool ChessBoard::Right()
{
    bool isMove = false;
    //traverse every row

    for(int row = 0; row < g_SIZE; ++row)
    {
        //to vector
        std::vector<int> rowVec;
        for(int col = g_SIZE ; col > -1; --col)
        {
            rowVec.push_back(_chessBoard[row][col]);
        }
        //call Merge
        if(Merge(rowVec))
        {
            isMove = true;
        }
        //fill back
        for(int col = 0 ; col < g_SIZE; ++col)
        {
            _chessBoard[row][g_SIZE-col-1] = rowVec[col];
        }
    }
    return isMove;
}

int ChessBoard::RandomGenerate(bool& isMove)
{
    std::vector<int> zeroBlocks;
    bool flag = false;
    for(int row = 0; row < 4; ++row)
    {
        for(int col = 0; col < 4; ++col)
        {
            if(0 == _chessBoard[row][col])
            {
                zeroBlocks.push_back(4*row+col);
                flag = true;
                // if(isMove)
                // {
                //     it = (0 == _randomSeed)? 4 : 2;
                //     _randomSeed++;
                //     _randomSeed %= 10;
                // }
            }
        }
    }
    if(!flag)
    {
        //no block to fill, you lose.
        return 1;
    }
    //generate a position randomly and fill a number
    else
    {
        std::srand(3); //set random seed.
        int randomIndexVec = rand() % zeroBlocks.size();
        // std::cout << "zeroBlocks.size() is :" << zeroBlocks.size() << std::endl;
        // std::cout << "randomIndexVec is :" << randomIndexVec << std::endl;
        int randomIndexBoard = zeroBlocks[randomIndexVec];
        // std::cout << "randomIndexBoard is :" << randomIndexBoard << std::endl;
        int insertRow = 0;
        int insertCol = 0;
        getRowColFromIndexBoard(randomIndexBoard, insertRow, insertCol);
        // std::cout << "insertRow is :" << insertRow << std::endl;
        // std::cout << "insertCol is :" << insertCol << std::endl;
        _chessBoard[insertRow][insertCol] = (0 == _randomSeed24)? 4 : 2;
        _randomSeed24++;
        _randomSeed24 %= 10;
        return 0;
    }

}







#endif // SCENE_HPP