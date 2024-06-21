#include <cstdio>
#include <iostream>

using namespace std;

int main(void){
  printf("Introduce un mensaje: ");

  string message;

  //Error: ambas librerías tienen getline() ¿a cual nos referimos?
  //getline(&cin, &message, '\n'); //si cometemos un error en los parámetros, no sabrá que función usar
  getline(cin, message, '\n'); 

  cin.ignore();
  cout<<message;
  return 0;
}
