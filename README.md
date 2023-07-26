# Malas prácticas en C y C++ y su solución

## Introducción

En este documento expondré algunas malas prácticas habituales en C y C++ con fácil solución, explicando las causas de ser consideradas así, y algunas propuestas para evitarlas

## Cadenas de caracteres

### Mala práctica 1: `gets()`: 

Esta función es una mala práctica, prohibida en cualquier entorno de desarrollo profesional. Esto se debe a que gets() no comprueba que la cadena recibida por teclado quepa en el array de char usado para almacenarlo.
   
   Esto hace que, una vez superado el tamaño del array de char, gets() siga escribiendo, lo cual puede producir que algunas variables se sobreescriban o incluso que el programa se cuelgue por completo.
      
   Para evitarlo, existen distintas alternativas, según estemos en C o en C++
   
#### Alternativas para C++
    
1. `fgets()`: El reemplazo mas sencillo es la función fgets(), que permite indicar un tamaño máximo de lectura. Esta función controla que la cadena recibida no sobrepase el tamaño indicado, cortando los caracteres restantes. 

	La sintaxis de la función es:
			
	   fgets(char array_char[], int num_caracteres_leer, FILE buffer_entrada);
			   
   Esta función está pensada para ficheros, por lo que para indicar que lea de teclado debemos indicar en el buffer de entrada el nombre stdin, correspondiente al buffer de entrada estándar de teclado. 
		  
   Un ejemplo de uso sería:
		   
       char cadena[MAX];
       fgets(cadena, MAX, stdin);
       
               
2. `getline()`: Esta función es algo mas sofisticada, reservando dinámicamente el espacio necesario para almacenar la cadena. Por esta razón, no se puede usar con array de char estáticos, sino que debe recibir un puntero a un array de char.  
              
	Para saber el tamaño de la cadena recibida, la función recibe un parámetro adicional por referencia que se actualizará con el tamaño recibido. 
              
	Su sintaxis es:
			
		size_t getline(char** puntero_cadena, size_t *tamano_recibido, FILE buffer_entrada);
			  
		  
   La función devuelve el número de caracteres leídos. 
              
	De nuevo, esta función está pensada para ficheros, por lo que, para leer desde teclado deberemos indicar `stdin`.
              
	Un ejemplo de uso sería:
			
	   char* cadena;
       size_t tamano;
       size_t caracteres_leidos;
       caracteres_leidos = getline(&cadena, &tamano, stdin);
    
	**NOTA:** El puntero `cadena` se ha pasado como referencia intencionalmente, dado que la función necesita un puntero a un array de char dinámico (que se accede mediante un puntero).
		
#### Alternativas para C++
		 
1. `std::cin.getline()`: En este caso podemos usar el método getline de la clase std::cin de la STL. Este es `cin.getline()`. Su sintaxis y funcionamiento es muy parecido al de fgets(), con el mismo efecto.
		  
	Su sintaxis es:
			
	   std::cin.getline(char array_char[], int num_caracteres_leer,  char _terminador_);
			    
			 
	 El caracter terminador es opcional, y sirve para indicar un final de cadena personalizado. Por ejemplo, podemos utilizar `‘\r\n’` si estamos en Windows.
			 
	 Un ejemplo de uso sería:
			 
	   char cadena[MAX];
       std::cin.getline(cadena, MAX);
            
 2. `std::getline()`: Esta función es similar a la anterior, pero mas genérica. En este caso, la cadena no se guarda en arrays de char sino en objetos de `std::string`. 
         
    La sintaxis es similar a la anterior, pero usando std::istream como entrada y std::string como contenedor de la cadena. 
            
         std::getline(std::istream& stream_entrada, std::string& cadena_salida, char terminador)
	             
     El terminador es opcional, igual que en `cin.getline()`
             
     **NOTA:** A diferencia de la getline() de C, en este caso los pasos se hacen por referencia y no por puntero, por lo que no hay que aplicar & al pasar los argumentos.
             
     De nuevo, esta función está pensada para ficheros. En este caso, para indicar que se lea de teclado, se indicará como primer argumento `std::cin`
             
       Un ejemplo de uso sería:
              
	    std::string cadena;
        std::getline(std::cin, cadena);


### Mala práctica 2: `fflush()`

En este caso, el problema viene de que esta función solo está definida para Windows, siendo comportamiento indefinido (undefined behaviour en inglés) en otras plataformas. En sistemas como GNU/Linux, esta función no tiene efecto alguno.

#### Alternativas para C:

En este caso no hay solución directa, simplemente “recetas” para leer los caracteres del buffer hasta vaciarlo.  

+ **Usando `getchar()`:**  La receta mas genérica y compatible es crear un bucle que llame a `getchar()` hasta encontrar un final de línea. Esta funciona en la mayoría de sistemas, a cambio de necesitar la librería string.h  
	
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
	
- **Funciones propias de los sistemas operativos:** Jugando con las librerías estándar de cada sistema operativo, se pueden replicar algunas de las funciones mas conocidas. Por ejemplo, en este enlace comparto una implementación propia de getch() para Windows y GNU/Linux (bastante mejorable, pero funcional)
          
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

- `strcpy()` no comprueba que la cadena origen quepa en el array destino. Esto provoca que, si la cadena origen es mayor que el array destino, esta se escriba fuera de rango (igual que pasaba con el gets() ), pudiendo sobreescribir datos de otras variables o provocar el fallo del programa 

- `sprintf()` presenta el mismo problema que strcpy, de manera que si la cadena no cabe en el destino, se escribirá fuera de rango.

#### Alternativas en C

1. `strncpy()`, `strncmp()` y `snprintf()`

	Una opción es usar es la versión “n” de cada función: strncpy(), strncmp(), snprintf(), en las cuales se indica el tamaño máximo de caracteres a copiar o comparar.
	
	- En el caso de `strncpy()` y `snprintf()`, si la cadena supera el tamaño solo se copiarán la cantidad de caracteres indicada por parámetro. 
	
	  Una ventaja de `snprintf()` respecto a `strncpy()` es que, en caso de que la cadena sea mas larga que el número de caracteres indicado, `snprintf()` corta la cadena añadiendo `\0` al final, cosa que `strncpy()` no hace.
	
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
	
