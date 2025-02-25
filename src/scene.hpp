#include <vector>

#include "common.hpp"

const int g_SIZE = 4;

class ChessBoard
{
private:
    std::vector<std::vector<int>> _chessBoard;

    
public:
    ChessBoard();
    // ~ChessBoard();

    void Play();


};

ChessBoard::ChessBoard()
{
    _chessBoard.resize(g_SIZE, std::vector<int>(g_SIZE,0));
}

void ChessBoard::Play()
{
    std::cout << "start to play!" << std::endl;
    for (const auto& row : _chessBoard) {
        for (int val : row) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }
}

