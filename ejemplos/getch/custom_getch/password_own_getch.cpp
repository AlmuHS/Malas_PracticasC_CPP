#include "getch.hpp"
#include <cstdio>
#include <iostream>

int main(void) {  
    std::cout<<"Introduce la contraseña\n";

    char c;
    char password[35];

    int i = 0;
    do{
        c = getch();
        password[i] = c;
        std::cout<<"*";
        i++;
    }while (c != '\n');
    
    printf("\nLa contraseña es %s", password);

    return 0;
}