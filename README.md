# Malas prácticas en C y C++ y su solución

## Introducción

En este documento expondré algunas malas prácticas habituales en C y C++ con fácil solución, explicando las causas de ser consideradas así, y algunas propuestas para evitarlas

## Cadenas de caracteres

### Mala práctica 1: `gets()`: 

Esta función es una mala práctica, prohibida en cualquier entorno de desarrollo profesional. Esto se debe a que gets() no comprueba que la cadena recibida por teclado quepa en el array de char usado para almacenarlo.
   
   Esto hace que, una vez superado el tamaño del array de char, gets() siga escribiendo, lo cual puede producir que algunas variables se sobreescriban o incluso que el programa se cuelgue por completo.
      
   Para evitarlo, existen distintas alternativas, según estemos en C o en C++
   
#### Alternativas para C
    
1. `fgets()`: El reemplazo mas sencillo es la función `fgets()`, que permite indicar un tamaño máximo de lectura. Esta función controla que la cadena recibida no sobrepase el tamaño indicado, cortando los caracteres restantes. 

	La sintaxis de la función es:
			
	   fgets(char array_char[], int num_caracteres_leer, FILE buffer_entrada);
			   
   Esta función está pensada para ficheros, por lo que para indicar que lea de teclado debemos indicar en el buffer de entrada el nombre stdin, correspondiente al buffer de entrada estándar de teclado. 
		  
   Un ejemplo de uso sería:
		   
       char cadena[MAX];
       fgets(cadena, MAX, stdin);
       
               
2. `getline()`: Esta función, **disponible a partir de C11**, es algo mas sofisticada, reservando dinámicamente el espacio necesario para almacenar la cadena. Por esta razón, no se puede usar con array de char estáticos, sino que debe recibir un puntero a un array de char.  
              
	Para saber el tamaño de la cadena recibida, la función recibe un parámetro adicional por puntero que se actualizará con el tamaño recibido. 
              
	Su sintaxis es:
			
		size_t getline(char** puntero_cadena, size_t *tamano_recibido, FILE buffer_entrada);
			  
		  
   La función devuelve el número de caracteres leídos. 
              
	De nuevo, esta función está pensada para ficheros, por lo que, para leer desde teclado deberemos indicar `stdin`.
              
	Un ejemplo de uso sería:
			
	   char* cadena;
       size_t tamano;
       size_t caracteres_leidos;
       caracteres_leidos = getline(&cadena, &tamano, stdin);
    
	**NOTA:** El puntero `cadena` se ha pasado mediante su dirección de memoria intencionalmente, dado que la función necesita un puntero a un array de char dinámico (que se accede mediante otro puntero).
		
#### Alternativas para C++
		 
1. `std::cin.getline()`: En este caso podemos usar el método getline de la clase std::cin de la STL. Su sintaxis y funcionamiento es muy parecido al de fgets(), con el mismo efecto.
		  
	Su sintaxis es:
			
	   std::cin.getline(char array_char[], int num_caracteres_leer,  char _terminador_);
			    
			 
	 El caracter terminador es opcional, y sirve para indicar un final de cadena personalizado. Por ejemplo, podemos utilizar `‘\r\n’` si estamos en Windows.
			 
	 Un ejemplo de uso sería:
			 
	   char cadena[MAX];
       std::cin.getline(cadena, MAX);
            
 2. `std::getline()`: Esta función es similar a la anterior, pero mas genérica. En este caso, la cadena no se guarda en arrays de char sino en objetos de `std::string`. 
         
    La sintaxis es similar a la anterior, pero usando `std::istream` como entrada y std::string como contenedor de la cadena. 
            
         std::getline(std::istream& stream_entrada, std::string& cadena_salida, char terminador)
	             
     El terminador es opcional, igual que en `std::cin.getline()`
             
     **NOTA:** A diferencia de la `getline()` de C, en este caso los pasos se hacen por referencia y no por puntero, por lo que no hay que aplicar & al pasar los argumentos.
             
     De nuevo, esta función está pensada para ficheros. En este caso, para indicar que se lea de teclado, se indicará como primer argumento `std::cin`
             
       Un ejemplo de uso sería:
              
	    std::string cadena;
        std::getline(std::cin, cadena);


### Mala práctica 2: `fflush()`

En este caso, el problema viene de que esta función solo libera el buffer de salida, no el de entrada. La liberación del buffer de entrada solo está definida para Windows, siendo comportamiento indefinido (undefined behaviour en inglés) en otras plataformas como GNU/Linux

