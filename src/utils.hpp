#ifndef UTILS_HPP
#define UTILS_HPP

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#else
#include <termio.h>
#include <fcntl.h>
#endif //_WIN32

#include <cstdio>
#include <iostream>

/// @brief 线性索引转换为二维坐标(4*4)
/// @param randomIndexBoard 线性索引
/// @param insertRow 行坐标
/// @param insertCol 列坐标
inline void getRowColFromIndexBoard(int randomIndexBoard,int &insertRow,int &insertCol)
{
    insertRow = randomIndexBoard / 4;
    insertCol = randomIndexBoard % 4;
    return ;
}

#ifdef _WIN32
inline char myGetch(void)
{
    int c = _getch();
    
    // 如果是回车和换行，继续读取
    while (c == '\n' || c == '\r') {
        c = _getch();
    }
    // 处理ESC键
    if (c == 27) {
        if (!_kbhit()) {
            return 27;  // 如果没有后续按键，就是单独的ESC键
        }
        else {
            // 清空输入缓冲区
            while (_kbhit()) {
                _getch();
            }
        }
    }
    return static_cast<char>(c);
}
#else
/// @brief 获取终端输入
/// @param void
/// @return 输入字母
inline char myGetch(void)
{
    struct termios tmtemp, tm;
    int c;
    int fd = 0;
    if (tcgetattr(fd, &tm) != 0)
    { /*获取当前的终端属性设置，并保存到tm结构体中*/
        return -1;
    }
    tmtemp = tm;
    cfmakeraw(&tmtemp); /*将tetemp初始化为终端原始模式的属性设置*/
    if (tcsetattr(fd, TCSANOW, &tmtemp) != 0)
    { /*将终端设置为原始模式的设置*/
        return -1;
    }
    c = getchar();
    while (c == '\n' || c == '\r') {  // 如果是换行符或回车符，继续读取
        c = getchar();
    }
    if (c == 27)  /* ESC返回27，上下左右为(27,91,xx) 与ESC的27冲突 */
    {
        int back_flags = fcntl(fd, F_GETFL);
        /*将fd设置为非阻塞的，没有输入时可以立即返回*/
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
    { /*接收字符完毕后将终端设置回原来的属性*/
        return 0;
    }
    return static_cast<char>(c);
}
#endif // _WIN32
#endif // UTILS_HPP