#include <string>
#include <iostream>
	
	int main(){
		std::string cadena1{"hola mundo"};
		std::string cadena2{cadena1};
		std::string cadena3 = "hola";
		
		cadena3 += " mundo";
		
		if(cadena3 == "hola mundo") std::cout<<"Cadena 3 concatenada correctamente\n";
		else std::cout<<"El contenido de cadena 3 es "<<cadena3<<"\n";
		
		if(cadena1 == cadena3) std::cout<<"cadena1 y cadena3 son iguales\n";
		else std::cout<<"cadena 1 y cadena 3 no son iguales\n";
		
		return 0;
	}