#### Alternativas para C:

En este caso no hay solución directa, simplemente “recetas” para leer los caracteres del buffer hasta vaciarlo.  

+ **Usando `getchar()`:**  La receta mas genérica y compatible es crear un bucle que llame a `getchar()` hasta encontrar un final de línea. Esta funciona en la mayoría de sistemas, a cambio de necesitar la librería `string.h`  
	
	La receta a seguir es:
		
      char ch;
      while( (ch=getchar()) != EOF && ch != '\n' );
          
    Esta receta compara con tanto con el carácter ‘\n’ como con la constante EOF de final de línea, para asegurar máxima compatibilidad.
        
    Una versión mas simple, comparando únicamente con `‘\n’` es:
        
   	  while(getchar() != ‘\n’);
       	 
   	Cualquiera de estas opciones se puede englobar en una macro o función para hacerlo mas legible y cómodo de usar
       	
      void flushin(){
          char ch;
          while( (ch=getchar()) != EOF && ch != '\n' );
      }

	El uso sería el mismo que el de `fflush()`
		
      char cadena[MAX];
      fgets(cadena, MAX, stdin);
      flushin();
         
     **NOTA:** En esta receta, si en el momento de ejecutarse la función el buffer esta vacío, tendremos que pulsar Enter para continuar el programa
         
     
 #### Alternativas para C++
 	
- `std::cin.ignore()`: El método ignore de la clase std::cin nos permite ignorar un flujo de caracteres y descartarlos del buffer de teclado.
 	
	Su sintaxis es:
 		
	  std::cin.ignore(size_t cantidad_caracteres_descartar, char _terminador_);
 			
	La cantidad de caracteres a descartar será la máxima cantidad que descarte la función. Si encuentra el terminador antes de llegar a esa cantidad, no descartará nada mas. 
 		
	Ambos argumentos son opcionales. Si no se indica nada, simplemente descartará el primer carácter existente en el buffer. Esto sería con:
 		
	  std::cin.ignore();
 		  
	Pero si queremos descartar todos los caracteres, debemos usar algo mas sofisticado. Para ello, indicaremos como cantidad de caracteres a descartar la máxima cantidad permitida por los streams, y le indicaremos como terminador el salto de línea.
 		
	  std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
 		
	Para obtener los numeric_limits necesitamos incluir la librería limits
 		
	  #include <limits>
 		 
 ### Mala práctica 3: Uso de la librería conio.h
 
 La librería conio.h fue creada por Borland para manipular la entrada y salida de la terminal de MSDOS. 
 
 El problema de esta librería es que, aparte de ser antigua y estar plagada de vulnerabilidades, no es estándar. Incluso en Windows, solo funciona con la terminal de MSDOS: si instalaras otra aplicación de terminal, ya no funcionaría.
 
 Por tanto, su uso está prohibido en la mayoría de proyectos de C y C++ en entornos de desarrollo profesional.
 
 #### Alternativas
 
En este caso, no hay alternativa completa, pero existen algunas opciones para utilizar sus funciones mas conocidas:
 
- **NCurses:** Muchas de sus funciones como `getch()`, están implementadas en la librería ncurses, compatible con múltiples sistemas operativos como Windows y GNU/Linux
	
- **Funciones propias de los sistemas operativos:** Jugando con las librerías estándar de cada sistema operativo, se pueden replicar algunas de las funciones mas conocidas. Por ejemplo, en este enlace comparto una implementación propia de `getch()` para Windows y GNU/Linux (bastante mejorable, pero funcional)
          
	https://gist.github.com/AlmuHS/4dc6a74b7e7d409ac75138c3e857c252 
		
### Mala práctica 4: Uso de `strcmp()`, `strcpy()` y similares

Las funciones strcmp() y strcpy() no comprueban el tamaño de las cadenas ni el final de cadena de forma fiable. Todas estas funciones se basan en el caracter `\0` para encontrar el final de la cadena, de forma similar a esta:

	char array[N];
	char* c = array;
	while (c != '\0') {
	  ...
	  c = c + 1;
	}

Pero, si la cadena no tiene el caracter `\0`, estas funciones seguirán recorriendo la memoria aún después de finalizar el array de char, hasta encontrar un `\0` de otra cadena, o hasta salirse de la zona de memoria permitida para el programa (lo cual provocaría un `segfault` y colgaría el programa) 

Además, hay otros problemas:

