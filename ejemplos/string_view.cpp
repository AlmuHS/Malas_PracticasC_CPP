#include <iostream>
#include <string_view>

int main(){
  char cadena[5] = "hola";
  char cadena2[5] = "hola";
  char cadena3[5] = "Hola";
  char cadena4[5];
  std::string_view strv_cadena{cadena};

   std::cout << "¿Son las cadenas 1 y 2 iguales? " << ((strv_cadena.compare(cadena2) == 0) ? "si" : "no") << '\n';
   std::cout << "¿Son las cadenas 1 y 3 iguales? " << ((strv_cadena.compare(cadena3) == 0) ? "yes" : "no") << '\n';

  std::cout<<"El contenido del sv es "<<strv_cadena.data()<<"\n";
  
  strv_cadena.copy(cadena4, 4, 0);
  cadena4[4]='\0';
  std::cout<<"El contenido de la cadena 4 es "<<cadena4<<"\n";
  
  
  return 0;

}

