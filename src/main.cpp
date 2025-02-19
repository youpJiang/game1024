#include<string>
#include<iostream>
inline void message(const char* str = "", bool lineFeed = true)
{
    std::cout << str;
    if(lineFeed)
         std::cout << std::endl;
}
inline void message(std::string& str, bool lineFeed = true)
{
    message(str.c_str(), lineFeed);
}

int main()
{
    message("hello word!");
    return 0;
}