- `strcpy()` no comprueba que la cadena origen quepa en el array destino. Esto provoca que, si la cadena origen es mayor que el array destino, esta se escriba fuera de rango (igual que pasaba con `gets()` ), pudiendo sobreescribir datos de otras variables o provocar el fallo del programa 

- `sprintf()` presenta el mismo problema que strcpy, de manera que si la cadena no cabe en el destino, se escribirá fuera de rango.

#### Alternativas en C

1. `strncpy()`, `strncmp()` y `snprintf()`

	Una opción es usar es la versión “n” de cada función: strncpy(), strncmp(), snprintf(), en las cuales se indica el tamaño máximo de caracteres a copiar o comparar.
	
	- En el caso de `strncpy()` y `snprintf()`, si la cadena supera el tamaño solo se copiarán la cantidad de caracteres indicada por parámetro. 
	
	  Una ventaja de `snprintf()` respecto a `strncpy()` es que, en caso de que la cadena sea mas larga que el número de caracteres indicado, `snprintf()` añade el `\0` tras cortar la cadena, a diferencia de `strncpy()` que simplemente corta la cadena.
	
	- En el caso de `strncmp()`, en caso de que la cadena no tenga `\0`, solo se comparará la cantidad de caracteres indicada por parámetro.
	
	La sintaxis de estas funciones es la siguiente:
	      
		strncpy(char destino[], char origen[], int num_caracteres_copiar);
		strncmp(char cadena1[], char cadena2[], int num_caracteres);
		snprintf(char destino[], int num_caracteres, char* formato, argumentos …)
	
	Un ejemplo de uso es:
	
	    #include <cstdio>
	    #include <cstring>
	      
		int main(){
		    char cadena1[5];
		    char cadena2[6]; 
		
		    strncpy(cadena1, “abcde”, 4);//solo se copiará “abcd”
		    strncpy(cadena2, cadena1, 6);
		      
		    if(strncmp(cadena1, cadena2, 4) == 0) 
	            printf(”Son iguales\n”);
		    else{
	            snprintf(cadena2, 5, “%s”, cadena1); //copia "abcd\0"
		    }
		      
		    return 0;
		}
		

2. **Alternativa para copiar cadenas: `strncat()`**

	El uso de `strncpy()` presenta un problema: si la cadena a origen tiene una cantidad de caracteres mayor a la indicada por parámetro, copiará la cadena recortada al destino sin añadir el `\0`, lo cual provocará problemas mas adelante.
	
	Una solución a esto es usar la función `strncat()` para concatenación de cadenas, que añade el `\0` al final de la cadena. Esta función necesita una cadena inicial, por lo que habrá que prepararla previamente.
	
	Un ejemplo de uso sería el siguiente:
	
		char cadena[] = "abcde";
		char cadena2[4];
		cadena2[0] = 0; 
		strncat(cadena2, cadena1, 4);
	
#### Alternativas en C++
	
1. `std::string_view`

	Esta clase, disponible desde C++17, permite realizar operaciones de lectura y consulta sobre cadenas de caracteres, tanto std::string como arrays de char.
	
	**WIP**
	

## Namespaces (C++)

### Importación de namespaces global

Una mala práctica habitual en C++ es añadir al inicio del fichero de código la sentencia

	using namespace std; 

Esta sentencia importa todo el espacio de nombres de las clases y funciones de la STL, de forma que puedas invocarlas sin tener que incluir el prefijo std:: 

Pero esto es problemático: al importar todo el espacio de nombres de forma global, se produce un riesgo de colisión de nombres. Esto quiere decir que, si en tu programa hay alguna función o clase que tenga el mismo nombre que la de la STL el compilador no sabrá cual de ellas es la que se quiere usar. 

Podemos poner como ejemplo las funciones std::getline() y getline(), de C++ y C respectivamente. Si tienes incluidas las librerias `<sstream>` y `<cstring>` en el mismo fichero y ponemos una llamada a `getline()` ¿a qué función estamos llamando?

Pasaría lo mismo si quisieras crearte una clase llamada vector: cuando creemos el objeto de la clase ¿de qué clase lo estamos creando: vector o `std::vector`?

#### Solución 1: Uso del prefijo std::

La primera solución y mas sencilla es usar el prefijo std:: delante de cada clase o función de la STL. 

De esta manera, no haría falta importar el namespace, además de hacer mas fácil la identificación de las funciones que provienen de la STL, de las propias o provenientes de otra librería.

Un ejemplo de uso sería

	std::cout<<”Hola mundo”<<std::endl;

