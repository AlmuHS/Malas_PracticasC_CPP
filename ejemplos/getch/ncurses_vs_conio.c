#include <curses.h>
#include <stdio.h>
#include <ncurses.h>

int main(void) {  
    initscr();
    noecho();

    printw("Introduce la contraseña\n");

    char c = ' ';
    char password[35];

    int i = 0;
    do{
        c = getch();
        password[i] = c;
        addch('*');
        i++;
    }while (c != '\n');
    
    endwin();

    printf("La contraseña es %s", password);

    return 0;
}

