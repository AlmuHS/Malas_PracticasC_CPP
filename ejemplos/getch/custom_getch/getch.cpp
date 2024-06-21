#include <iostream>
#include "getch.hpp"

#ifdef WIN32

#include <windows.h>

char getch(){
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode = 0;
    GetConsoleMode(hStdin, &mode);
    SetConsoleMode(hStdin, mode & (~ENABLE_ECHO_INPUT) & (~ENABLE_LINE_INPUT));

    char data;
    ReadFile(hStdin, &data, 1, NULL, nullptr);

    return data;
}


#else

#include <unistd.h>
#include <termios.h>

char getch(void){
    termios TermConf, TermCurConf;

    //Store default console config
    tcgetattr(STDIN_FILENO, &TermCurConf);

    //Change current config to disable new lines, echo, and flush \n
    tcgetattr(STDIN_FILENO, &TermConf);
    TermConf.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW | TCSAFLUSH, &TermConf);

    //Read the data
    char data;
    data = std::cin.get();
    tcflush(STDIN_FILENO, TCIOFLUSH);


    //Recover the default console config
    tcsetattr(STDIN_FILENO, TCSANOW, &TermCurConf);

    return data;
}

#endif // WIN32