#### Solución 2: Importación local del namespace

Otra alternativa es importarla, pero de forma local, en el ámbito mas pequeño posible donde lo queramos usar. 
          
	void cppflush(){
		using namespace std;	
		cin.ignore(numeric_limits<streamsize>::max(),'\n')
	}

#### Solución 3: Importación local de la librería o clase

La última opción es usar la instrucción using de forma local pero, en vez de importar el namespace completo, importar únicamente la librería o clase a utilizar. 

Siguiendo el ejemplo anterior, sería

	void cppflush(){
		using std::cin;
		using std::numeric_limits;
		using std::streamsize;
		cin.ignore(numeric_limits<streamsize>::max(),'\n')
	}

## Ficheros

### Escritura en ficheros binarios

#### Mala práctica 1: Escritura de struct en ficheros sin serializar

Una mala práctica habitual en C y C++ consiste en escribir un struct en un fichero en una sola operación, tal que
      
	struct datos{
       int dato1;
       float dato2;
       char cadena[20];
	 };
	
	…
	
	datos datos_obj;
	datos_obj.dato1 = 1;
    datos_obj.dato2 = 3.6;
    strncpy(datos_obj.cadena, “cadena”, 20);
	      
	ofstream fichero(“fichero.txt”, std::ios::binary)
	fichero.write(fichero, (char*) &datos_obj, sizeof(datos));

Este tipo de escritura presenta varios problemas, que pueden hacer que el fichero escrito en un sistema operativo no sea legible en otro sistema.
Esto se debe a lo siguiente:

- Los sistemas operativos, al almacenar un struct, no almacenan sus campos de forma completamente contigua en memoria (la siguiente dirección después de dato1 no tiene porqué ser la dirección inicial de dato2) , sino que los alinea añadiendo una memoria adicional llamada *padding*. Esta memoria se añade entre los campos para que los tamaños coincidan con los de los bloques de memoria. 
          
	El problema es que el tamaño del padding no está definido en el estándar, sino que es dependiente del compilador y del sistema operativo. 
          
	Esto implica que, al escribir el struct al fichero desde un sistema dado, este puede tener un tamaño de *padding* distinto al de otro sistema operativo que intente leer ese fichero, lo cual provocaría que el struct no se pudiera leer correctamente. 
	
- Otro problema, aunque este es mas evitable, es que el tamaño de los tipos de datos enteros (int, long…) es dependiente de la arquitectura y del sistema operativo, lo cual empeoraría el error anterior.

	Aunque este se puede evitar usando los tipos de la librería `stdint.h`, que tienen tamaño fijo. Estos son `int32_t`, `int16_t`, `uint32_t` … etc 

- El último problema es el *endianess*. Esto consiste en que los datos se representan en la memoria del computador en mediante cadenas de bits en algún tipo de codificación binaria. Según la arquitectura, la representación irá del bit mas significativo al de menos, o viceversa.

   La solución a este problema es bastante mas compleja, requiriendo escribir byte a byte y leyendo de forma diferente según el *endianess* correspondiente a cada arquitectura. **Dada la complejidad de esta solución, ignoraremos este problema, asumiendo *little endian* a la hora de escribir los datos**.


Las soluciones existentes para resolver este problema son:

1. **Eliminación del padding y uso de tipos stdint.h**

	Esta solución es la mas básica. Es fácil de hacer, pero no es la forma mas ideal de proceder, dado que **no es estándar y solo funciona en algunos compiladores y sistemas.**
	          
	Consiste en “empaquetar” el struct eliminando el *padding*, de forma que los campos queden completamente contiguos unos a otros independientemente del sistema operativo. 
	          
	Esto evita el problema de que el *padding* sea distinto en escritura y lectura, aunque no deja de ser una argucia para evitar crear un serializador.
	
	Para aplicarlo, justo antes de la declaración del struct, hay que aplicar la directiva 
	          
		#pragma pack(push, 1)
	          
	para desactivar el padding. Y, después de la declaración del struct, para que se siga aplicando el padding a otras estructuras, se vuelve a activar con
	          
		#pragma pack(pop)
		
	**Esta directiva no está disponible en todos los compiladores, por lo que habrá que revisar la documentación de nuestro compilador para comprobar si está soportada por el mismo.**
	
	A su vez, para evitar que el tamaño de los tipos enteros cambien según el sistema,  se puede utilizar la librería stdint.h , que define los tipos a partir de su tamaño y signo: `int32_t`, `uint32_t`, `int16_t`, `uint16_t` … 
	
   Un ejemplo de uso en C++, partiendo del ejemplo anterior, sería

		#include <cstdint>
		
		#pragma pack(push, 1)
		struct datos{
	          int32_t dato1;
	          float dato2;
	          char cadena[20];
         }
         #pragma pack(pop)
		
		…
		
		 datos datos_obj;
		 datos_obj.dato1 = 1;
		 datos_obj.dato2 = 3.6;
		 strcpy(datos_obj.cadena, “cadena”);
		      
		 std::ofstream fichero(“fichero.txt”, std::ios::binary)
		 fichero.write(fichero, (char*) &datos_obj, sizeof(datos));

