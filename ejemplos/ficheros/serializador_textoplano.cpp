#include <cstdio>
#include <iostream>

#include <fstream>
#include <sstream>
#include <iomanip>

#include <cstring>
#include <string>
#include <charconv> // from_char, to_char
#include <cstdint>


struct datos{
       int32_t dato1;
       float dato2;
       char cadena[20];
};
          
          
void serializador_escritura_struct(std::ofstream& fichero_texto, const datos &datos_obj){  
       fichero_texto << datos_obj.dato1 << "|" << datos_obj.dato2 << "|"  << datos_obj.cadena << '\n' ;
}

datos convertir_tipos_chars(std::string datos_str[4]){ 
       datos datos_obj;

       std::from_chars(datos_str[0].data(), datos_str[0].data() + datos_str[0].size(), datos_obj.dato1);
       std::from_chars(datos_str[1].data(), datos_str[1].data() + datos_str[1].size(), datos_obj.dato2);

       uint16_t tam_cadena;
       std::from_chars(datos_str[2].data(), datos_str[2].data() + datos_str[2].size(), tam_cadena);

       snprintf(datos_obj.cadena, datos_str[3].length()+1, "%s", datos_str[3].c_str());

       return datos_obj;

}

datos convertir_tipos_stringstream(std::string datos_str[4]){
       datos datos_obj;

       //Creamos un stringstream auxiliar para copiar las cadenas a convertir
       std::stringstream stream_aux;

       //Convertimos el primer dato a int
       stream_aux<<datos_str[0]; 
       stream_aux>>datos_obj.dato1;

       //Convertimos el segundo dato a float
       stream_aux<<datos_str[1]; 
       stream_aux>>datos_obj.dato2;

       //Convertimos el std::string a una cadena de caracteres
       snprintf(datos_obj.cadena, datos_str[3].length()+1, "%s", datos_str[3].c_str());

       return datos_obj;

}


void deserializador_lectura_struct(std::ifstream& fichero, datos &datos_obj){
       
       std::string datos_str[4];
       
       //Dado que no sabemos el tamaño de la cadena, usamos std::getline() para obtener un std::string de tamaño dinámico
       std::getline(fichero, datos_str[0], '|'); 
       
       //Repetimos el proceso con el segundo dato
       std::getline(fichero, datos_str[1], '|');        

       //Leemos la cadena
       std::getline(fichero, datos_str[3], '\n');
               
               
       //Reconvertimos los datos a sus tipos originales

       //datos_obj = convertir_tipos_stringstream(datos_str);
       datos_obj = convertir_tipos_chars(datos_str);

}

int main(){
       
       //Abrimos el fichero y escribimos
       std::string nom_fichero = "fichero";
       std::ofstream fichero_texto(nom_fichero.c_str()); 

       datos datos_obj{23, 45.8, "prueba"};
       serializador_escritura_struct(fichero_texto, datos_obj);

       datos datos_obj2{30, 70.965, "test con espacios"};
       serializador_escritura_struct(fichero_texto, datos_obj2);

       fichero_texto.close();


       //Abrimos el fichero y leemos
       std::ifstream fichero(nom_fichero.c_str());
       datos datos_leidos;
       deserializador_lectura_struct(fichero, datos_leidos);

       std::cout<<datos_leidos.dato1<<" "<<datos_leidos.dato2<<" "<<datos_leidos.cadena<<"\n";

       deserializador_lectura_struct(fichero, datos_leidos);

       std::cout<<datos_leidos.dato1<<" "<<datos_leidos.dato2<<" "<<datos_leidos.cadena<<"\n";

       fichero.close();

       return 0;

}

