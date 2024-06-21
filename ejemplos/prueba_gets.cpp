#include <iostream>
#include <cstdio>
#include <cstring>

//compile with -std=c++11

int main(){
  char cadena2[11] = "cadena2";
  char cadena1[10] = "prueba";
  
  std::cout<<"Introduce una palabra: ";
  gets(cadena1);
  //std::cin.getline(cadena1, '\n');  

  std::cout<<"El contenido de la cadena1 es "<<cadena1<<"\n";
  std::cout<<"El contenido de la cadena2 es "<<cadena2<<"\n";
  
  return 0;
}