2. **Creación de un serializador**

	Esta solución es mas profesional. Consiste en crear una función que serialice la escritura, escribiendo el struct campo a campo con el formato adecuado. Para leer se debe crear otra función análoga, que lea campo a campo en el mismo orden y formato que se ha escrito en el fichero. 
	          
	Esto nos permite asegurar que los datos se lean en el orden y formato adecuados, independientemente del sistema operativo, ya que, al escribir los datos por separado, no hay padding.

    Un ejemplo en C++, partiendo del ejemplo anterior, sería:
          
          struct datos{
             int32_t dato1;
             float dato2;
             uint8_t cadena[20];
          }
		… 
          
          
       void serializador_escritura_struct(std::ofstream fichero, const datos &datos_obj){
               fichero << dato1 << dato2 << strlen(cadena) << cadena << ‘\n’ ;
        }
          
	   void deserializador_lectura_struct(std::ifstream fichero, datos &datos_obj){
              uint16_t tam_cadena;
		
              fichero >> datos_obj.dato1;
              fichero >> datos_obj.dato2;
              fichero >> tam_cadena;
              std::cin.getline(cadena, tam_cadena, ‘\n’);
		              
		}
		          
		… 
		
		std::ofstream fichero_out(“fichero.txt”, std::ios::binary);
		struct datos datos_obj{ 1, 2.5, “cadena”};
		serializador_escritura_struct(fichero, datos_obj);
		          
		… 
		          
		std::ifstream fichero_in(“fichero.txt”, std::ios::binary);
        struct datos datos_in;
        deserializador_lectura_struct(fichero_in, datos_in);

