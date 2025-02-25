#include<string>
#include<iomanip>

#include "common.hpp"

const int SIZE = 4;
const int CELL_WIDTH = 5; 


inline void PrintCover()
{
    Message("===================================");
    Message("|         WELCOME TO GAME         |");
    Message("|    _       __     ___    _ _    |");
    Message("|   / |     /  \\   |_  )  | | |   |");
    Message("|   | |    | () |   / /   |_  _|  |");
    Message("|  _|_|_   _\\__/   /___|   _|_|_  |");
    Message("|_|\"\"\"\"\"|_|\"\"\"\"\"|_|\"\"\"\"\"|_|\"\"\"\"\"| |");
    Message("|\"`-0-0-\'\"`-0-0-\'\"`-0-0-\'\"`-0-0-\' |");
    Message("===================================");
}


inline void PrintChessboardBorder()
{
    std::cout << "+";
    for (int i = 0; i < SIZE; ++i) {
        std::cout << std::string(CELL_WIDTH, '-') << "+";
    }
    std::cout << std::endl;
}
inline void PrintChessboard() {
    // 打印棋盘的顶部边框
    PrintChessboardBorder();

    // 打印棋盘内容
    for (int i = 0; i < SIZE; ++i) {
        std::cout << "|";
        for (int j = 0; j < SIZE; ++j) {
            // 计算当前格子的数字
            int number = (i * SIZE + j + 1)*100;
            // 将数字转换为字符串
            std::string numberStr = std::to_string(number);
            int numberWidth = numberStr.length();

            // 计算左右填充的空格数量
            int leftPadding = (CELL_WIDTH - numberWidth) / 2;
            int rightPadding = CELL_WIDTH - numberWidth - leftPadding;

            // 打印数字并居中
            std::cout << std::string(leftPadding, ' ') << numberStr << std::string(rightPadding, ' ') << "|";
        }
        std::cout << std::endl;

        PrintChessboardBorder();
    }
}

inline void AskStart()
{
    Message("Please select:");
    Message("[1] Start Game    [2] Exit ");
    std::string input;
    
    do{
        try{
            std::cin >> input;
            int key = stoi(input);
            //if 1
            if(1 == key)
            {
                //play
                Message("game start!");
                exit(0);
            }
            //if 2
            else if(2 == key)
            {
                Message("game exit!");
                exit(0);
            }
            else
                throw std::runtime_error("");
        }catch(const std::exception& e){
            Message("Invalid Input! Please enter 1 or 2:");
        }

    }while(true);

}

int main()
{
    PrintCover();
    //init game
    //AskStart
    AskStart();
    
    return 0;
}