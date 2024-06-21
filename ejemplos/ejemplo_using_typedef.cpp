#include <iostream>
#include <string>

using cadena = char[100];

//using str = std::string{"hola"};

void mifuncion(cadena cad){
	/*for (typedef int Foo; Foo{} != 0;) //creamos el tipo dentro del inicializador del for
	{
          std::cout<<"cad\n";
	}*/
	
	for (using Foo = int; Foo{} != 0;) {
	  std::cout<<"cad\n";
	}
}

int main(){
  cadena cad;
  mifuncion(cad);
}