3. **Serializador + sobrecarga de operadores (solo C++)**

      Esta solución es idéntica a la anterior, pero usando la sobrecarga de los operadores >> y << para poder escribir y leer del fichero con esos operadores, sin tener que llamar a una función de forma explícita. 
          
		std::ostream& operator << (std::ostream& fichero_out, const datos datos_obj) {
	           return fichero_out << dato1 << dato2 << strlen(cadena) << cadena << ‘\n’ ;
	    }
	
	    std::istream& operator >> (std::istream& fichero_in, datos& datos_obj)
        {
          uint16_t tam_cadena;

          fichero >> datos_obj.dato1;
          fichero >> datos_obj.dato2;
          fichero >> tam_cadena;
          std::cin.getline(cadena, tam_cadena, ‘\n’);
         
          return fichero;
        }
	
	              … 
        std::ofstream fichero_out(“fichero.txt”, std::ios::binary);
        struct datos datos_obj{ 1, 2.5, “cadena”};
	    fichero_out << datos_obj;
	          
		… 
	          
       std::ifstream fichero_in(“fichero.txt”, std::ios::binary);      
       struct datos datos_out;
       
       fichero_in >> datos_out;
 
 4. **Escritura en texto plano con separadores**
 
      Esta solución es muy versátil, aunque puede tener cierta dificultad al implementarlo. Consiste en escribir los datos en texto plano, en lugar de en forma binaria. 
      
      Para ello, es necesario transformar todos los datos a cadenas de caracteres separadas mediante un caracter separador, y luego leerlos de la misma forma, transformándolos a sus tipos originales. Hay que tener en cuenta que si alguno de los datos es una cadena de caracteres, esta podrá incluir espacios, por lo que no sería recomendable usar el espacio como separador.
        
      
      Si estamos en C++, el operador << de std::ofstream ya realizará la conversión automáticamente a cadena de caracteres. En C habrá que realizar el proceso manualmente.
      
      
      Una implementación de esta solución, en C++, sería esta. Aprovechamos las propiedades de std::string, una clase para cadenas de tamaño dinámico, para poder recoger las cadenas de caracteres sin necesidad de conocer su tamaño previamente. 
      
      Como separador de campos, se ha seleccionado el caracter '|', que es poco utilizado y con poco riesgo de confusión. Para leer cada campo en una sola operación, hemos utilizado `std::getline()`, que devuelve `std::string` y permite indicar un delimitador personalizado. 
      
      Finalmente, para convertir los datos a sus tipos originales, hemos utilizado `std::stringstream`, cargando los datos a un stream auxiliar y luego extrayendolo a sus campos originales. Otra opción hubiera sido `std::from_chars()` de C++17, que es mas preciso en algunos casos.
      
		#include <cstdio>
		#include <iostream>
		
		#include <fstream>
		#include <sstream>
		#include <iomanip>
		
		#include <cstring>
		#include <string>
		#include <cstdint>
		
		struct datos{
		       int32_t dato1;
		       float dato2;
		       char cadena[20];
		};
		          
		          
		void serializador_escritura_struct(std::ofstream& fichero_texto, const datos &datos_obj){  
		       fichero_texto << datos_obj.dato1 << "|" << datos_obj.dato2 << "|"  << datos_obj.cadena << '\n' ;
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
		
		       datos_obj = convertir_tipos_stringstream(datos_str);
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
## Consola

### Mala práctica 1: Uso de printf sin especificador de formato (C)

La función `printf()` admite mostrar cadenas por pantalla sin necesidad de indicar el formato `"%s"`, tal que

	printf("cadena");

Pero esto, si se combina con cadenas recibidas desde teclado, puede ser problemático. Esto se daría en esta situación.

	#include <stdio.h>

	int main(){
	  char cadena[20];
	  printf("Introduce cadena: ");
	  fgets(cadena, 20, stdin);
	  
	  printf(cadena);
	}

El problema sucede si el usuario introduce un especificador de formato como parte de la cadena introducida por teclado. Por ejemplo, si la cadena introducida fuera `"%i hola"` el `printf()` quedaría así:

	printf("%i hola");

Lo cual haría que el printf() esperara un número entero que no se ha aportado y mostrara un valor incorrecto.


	Introduce cadena: Hola %i
	Hola 543255663


#### Solución

La solución consiste en añadir el especificador de formato para cadenas, `"%s"`, al `printf()`

	printf("%s", cadena);

Esto, aplicado al ejemplo anterior, mostraría

	Introduce cadena: Hola %i
	Hola %i

### Mala práctica 2: Uso de system()

La función `system()` permite ejecutar comandos en la terminal del programa, indicando el comando mediante una cadena de caracteres. 

Esto puede resultar muy cómodo, pero entraña varios problemas:

- Los comandos son dependientes del entorno de ejecucion y del sistema operativo, por lo que los programas no serían compatibles con otros sistemas

- La orden `system()` es vulnerable a ataques de reemplazo del ejecutable. Si un atacante altera el ejecutable, podría reemplazar el comando invocado por otro con efectos malintencionados.

#### Solución

La solución más simple consiste en replicar el efecto de esos comandos mediante funciones de la librería estándar o de librerías externas.

En este caso, explicaré alternativas para `system("pause")` y `system("cls")`

##### Alternativas a `system("pause")`

`system("pause")` es una llamada que ejecuta el comando `pause` de la consola de Windows. Esto hace que la consola se espere a la pulsación de una tecla antes de continuar la ejecución del programa, lo cual se suele usar para evitar que la consola se cierre al finalizar el programa.

El comando `pause` solo existe en Windows, por lo que la llamada no será portable a otras plataformas, además de los problemas de seguridad comentados anteriormente.

He de aclarar que el comportamiento de cerrar la consola al finalizar el programa es propio de la consola de Windows, siendo poco frecuente en otros sistemas y entornos de ejecución. Aún así, las soluciones que propondremos serán portables entre múltiples sistemas.

En este caso, la solución es tan sencilla como llamar a cualquier función que espere la pulsación de Enter para continuar. Esto podría hacerse con `scanf()` en C y `std::cin` en C++, pero dado que no necesitamos guardar la cadena recibida, podemos usar funciones mas simples, que no requieren destino para ejecutarse.

Esto es, en C

	getchar()

En C++
	
	std::cin.get()

Estas dos funciones recogen un único caracter y esperan a pulsar Enter para continuar, con lo cual son soluciones sencillas para evitar este problema.

##### Alternativas a `system("cls")`

`system("cls")` es una llamada que provoca el borrado de la salida de consola. 

El borrado de la consola sí es algo necesario en todos los sistemas. Pero, el comando `cls` solo existe en Windows. En Linux, el comando equivalente sería `clear`. 

En este caso, la solución es algo mas compleja: no existe ninguna función de la librería estándar que realice esta funcionalidad, por lo que tendremos que usar soluciones dependientes de entorno de ejecución (tanto sistema operativo como emulador de terminal) o recurrir a librerías externas.

En caso de usar soluciones dependientes del sistema, habrá que aplicar directivas del procesador para que compile una solución diferente según el sistema desde el que se compile.

**NOTA: Aunque `<conio.h>` dispone de funciones para esta tarea, ya hemos avisado de que su uso está fuertemente desaconsejado, por lo que no lo incluiremos como opción**

- **Solución 1: Secuencias de escape de la consola**

	Las consolas suelen disponer de una caracteristica llamada "secuencias de escape", que permiten realizar acciones de control de la consola mediante la escritura de una cadena de caracteres a la salida estándar.
	
	La secuencia de escape depende de la consola y del sistema operativo, aunque existen ciertos estándares seguidos por la mayoría de consolas, como el ANSI que usaremos en este caso.
	
	Una secuencia de escape ANSI para borrar la consola es `"\033c"`. Escribiendo esto dentro de un printf o cout podremos borrar la consola sin necesidad de llamadas adicionales.
	
		printf("\033c");	//Solución para C
		std::cout<<"\033c";   //Solución para C++
	
	En Linux esta secuencia de escape está ampliamente soportada por la mayoría de consolas y emuladores de terminal, pero en Windows se requiere que el emulador de terminal tenga activado el flag `ENABLE_VIRTUAL_TERMINAL_PROCESSING` para soportar este tipo de secuencias.
	
	Este flag está activado por defecto en *Windows Terminal*, la terminal por defecto de Windows 11 y en las últimas actualizaciones de Windows 10 de 2023, pero en la terminal *cmd* de Windows 10 y anteriores se requiere activarlo manualmente. 
		
	Desde C y C++, se puede activar usando `<windows.h>`, con el siguiente snippet de código.
		
		
	    #define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
	    
	    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE), hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	    DWORD dwMode;
	    GetConsoleMode(hOutput, &dwMode);
	    dwMode |= ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	    if (!SetConsoleMode(hOutput, dwMode)) {
	        printf("SetConsoleMode failed."); //Sustituir por std::cout en C++
	    }

	Después de esto, se pondría la secuencia de escape de igual forma al ejemplo anterior.
	
	Siguiendo esta idea, y añadiendo las directivas de preprocesador oportunas para detectar el sistema operativo desde el que se está compilando, el código en C quedaría así:
	
		#include <stdio.h> 
		
		#ifdef _WIN32 || _WIN64 //Si estamos en Windows, activa las secuencias de escape desde WinAPI
			#include <windows.h>
			
			#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
			
			int enableANSI(){
			    HANDLE hStdout;
			    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
			
			    DWORD mode = 0;
			    if (!GetConsoleMode(hStdout, &mode))
			    {
			        return ::GetLastError();
			    }
			
			    const DWORD originalMode = mode;
			    mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
			    // Try to set the mode.
			    if (!SetConsoleMode(hStdout, mode))
			    {
			        return ::GetLastError();
			    }
			
			    return 0;
			}
			
			void clearscr(){
			    enableANSI();
			    printf ("\033c");
			    
			}
		#elif __linux__ || __unix__ //si estamos en Linux u otro sistema UNIX, aplica la secuencia directamente
		
			void clearscr(){
			    printf("\033c");
			}
		#endif
		
		int main(void){
			clearscr();
		}
			 

