#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>
#include <iomanip>

#include "common.hpp"

const int g_SIZE = 4;
const int g_CELL_WIDTH = 5; 


class ChessBoard
{
private:
    std::vector<std::vector<int>> _chessBoard;

    inline void PrintChessboard();

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

inline void ChessBoard::PrintChessboard() {
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

inline ChessBoard::ChessBoard()
{
    _chessBoard.resize(g_SIZE, std::vector<int>(g_SIZE,0));
}

void ChessBoard::Play()
{
    // for (const auto& row : _chessBoard) {
    //     for (int val : row) {
    //         std::cout << val << " ";
    //     }
    //     std::cout << std::endl;
    // }
    system("clear");
    PrintChessboard();
}

#endif // SCENE_HPP