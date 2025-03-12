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


class ChessBoard
{
private:
    std::vector<std::vector<int>> _chessBoard;
    const int _size = 4;
    const int _cell_width = 5;
    int _score;  //target+score: 1024+100,512+80,256+50,128+30,64+15,32+10,16+5,8+3,4+2
    int _score_record;
    std::unordered_map<int, int> _score_map;
    bool _isWin;

    void PrintChessboardBorder() const;
    void PrintChessboard() const;
    void Show();

    bool Up();
    bool Down();
    bool Left();
    bool Right();
    int RandomGenerate(bool isMove = true);
    bool Merge(std::vector<int>& vec);
    void CalScore(int number);
    void SaveScore();

public:
    ChessBoard();
    // ~ChessBoard();

    void Play();
};

void ChessBoard::PrintChessboardBorder() const
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

ChessBoard::ChessBoard():_isWin(false), _score(0)
{
    std::ifstream inputFile("record.txt");
    if(!inputFile.is_open())
    {
        std::cerr << "Record file can not open." << std::endl;
        _score_record = 0;
    }
    inputFile >> _score_record;
    if(inputFile.fail())
    {
        std::cerr << "Record read error!" << std::endl;
    }
    // inputFile.close();

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
    //init two number
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
    }
}

void ChessBoard::Show()
{
    system("clear");
    Message("Record Score: ", false);
    std::cout << _score_record << std::endl;
    Message("Current Score: ", false);
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
            SaveScore();
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
            if(it+1 != vec.end() && *(it-1) == *(it+1))
            {
                isMove = true;
                *(it-1) *= 2;
                CalScore(*(it-1));
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
            CalScore(*it);
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

void ChessBoard::CalScore(int number)
{
    switch(number)
            {
                case 1024:
                    _isWin = true;
                    _score += _score_map[number];
                    break;
                case 512:
                case 256:
                case 128:
                case 64:
                case 32:
                case 16:
                case 8:
                case 4:
                    _score += _score_map[number];
                    break;
                default:
                    break;
            }
    return ;
}

bool ChessBoard::Up()
{
    bool isMove = false;
    //traverse every column
    for(int col = 0; col < _size; ++col)
    {
        //to vector
        std::vector<int> colVec;
        for(int row = 0 ; row < _size; ++row)
        {
            colVec.push_back(_chessBoard[row][col]);
        }
        //call Merge
        if(Merge(colVec))
        {
            isMove = true;
        }
        //fill back
        for(int row = 0 ; row < _size; ++row)
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
    for(int col = 0; col < _size; ++col)
    {
        //to vector
        std::vector<int> colVec;
        for(int row = _size-1 ; row > -1; --row)
        {
            colVec.push_back(_chessBoard[row][col]);
        }
        //call Merge
        if(Merge(colVec))
        {
            isMove = true;
        }
        //fill back
        for(int row = _size-1 ; row > -1; --row)
        {
            _chessBoard[_size-row-1][col] = colVec[row];
        }
    }
    return isMove;
}

bool ChessBoard::Left()
{
    bool isMove = false;
    //traverse every row
    for(int row = 0; row < _size; ++row)
    {
        //to vector
        std::vector<int> rowVec;
        for(int col = 0 ; col < _size; ++col)
        {
            rowVec.push_back(_chessBoard[row][col]);
        }
        //call Merge
        if(Merge(rowVec))
        {
            isMove = true;
        }
        //fill back
        for(int col = 0 ; col < _size; ++col)
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

    for(int row = 0; row < _size; ++row)
    {
        //to vector
        std::vector<int> rowVec;
        for(int col = _size ; col > -1; --col)
        {
            rowVec.push_back(_chessBoard[row][col]);
        }
        //call Merge
        if(Merge(rowVec))
        {
            isMove = true;
        }
        //fill back
        for(int col = 0 ; col < _size; ++col)
        {
            _chessBoard[row][_size-col-1] = rowVec[col];
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
    else if(isMove)
    {
        std::srand(std::time(0)); //set random seed.
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
        _chessBoard[insertRow][insertCol] = (0 == (rand() % 10))? 4 : 2;
    }
    return 0;
}



#endif // SCENE_HPP