#include<string>


#include "common.hpp"
#include "scene.hpp"



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
                Message("Game start!");
                break;
            }
            //if 2
            else if(2 == key)
            {
                Message("Game exit!");
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
    ChessBoard& cb = ChessBoard::GetCBInstance();
    // AskStart
    AskStart();

    cb.Play();


    return 0;
}