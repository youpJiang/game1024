#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include<iostream>
inline void Message(const char* str = "", bool lineFeed = true)
{
    std::cout << str;
    if(lineFeed)
         std::cout << std::endl;
}

inline void Message(std::string& str, bool lineFeed = true)
{
    Message(str.c_str(), lineFeed);
}

#endif  // MESSAGE_HPP