- **Solución 2: Uso de APIs propias de cada sistema operativo**

	Cada sistema operativo dispone de APIs o librerías propias para controlar la consola desde lenguaje C. Haciendo uso de las mismas, podremos limpiar la pantalla sin depender de la compatibilidad con las secuencias de escape.
	
	Como incoveniente, en algunos casos dependeremos de librerías externas.
	
	- **En Windows, deberemos recurrir a WinAPI**, que dispone de funciones para controlar la consola. Existen múltiples formas de hacerlo, según queramos borrar la consola llenando todo su contenido con espacios y desplazando, o queramos realizar el borrado del contenido manualmente.
	
		En nuestro caso, utilizaremos la primera opción. Esto se hace con la siguiente función
		
			#include <windows.h>
			
			void cls()
			{
				HANDLE hConsole;
    				hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			
			    CONSOLE_SCREEN_BUFFER_INFO csbi;
			    SMALL_RECT scrollRect;
			    COORD scrollTarget;
			    CHAR_INFO fill;
			
			    // Get the number of character cells in the current buffer.
			    if (GetConsoleScreenBufferInfo(hConsole, &csbi))
			    {
				    // Scroll the rectangle of the entire buffer.
				    scrollRect.Left = 0;
				    scrollRect.Top = 0;
				    scrollRect.Right = csbi.dwSize.X;
				    scrollRect.Bottom = csbi.dwSize.Y;
				
				    // Scroll it upwards off the top of the buffer with a magnitude of the entire height.
				    scrollTarget.X = 0;
				    scrollTarget.Y = (SHORT)(0 - csbi.dwSize.Y);
				
				    // Fill with empty spaces with the buffer's default text attribute.
				    fill.Char.UnicodeChar = TEXT(' ');
				    fill.Attributes = csbi.wAttributes;
				
				    // Do the scroll
				    ScrollConsoleScreenBuffer(hConsole, &scrollRect, NULL, scrollTarget, &fill);
				
				    // Move the cursor to the top left corner too.
				    csbi.dwCursorPosition.X = 0;
				    csbi.dwCursorPosition.Y = 0;
				
				    SetConsoleCursorPosition(hConsole, csbi.dwCursorPosition);
			   }
			}		

	- **En Linux, no hay una forma estándar de controlar la consola a bajo nivel, sino que el control de la consola está estandarizado bajo la librería ncurses.**
	 
	  En este caso, recurriremos a las secuencias de escape. Pero, para asegurar compatibilidad entre sistemas y terminales, usaremos *terminfo* para obtener la secuencia de escape correspondiente a nuestra consola. 
	  
	  Esta librería consultará en la base de datos de ncurses la secuencia correspondiente a la orden de control de la consola que nos interese, y devolverá la cadena asociada a la misma. Una vez obtenida la cadena de la secuencia de escape, la enviaremos a la salida estándar para aplicarla.
	  
	  En nuestro caso, la orden a consultar es "clear", que limpia la consola desplazando la salida y colocando el cursor al inicio.  
	  Pero "clear" no borra el scroll, así que aún podríamos ver el contenido anterior desplazando hacia arriba. Para solucionarlo, usaremos otra orden llamada "E3". Dado que esta orden es una extensión del estándar, deberemos comprobar que está disponible antes de aplicarla.
	  
	  La función resultante (en C) quedará así:
	  
			#include <stdio.h>
			#include <unistd.h>
			#include <term.h>
			
			void ClearScreen()
			{
			  if (!cur_term)
			  {
			    int result;
			    setupterm( NULL, STDOUT_FILENO, &result );
			    
			    if (result > 0){
			      printf( tigetstr( "clear" ) ); //clean screen
			      
			      char* e3 = tigetstr("E3"); //Remove scroll
			      if(e3 != NULL)
			        printf( tigetstr( "E3" ) );
			        
			    }
			  }
			}
	
	Esta función depende de la librería `terminfo`. Esta hay que añadirla al linkador al compilar. 
	
		gcc [fichero_fuente].c -lterminfo -o [nombre_ejecutable]

	En algunas distribuciones, `terminfo` pertenece a la librería `ncurses`. En este caso, habría que compilar con esta otra línea
	
		gcc [fichero_fuente].c -lncurses -o [nombre_ejecutable]

## Punteros

### Mala práctica 1: uso de `NULL` en lugar de `nullptr` (c++)

WIP

### Mala práctica 2: comprobación de éxito del operador `new` mediante comparación con `NULL` (c++)


## Buenas prácticas

### Uso de stdint.h para tipos enteros

### Uso de stdbool.h para tipos booleanos (C)

### Uso de `using as` en lugar de `typedef` en C++


## Referencias

- https://cplusplus.com/articles/4z18T05o
- https://learn.microsoft.com/es-es/windows/console/clearing-the-screen
- https://stackoverflow.com/questions/32109646/how-to-write-a-struct-to-file-in-c-and-read-the-file
- https://parzibyte.me/blog/2022/01/01/guardar-recuperar-struct-archivo-cpp/
- https://stackoverflow.com/questions/21344106/serializing-struct-to-file-and-deserializing-it-again-with-string
- https://en.cppreference.com/w/cpp/string/basic_string_view
- 