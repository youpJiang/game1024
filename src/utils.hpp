#ifndef UTILS_HPP
#define UTILS_HPP

#ifdef _WIN32
#include <conio.h>
#else
#include <termio.h>
#include <fcntl.h>
#endif //_WIN32

#include <cstdio>
#include <iostream>

/**
 * @brief transformation of the coordinate
 * 
 * transform the linear coordinates randomIndexBoard 
 * into 2D coordinates on a 4 \times 4 chessboard
 * 
 * @param randomIndexBoard linear coordinate
 * @param insertRow row. coordinate
 * @param insertCol col. coordinate
 */
inline void GetRowColFromIndexBoard(int randomIndexBoard,int &insertRow,int &insertCol)
{
    insertRow = randomIndexBoard / 4;
    insertCol = randomIndexBoard % 4;
    return ;
}

#ifdef _WIN32
/**
 * @brief get terminal input on the win platform 
 * 
 * @return the value of the input key
 */
inline char MyGetch(void)
{
    int c = _getch();
    
    // If it's a carriage return and line feed, continue reading
    while (c == '\n' || c == '\r') 
    {
        c = _getch();
    }
    // Handle ESC keys
    if (c == 27) 
    {
        if (!_kbhit()) 
        {
            return 27;  // If there are no subsequent keys, it's the ESC key alone
        }
        else 
        {
            // Empty Input Buffer
            while (_kbhit()) 
            {
                _getch();
            }
        }
    }
    return static_cast<char>(c);
}
#else
/**
 * @brief get terminal input on the linux platform
 * 
 * @return the value of the input key
*/
inline char MyGetch(void)
{
    struct termios tmtemp, tm;
    int c;
    int fd = 0;
    if (tcgetattr(fd, &tm) != 0)
    {
        // get the current terminal attribute settings 
        // and save them to the tm structure
        return -1;
    }
    tmtemp = tm;
    cfmakeraw(&tmtemp); // initialize tetemp to the attribute settings of the terminal's original mode
    if (tcsetattr(fd, TCSANOW, &tmtemp) != 0)
    {
        // set the terminal to the settings of the original mode
        return -1;
    }
    c = getchar();
    while (c == '\n' || c == '\r') 
    {  
        // If it's a carriage return and line feed, continue reading
        c = getchar();
    }
    if (c == 27)  // ESC returns 27, up and down as (27,91,xx) Conflict with ESC's 27
    {
        int back_flags = fcntl(fd, F_GETFL);
        // Set fd to non-blocking to return immediately when there is no input
        fcntl(fd, F_SETFL, back_flags | O_NONBLOCK);
        c = getchar();
        if (c == EOF)
        {
            c = 27;
        }
        else
        {
            while ( (c = getchar()) != EOF)
            {
            }
        }
        fcntl(fd, F_SETFL, back_flags);
    }
    if (tcsetattr(fd, TCSANOW, &tm) != 0)
    {
        // set the terminal back to its original properties 
        // after receiving characters
        return 0;
    }
    return static_cast<char>(c);
}
#endif // _WIN32
#endif // UTILS_HPP