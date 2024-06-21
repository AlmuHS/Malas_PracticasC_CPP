#include <cstdio>
#include <iostream>

using namespace std;

int main(void){
  printf("Introduce un mensaje: ");

  char *string_ptr = NULL;
  size_t size_str = 0;
  ssize_t chars_read; 

  //Error: ambas librerías tienen getline() ¿a cual nos referimos?
  getline(&string_ptr, &size_str, stdin); 

  cin.ignore();
  printf("%s", string_ptr);
  return 0;
}
