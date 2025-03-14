#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>
#include <unordered_map>
#include <iomanip>
#include <unistd.h>
#include <cstdlib>
#include <ctime>
#include <fstream>

#include "common.hpp"
#include "utils.hpp"


enum class Direction {
    Up,
    Down,
    Left,
    Right
};

class ChessBoard
{
private:
    std::vector<std::vector<int>> _chessBoard;
    static constexpr int _size = 4;
    static constexpr int _cell_width = 5;
    int _score;
    int _score_record;
    std::unordered_map<int, int> _score_map;
    bool _isWin;

    static void PrintChessboardBorder();
    void PrintChessboard() const;
    void Show() const;

    bool Move(Direction);
    int RandomGenerate(bool isMove = true);
    bool Merge(std::vector<int>& vec);
    void SaveScore();
protected:
    ChessBoard();
    ChessBoard(const ChessBoard&)=delete;
    ChessBoard& operator=(const ChessBoard)=delete;
public:
    static ChessBoard& GetCBInstance();

    void Play();
};


ChessBoard& ChessBoard::GetCBInstance()
{
    static ChessBoard _cb_instance;
    return _cb_instance;
}

void ChessBoard::PrintChessboardBorder()
{
    Message("+", false);
    for (int i = 0; i < _size; ++i) {
        Message(std::string(_cell_width, '-'), false);
        Message("+", false);
    }
    Message("");
}

void ChessBoard::PrintChessboard() const{
    PrintChessboardBorder();

    // print the whole chessborad.
    for (int i = 0; i < _size; ++i) {
        Message("|", false);
        for (int j = 0; j < _size; ++j) {
            int number = _chessBoard[i][j];
            std::string numberStr = std::to_string(number);
            int numberWidth = numberStr.length();

            // calculate padding.
            int leftPadding = (_cell_width - numberWidth) / 2;
            int rightPadding = _cell_width - numberWidth - leftPadding;

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

ChessBoard::ChessBoard():_isWin(false), _score(0), _score_record(0)
{
    std::srand(std::time(0)); //set random seed.

    std::ifstream inputFile("record.txt");
    if(inputFile) inputFile >> _score_record;

    _chessBoard.resize(_size, std::vector<int>(_size,0));

    //number:score
    _score_map = {
        {1024,100},
        {512,80},
        {256,50},
        {128,30},
        {64,20},
        {32,10},
        {16,5},
        {8,3},
        {4,2}
    };
    //init two numbers
    RandomGenerate();
    RandomGenerate();
}

void ChessBoard::SaveScore()
{
    if(_score > _score_record)
    {
        std::ofstream outFile("record.txt");

        if(!outFile.is_open())
        {
            std::cerr << "Can not open the record file." << std::endl;
            return ;
        }
        outFile << _score;
        // outFile.close();
        Message("Your score has been recorded.");
        Message("Game exit!");
    }
}

void ChessBoard::Show() const
{
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif //_WIN32

    Message("[Instruction]: Press <w> <s> <a> <d> to move");
    Message("[Game Record]: ", false);
    std::cout << _score_record << std::endl;
    Message("[Current Score]: ", false);
    std::cout << _score << std::endl;
    PrintChessboard();
}

void ChessBoard::Play()
{
    Show();
    char key = '\0';
    while(1)
    {
        bool isMove = false;
        key = MyGetch();
        if('w' == key)
        {
            if(Move(Direction::Up))
                isMove = true;
        }
        else if('s' == key)
        {
            if(Move(Direction::Down))
                isMove = true;
        }
        else if('a' == key)
        {
            if(Move(Direction::Left))
                isMove = true;
        }
        else if('d' == key)
        {
            if(Move(Direction::Right))
                isMove = true;
        }
        else if('q' == key)
        {
            SaveScore();
            exit(0);
        }
        else{
            Message("Invalid input! Please try again.");
            continue;
        }
        if(RandomGenerate(isMove))
        {
            SaveScore();
            Message("~~~~~~~ YOU LOSE! ~~~~~~~");
            exit(0);
        }
        Show();
        if(_isWin)
        {
            SaveScore();
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
            if(it+1 != vec.end() && it != vec.begin() && *(it-1) == *(it+1))
            {
                isMove = true;
                *(it-1) *= 2;
                _score += _score_map[*(it-1)];
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
            _score += _score_map[*it];
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

bool ChessBoard::Move(Direction dir) {
    bool isMove = false;

    for (int i = 0; i < _size; ++i) {
        std::vector<int> vec;

        // get row or col accoding to direction
        if (dir == Direction::Up || dir == Direction::Down) {
            for (int j = 0; j < _size; ++j) {
                int row = (dir == Direction::Up) ? j : _size - 1 - j;
                vec.push_back(_chessBoard[row][i]);
            }
        } else if (dir == Direction::Left || dir == Direction::Right) {
            for (int j = 0; j < _size; ++j) {
                int col = (dir == Direction::Left) ? j : _size - 1 - j;
                vec.push_back(_chessBoard[i][col]);
            }
        }

        if (Merge(vec)) {
            isMove = true;
        }

        // fill the chess back
        if (dir == Direction::Up || dir == Direction::Down) {
            for (int j = 0; j < _size; ++j) {
                int row = (dir == Direction::Up) ? j : _size - 1 - j;
                _chessBoard[row][i] = vec[j];
            }
        } else if (dir == Direction::Left || dir == Direction::Right) {
            for (int j = 0; j < _size; ++j) {
                int col = (dir == Direction::Left) ? j : _size - 1 - j;
                _chessBoard[i][col] = vec[j];
            }
        }
    }

    return isMove;
}

int ChessBoard::RandomGenerate(bool isMove)
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
            }
        }
    }
    if(!flag)
    {
        //no block to fill, you lose.
        return 1;
    }
    //generate a position randomly and fill a number
    else if(isMove)
    {
        int randomIndexVec = rand() % zeroBlocks.size();
        int randomIndexBoard = zeroBlocks[randomIndexVec];
        int insertRow = 0;
        int insertCol = 0;
        GetRowColFromIndexBoard(randomIndexBoard, insertRow, insertCol);
        _chessBoard[insertRow][insertCol] = (0 == (rand() % 10))? 4 : 2;
    }
    return 0;
}

#endif // SCENE_HPP