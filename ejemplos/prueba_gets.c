#include <stdio.h>
#include <string.h>

int main(){
  char cadena2[11] = "cadena2";
  char cadena1[10] = "prueba";
  
  printf("Introduce una palabra: ");
  gets(cadena1);
  
  printf("El contenido de la cadena1 es %s\n", cadena1);
  printf("El contenido de la cadena2 es %s\n", cadena2);
  
  return 0;
}
