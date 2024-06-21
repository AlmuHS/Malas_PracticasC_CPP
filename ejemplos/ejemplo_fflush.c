#include <stdio.h>

int main(){
    int valor;
    printf("Introduce un valor: ");
    scanf("%d", &valor);

    //fflush(stdin);
    char ch;
    while((ch = getchar() != '\n') && ch != EOF);

    char caracter;
    printf("Introduce un caracter: ");
    scanf("%c", &caracter);

    

    return 0;
}