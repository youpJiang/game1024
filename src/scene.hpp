#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>
#include <iomanip>

#include "common.hpp"

const int g_SIZE = 4;
const int g_CELL_WIDTH = 5; 

inline void PrintChessboardBorder();
void Merge(std::vector<int>& vec);

class ChessBoard
{
private:
    std::vector<std::vector<int>> _chessBoard;
    bool _isWin;

    void PrintChessboard() const;
    void Show();

    void Up();
    void Down();
    void Left();
    void Right();
    void Merge(std::vector<int>& vec);

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
    _chessBoard.resize(g_SIZE, std::vector<int>(g_SIZE,1));
}

void ChessBoard::Show()
{
    // system("clear");
    PrintChessboard();
    Up();
    PrintChessboard();
    Up();
    PrintChessboard();
    Up();
    PrintChessboard();
    Up();
    PrintChessboard();
}

void ChessBoard::Play()
{
    Show();

}

void ChessBoard::Merge(std::vector<int>& vec)
{
    // Message("Before:");
    // for(auto &it:vec)
    // {
    //     std::cout << it <<' ';
    // }
    // std::cout << std::endl;


    int paddingCount = 0;
    for(auto it = vec.begin(); it != vec.end();)
    {
        //delete 0,count++
        if(0 == *it)
        {
            it = vec.erase(it);
            paddingCount++;
        }
        //vec[i] == vec[i+1]: delete & double. count++
        else if(it+1 != vec.end() && *it == *(it+1))
        {
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
    
    // Message("After:");
    // for(auto &it:vec)
    // {
    //     std::cout << it <<' ';
    // }
    // std::cout << std::endl;
}

void ChessBoard::Up()
{
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
        Merge(colVec);
        //fill back
        for(int row = 0 ; row < g_SIZE; ++row)
        {
            _chessBoard[row][col] = colVec[row];
        }
    }
}





#endif // SCENE_HPP