# Malas prácticas en C y C++ y su solución

## Introducción

En este documento expondré algunas malas prácticas habituales en C y C++ con fácil solución, explicando las causas de ser consideradas así, y algunas propuestas para evitarlas

He de aclarar que este documento está pensado para poder ser explicado a estudiantes, por lo que he ignorado aquellas soluciones que tienen una complejidad excesiva, aún siendo utilizadas en ámbito profesional.

## Variables

### Mala práctica: declarar todas las variables arriba de la función

En versiones muy antiguas de C, era necesario declarar todas las variables arriba de cada función, en las primeras líneas de la misma.

Pero, desde C99 en C, además de en C++, esto ya no es necesario, siendo posible declarar las variables en cualquier parte del código, incluso dentro de ámbitos mas pequeños, como dentro de un condicional o un bucle.

Así pues, la declaración de las variables arriba de la función se considera mala práctica, debido a que puede ser una causa de vulnerabilidades y errores.

Por ejemplo, imaginemos que queremos mostrar la suma de cada fila de una matriz:

	int main(void){}
		int i,j;
		int matriz[N][M];
		int suma = 0;
		
		... 
		for(i = 0; i < N; i++){
		
			//Error: ha olvidado reinicializar "suma", 
			//por lo que la suma de las siguientes sumas partirán de la suma anterior
			for(int j = 0; j < M; i++){
				suma += matriz[i][j];
			}
			printf("La suma de la fila %d es %d\n", i, suma);
		}

Si el código es muy largo, es muy fácil despistarse y olvidar inicializar la variable `suma`

Otro ejemplo sería el olvido de la inicialización

	int main(void){
		int i;
		int vector[N];
		int valor;
		...
		
		while(i < N && vector[i] != valor) i++;
		
		...
		
		return 0;
	}

En este caso, si entre la declaración de la variable y el bucle hay muchas líneas, es posible que la declaración quede fuera de la visión en el momento de escribir el bucle, por lo que el programador podría confiar en que la variable se inicializó al declararla, y no se daría cuenta de que ha olvidado inicializarla.

Otro problema mas sería la reutilización accidental de variables.

#### Solución

La solución consiste en declarar las variables u objetos en el lugar mas cercano a donde se van a usar, y en el ámbito mas pequeño necesario. 

De esta forma, la declaración está visible cerca del bloque de código donde se va a usar, y es mas fácil comprobar las situaciones anteriores.

A su vez, el declararlo dentro del ámbito mas pequeño necesario evita colisiones de nombres y posibles confusiones.

**NOTA: Los compiladores actuales están pensados para optimizar este tipo de declaraciones de variables, por lo que el uso de muchas variables auxiliares no implica una reducción en el rendimiento**

Por ejemplo, en el primer ejemplo:

	int main(void){
		int i,j;
		int matriz[N][M];
		
		... 
		
		for(i = 0; i < N; i++){
		
			/* la variable se redeclara en cada iteración y no existe fuera de
			 * este for, por lo que el riesgo de ser reutilizada o
			 * reasignada es muy pequeño   */
			
			int suma = 0; //se ve claramente su valor inicial
			 
			for(int j = 0; j < M; i++){
				suma += matriz[i][j];
			}
			printf("La suma de la fila %d es %d\n", i, suma);
		}
	}

En este caso, al declararla dentro del for externo, la variable se redeclara en cada iteración, y deja de existir al terminar el bucle. Esto implica lo siguiente:

- No hay riesgo de que la variable sea reutilizada antes o despues del bucle for
- La inicialización se hace junto a la declaración. Y, al estar dentro del bucle, se ve con mayor claridad.
- La variable desaparece tras cada iteración del bucle externo, con lo cual queda asegurado la desaparición del valor anterior.


El segundo ejemplo quedaría así  
 
	int main(void){
		int vector[N];
		int valor;
		...
		
		int i = 0;
		while(i < N && vector[i] != valor) i++;
		
		...
		
		return 0;
	}

En este caso, la ventaja radica simplemente en el tener la declaración junto al inicio del bucle, quedando en el área de visión del programador al escribir ese bucle. Esto permite ver claramente el valor inicial de la variable, y cambiarlo rápidamente si es necesario.



## Funciones

### Mala práctica: Usar `()` en la declaración de funciones sin parámetros (C)

Una práctica común en C es declarar las funciones que no reciben parámetros como

	[tipo] funcion();

Sin embargo, en C (y en algunas versiones muy antiguas de C++), este tipo de declaración hace referencias a un número de parámetros indefinido, de forma que la función admitiría el paso de argumentos.

Un ejemplo sencillo sería este:

	#include <stdio.h>
	
	void mifuncion(){
		printf("Hola");
	}
	
	int main(){
		mifuncion(4,8);
	}

Este ejemplo, al compilarlo en C, compilaría sin problemas, incluso aplicando los últimos estándares.

#### Solución

Para que la función no admita ningún argumento, hay que declarar la función tal que

	[tipo] funcion(void);

Así, nuestro ejemplo quedaría de esta manera

	#include <stdio.h>
	
	void mifuncion(void){
		printf("Hola");
	}
	
	int main(){
		mifuncion(4,8);
	}

Y, al compilarlo, nos daría el error

	funcion_c.c: In function ‘main’:
	funcion_c.c:8:9: error: too many arguments to function ‘mifuncion’
	    8 |         mifuncion(4,8);
	      |         ^~~~~~~~~
	funcion_c.c:3:6: note: declared here
	    3 | void mifuncion(void){
	      |      ^~~~~~~~~

Asegurando así que no se permita ningún argumento en la llamada a la función


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
	
1. `std::string_view` (C++17)

	Esta clase, disponible desde C++17, permite realizar operaciones de lectura y consulta sobre cadenas de caracteres, tanto std::string como arrays de char.
	
	Es importante recalcar que esta clase abre las cadenas en modo de *solo-lectura*, por lo que las operaciones de copia se realizarán sobre otras cadenas, y no sobre la que se ha cargado en la clase.
	
	La creación del objeto sería algo tal que:
	
		std::string_view sv{cadena};
	
	Donde cadena puede ser tanto un array de char tipo C, un `std::string` u otro tipo de contenedores que almacenen cadenas de caracteres.
	
	- **Comparar dos cadenas**
	
		Para comparar la cadena cargada en el objeto con otra cadena externa, se puede usar el método `compare`. Este tiene la siguiente sintaxis
		
			int std::string_view.compare(cadena2);

		La función devuelve valores de forma similar a `strcmp()`
		
		- Devuelve 0 si son iguales
		- Devuelve un valor negativo si la cadena cargada es menor a `cadena2`
		- Devuelve un valor superior a 0 si la cadena cargada es mayor a `cadena2`	
		
		Un ejemplo de uso sería:
	
			#include <iostream>
			#include <string_view>
			
			int main(){
			  char cadena[5] = "hola";
			  char cadena2[5] = "hola";
			  char cadena3[5] = "Hola";
			  std::string_view strv_cadena{cadena};
			
			  std::cout << "¿Son las cadenas 1 y 2 iguales? " << ((strv_cadena.compare(cadena2) == 0) ? "si" : "no") << '\n';
			  std::cout << "¿Son las cadenas 1 y 3 iguales? " << ((strv_cadena.compare(cadena3) == 0) ? "yes" : "no") << '\n';
			
			  
			  return 0;
			
			}

		Esto mostraría por pantalla
		
			¿Son las cadenas 1 y 2 iguales? si
			¿Son las cadenas 1 y 3 iguales? no

	- **Copiar un `std::string_view` a otra cadena**
	
		También es posible copiar la cadena cargada en el string_view a otra cadena externa, usando el método `copy()`. Este tiene la sintaxis
		
			std::string_view.copy(char cadena[], int longitud, [int inicio]);
		
		Donde `cadena` debe ser un array de char, `longitud` es el número de caracteres a copiar, e `inicio` la posición desde la que empezamos a copiar. Si no se indica `inicio`, se tomará la primera posición de la cadena.
		
		**NOTA: `std::string_view` no añade el final de cadena `\0`, por lo que la cadena resultante podría mostrarse de forma incorrecta al pasarla a otras funciones**
		
		Un ejemplo de uso sencillo sería:
		
			#include <iostream>
			#include <string_view>
			
			int main(){
			  char cadena[5] = "hola";
			  char cadena2[5]; //cadena destino
			  std::string_view strv_cadena{cadena};
			
			  std::cout<<"El contenido del sv es "<<strv_cadena.data()<<"\n";
			  
			  strv_cadena.copy(cadena2, 4, 0);
			  cadena2[4]='\0'; //añadimos el final de cadena manualmente, para que cout muestre la cadena de forma correcta
			  std::cout<<"El contenido de la cadena 2 es "<<cadena2<<"\n";
			  
			  
			  return 0;
			
			}
	
	
### Mala práctica 5: `atoi()` y `atof()`

`atoi()` y `atof()` son dos funciones de C que permiten convertir de cadenas de caracteres basadas en arrays de char a variables de tipo `int` y `float` respectivamente.

El problema que presentan es que, en caso de que la conversión falle, los valores devueltos podrían ser considerados conversiones válidas. 

- `atoi()` devuelve 0 si la conversion falla. Pero ¿qué sucede si el valor que queremos convertir es, precisamente, 0?. Si estamos leyendo un valor desde un fichero o desde teclado, no podemos saber qué valor se va a pasar a la función. Y, por tanto, no podríamos saber si ese 0 es un indicador de fallo de conversión o es, en efecto, el número leído de fichero o teclado que queremos convertir.

- `atof()` presenta un problema similar. En este caso, si la conversión falla, la función devuelve 0.0. Lo cual nos lleva a la misma situación que en `atoi()`. 

Además, en caso de que el valor leído esté fuera de rango, el valor devuelto no está definido.

#### Soluciones en C

1. **Familia de funciones `strtof()` y `strtoimax()`**

	Esta familia de funciones está formada por
	
	- `strtof()`: Conversión a `float`
	- `strtod()`: Conversión a `double`
	- `strtold()`: Conversión a `long double`
	- `strtoimax()`: Conversión a `intmax_t` (el `int` de mayor tamaño)
	- `strtouimax()`: Conversión a `uintmax_t` (el `unsigned int` de mayor tamaño)
	- `strtoul()`: Conversión a `unsigned long`
	- `strtoull()`: Conversión to `unsigned long long` 

	La sintaxis de estas funciones es
		
		float strtof(char cadena_convertir[], char** puntero_siguiente_cadena);
		intmax strtoimax(char cadena_convertir[], char** puntero_sig_cadena, int base_numero);

	**NOTA: El resto son bastante similares, cambiando los tipos**

	Donde `puntero_siguiente_cadena` es un puntero al inicio de la siguiente cadena a convertir. Estas funciones usan el espacio como separador de cadena, con lo cual el puntero apuntará al primer caracter tras el espacio al final de la cadena actual.
	
	En caso de éxito, devuelven el número ya convertido. En caso de que el valor no se pueda convertir, se escribirá en la variable `errno` el error correspondiente y la función devolverá 0. 
	
	En caso de que el valor convertido esté fuera de rango, `strtof()` devolverá `HUGE_VAL`, `HUGE_VALF` or `HUGE_VALL` según el caso y `errno` tomará el valor `ERANGE`. Si el fuera de rango se da en `strtointmax()`o `strtouintmax()`, este devolverá `INTMAX_MAX`, `INTMAX_MIN` o `UINTMAX_MAX`
	
	En algunos compiladores como GCC, la variable `errno` toma el valor `EINVAL` en caso de que la conversión no sea posible (por ejemplo, si el usuario introduce la cadena "cero"). **Pero esto no es estándar, por lo que en otros compiladores los flags podrían ser distintos, o incluso no generar flag en `errno` en esta situación**
	
	
	Un ejemplo de uso con `strtoimax()` sería:
	
	
		char cadena[] = "45";
		char* siguiente;
		
		int valor = strtoimax(cadena, &siguiente);
		
		if(errno == ERANGE) printf("Error: valor fuera de rango);
		else printf("El valor obtenido es %d", valor);
	
	
2. **`sscanf()`**

	Esta función es similar a `sprintf()`, pero usando la cadena como entrada en lugar de como salida. Es una versión de la función `scanf()` que permite leer los datos desde una cadena de caracteres.
	
	**Esta función tiene como ventaja respecto a las anteriores el que, en caso de que la cadena no tenga conversión posible, devuelve un valor erróneo`**, permitiendo saber si la cadena recibida no se corresponde un número.
	
	Su sintaxis es 
		
		int sscanf(const char cadena_convertir[], const char formato[], parametros...)
	
	En caso de éxito, la función guardará en las variables indicadas en los parámetros los valores en los tipos indicados en el formato, y devolverá la cantidad de variables rellenas. En caso de un error en la entrada antes de procesar los datos, devolverá EOF.
	
	**De esta manera, podemos saber si ha habido algún error de conversión, comprobando si la cantidad de variables rellenas coincide con la cantidad de parámetros enviados a la función.**
	
	 En este caso, para convertir a números, se realizaría algo como esto:
	 
	 	char cadena[] = "45";
	 	int numero;
	 	
	 	int resultado = sscanf(cadena, "%d", numero);
	 	if(resultado != 1) printf("Error de conversion");
	 	else printf("El número recibido es %d\n", numero);
	
#### Soluciones en C++

 En C++, disponemos de múltiples opciones dentro de la STL para convertir cadenas a números.
 
1. Familia de funciones `std::stoi()` y `std::stof()`
 
 	Estas funciones son equivalentes a `atoi()` y `atof()`, pero tomando como entrada cadenas de `std::string`. Pero, como mejora a las anteriores, en caso de error, **estas funciones generan excepciones que nos permiten diferenciar el error obtenido.**
 	
 	- Si no hay conversión posible, se genera la excepción `std::invalid_argument `
 	- Si el valor está fuera del rango de su tipo de datos, se genera la excepción `std::out_of_range`. Además, se establece la variable `errno` a `ERANGE`
 	
	Un ejemplo de uso de `str::stoi()`, partiendo de una cadena de C, sería:
	
		char cadena[] = "23";
		std::string cadena_str(cadena);
 	
 		try{
 			int valor = std::stoi(cadena_str);
 		
 		}catch (std::invalid_argument const& ex) //valor no convertible a numero
        {
            std::cout << "std::invalid_argument::what(): " << ex.what() << '\n';
        }
        catch (std::out_of_range const& ex) //Valor fuera de rango
        {
            std::cout << "std::out_of_range::what(): " << ex.what() << '\n';
            const long long ll{std::stoll(s, &pos)};
            std::cout << "std::stoll('" << s << "'): " << ll << "; pos: " << pos << '\n';
        }
 
2. **`std::stringstream`**

	La clase `std::stringstream` permite crear un `stream` a partir de una cadena de caracteres. Sobre ese stream se puede cargar un dato con el operador `<<` (como en `std::cout`) y extraerlo a una variable con el operador `>>`. Al extraer el dato a una variable, se aplica automáticamente la conversión necesaria para ajustarla a la misma. 
	
	En este caso, podemos utilizar la clase para cargar una cadena de caracteres y extraerla a una variable numérica como `int`, `float` o similar, de esta manera:
	
		char cadena[] = "20";
		std::stringstream sstream(cadena);
		int valor;
		sstream >> valor;
	
	La función intentará convertir la cadena a un valor compatible con dicho tipo de datos y guardará el resultado en la variable indicada tras el operador. 
	
	En caso de error, la función activará los flags correspondientes:
	
	- `goodbit`: sin errores
	- `eofbit`: fin de fichero encontrado durante la operación
	- `failbit`: error de conversión
	- `badbit`: error de lectura/escritura

	Para comprobarlos, podemos usar el método `rdstate()` o podemos acceder a los métodos de la clase asociados a cada flag
	
	- método `fail()`: devuelve `true` si está activo `failbit`
	- método `bad()`: devuelve `true` si está activo `badbit`
	- método `eof(): devuelve `true` si está activo `eofbit`

	Si queremos realizar una nueva operación sobre el stream, podemos utilizar el método `clear()` para borrar los flags anteriores
 
 	Un ejemplo de uso de `std::stringstream` sería
 	
 		char cadena[] = "20";
 		std::stringstream(cadena);
 		
 		int valor;
 		sstream >> valor;
 		
 		if(sstream.good()) std::cout<<"Operación correcta\n";
 		else if(sstream.bad())
 			std::cout<<"Error de lectura/escritura\n";
 		else if(sstream.fail())
 			std::cout<<"Error de conversión\n";
 		else if(sstream.eof())
 			std::cout<<"Error: final de fichero encontrado\n";
 

3. **`std::from_chars()` (C++17 en adelante)**

	La función `std::from_chars()` permite convertir cadenas de caracteres a otro tipo de dato. A diferencia de `std::stringstream`, esta función no tiene en cuenta las locales del sistema, simplemente comprueba que el tamaño concuerda con el del tipo de dato. A cambio, es mas preciso que `std::stringstream` en números en coma flotante. 
	
	La función pertenece a la librería `<charconv>`
	
	Su sintaxis es
	
		std::from_chars_result std::from_chars(char *inicio_cadena, char *fin_cadena, [tipo_entero] &valor, int [base]); //Para valores enteros
		
		std::from_chars_result std::from_chars(char *inicio_cadena, char *fin_cadena, [tipo_flotante] &valor, std::chars_format [formato]);
		
	
	El tipo del valor puede ser  cualquier tipo númerico, tanto de entero como en coma flotante. **Este parámetro almacenará el valor ya convertido tras la ejecución de la función**.
	
	- **En el caso de tipos enteros, podrá indicarse la base con la que se trabaja (base 10, base 16, base 2...). Si no se indica, se asumirá base 10**
	
	- **En los tipos de coma flotante, podrá indicarse distintos formatos**
		+ Si el formato tiene `std::chars_format::scientific` pero no `std::chars_format::fixed`, se considerará notación científica y se requerirá un exponente.
		+ En caso inverso, (scientific y no fixed), se asumirá coma fija y no se permitirá exponente
		+ Si el formato es `std::chars_format::hex`, se considerará un valor hexadecimal y no se permitirán los prefijos `0X` ni `0x`. En caso de encontrarlos, se considerará que el valor es 0 y se despreciará el resto.
		+ Además, los espacios en blanco iniciales no serán despreciados.
	

	La función devuelve un objeto de tipo `std::from_chars_result`, en el cual se indicará el éxito o fracaso de la operación
	
	- En caso de éxito, el atributo `ptr` apuntará al primer caracter que no coincida con el formato, o su valor será igual al del parámetro `fin_cadena` en caso de haber convertido la cadena completa.
	
	- Si la cadena no coincide con el formato indicado, o no se ha podido convertir al tipo indicado, el atributo `ptr` coincidirá con el parametro `inicio_cadena`, el atributo `ec` tomará el valor ` std::errc::invalid_argument`, y el parámetro `valor` no será modificado.
	
	- Si la cadena se ha podido convertir, pero el valor obtenido no cabe en el tipo indicado, el atributo `ec` tomará el valor `std::errc::result_out_of_range` y el atributo `ptr` coincidirá con `inicio_cadena`. Además, el parámetro `valor` no será modificado.
	
	Un ejemplo de uso es el siguiente:
	
		#include <charconv> // from_char, to_char
		#include <string>
		#include <iostream>
		
		int main()
		{
		    const std::string str { "12345678901234" };
		    int valor = 0;
		    std::from_chars_result resultado = std::from_chars(str.data(), str.data() + str.size(), valor);
		
		    if (resultado.ec == std::errc())
		    {
		        std::cout << "Valor obtenido: " << value 
		                  << ", distancia recorrida: " << res.ptr - str.data() << '\n';
		    }
		    else if (resultado.ec == std::errc::invalid_argument)
		    {
		        std::cout << "Error: el valor no es representable en el formato indicado\n";
		    }
		    else if (resultado.ec == std::errc::result_out_of_range)
		    {
		        std::cout << "Error: valor fuera de rango!! distancia: " 
		                  << resultado.ptr - str.data() << '\n';
		    }
		}
	
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

En C, la manera habitual de indicar que un puntero no está inicializado es asignarle el valor `NULL`. Este valor no tiene asignado ningún tipo, sino que es una simple definición tal que

	#define NULL 0

Pero esta definición es algo débil, por lo que puede ser utilizada de forma errónea

	int array[20];
	array[NULL] = 10;

Como `NULL` vale 0, esta instruccion asignaría el valor 10 a la posición 0 del array, lo cual resulta confuso.

#### Problemas en C++

En C++, el problema se complica con los pasos por referencia. En un paso por referencia (no confundir con un paso por puntero), el acceso a la memoria de la variable original es implícito.

La función se declara como

	void funcion_referencia(_const_ [tipo] &parametro_referencia);

Y se invoca con

	[tipo] variable;
	funcion_referencia(variable);

Pero esto, al mezclarlo con el `NULL` de C, puede producir el siguiente problema

	void funcion_referencia(const int& variable);
	
	...
	
	int main(){
		funcion_referencia(NULL);
	}

Al ser una referencia constante, no requiere que se pase una variable explícitamente. Y, dado que `NULL` vale 0 y no tiene ningún tipo, su valor es compatible con el de una variable tipo `int` (o cualquier variable numérica, en general), por lo que la referencia asume un valor inválido.

#### Solución

La solución es usar `nullptr`. Esta es una variable de tipo `nullptr_t`, que reemplaza al `NULL` de C. Al tener un tipo, impide pasarlo por error como si fuera un valor numérico. E impide que se pueda utilizar para reemplazar el valor 0.

Así pues, el caso anterior, al aplicar `nullptr` quedaría así:

	void funcion_referencia(const int& variable);
	
	...
	
	int main(){
		funcion_referencia(nullptr);
	}


Esto daría un error de compilación, puesto que `nullptr` no es de tipo `int` sino de tipo `nullptr_t`

	funcion_cpp.cpp:8:19: error: invalid initialization of reference of type ‘const int&’ from expression of type ‘std::nullptr_t’


### Mala práctica 2: comprobación de éxito del operador `new` mediante comparación con `NULL` (c++)

En C++, el operador `new` sirve para reservar memoria para una variable o para una estructura.

Pero, a diferencia de su equivalente `malloc()` de C, `new` no devuelve `NULL` en caso de error al reservar memoria. En cambio, genera una excepción, que se captura con `try-catch`. Por tanto, este código no funcionaría

	int* data = new int;
	if(data != NULL){ 
		... 
	}

#### Solución 1: Capturar excepción con try-catch

La primera solución consiste en capturar la excepción devuelta por `new`, que en este caso es `std::bad_alloc`. Esto se haría con un bloque try-catch tal que así

	try {
        int* dato = new int;
 	    std::cout<<"Memoria reservada correctamente\n"; //si la reserva es correcta, llegará aquí
    }
 
    // Si salta la excepción, la capturamos aquí
    catch (const std::bad_alloc& e) {
        std::cout<<"Error al reservar memoria: <<e.what()<<"\n";
    }

En caso de error, `e.what()` mostrará la causa del error.

#### Solución 2: Desactivar lanzamiento de excepciones, devolviendo `nullptr`

La segunda solución consiste en desactivar el lanzamiento de excepciones, forzando la devolución de `nullptr` (distinto de `NULL`, como indicamos anteriormente) en caso de error. Esto se hace aplicando la cláusula `std::nothrow` en el constructor de `new`.

	int dato = new(std::nothrow) int;
	if(dato == nullptr){
		std::cout<<"Error al reservar memoria\n";
	}
	else{
		... 
	}

## Buenas prácticas

### Buena práctica 1: Uso de `<stdint.h>` para tipos enteros

Los tipos clásicos para números enteros tienen un tamaño dependiente de la arquitectura

- `int` usa 16 bits en x86_32 y 32 bits en x86_64
- `long int` usa 32 bits en x86_32 y 64 bits en x86_64
- `long long int` usa 64 bits en x86_32 y 64 bits en x86_64

Esto supone un gran problema al hacer programas multiplataforma, dado que no se conoce con certeza el tamaño que tendrán las variables enteras. Así, no se puede asegurar que el dato quepa correctamente en el tipo de dato elegido, ni de que las operaciones no produzcan desbordamento. 

O incluso, que un fichero binario escrito en una plataforma, se pueda leer en otro, dado que la cantidad de bytes puede ser distinto en origen y destino.


#### Solución

La librería `<stdint.h>` ofrece alternativas de tamaño fijo para los tipos clásicos. Estos son:

- `int8_t`: Entero con signo de 8 bits
- `uint8_t`: Entero sin signo de 8 bits
- `int16_t`: Entero con signo de 16 bits
- `uint16_t`: Entero sin signo de 16 bits
- `int32_t`: Entero con signo de 32 bits
- `uint32_t`: Entero sin signo de 32 bits
- `int64_t`: Entero con signo de 64 bits
- `uint64_t`: Entero sin signo de 64 bits

Su uso es tan simple como indicar dichos tipos al crear la variable.  
Por ejemplo:

	#include <stdint.h>
	
	...
	
	int8_t littlenum;

### Buena práctica 2: Uso de `<stdbool.h>` para tipos booleanos (C)

En C, por defecto no hay tipos booleanos, y se suele considerar 0=false y 1=true.
Pero esto puede ser problemático, habiendo escenarios donde no queda claro si la comparación comprueba un valor numérico o un booleano simulado mediante 0/1.

#### Solución

La librería `<stdbool.h>` ofrece un tipo `bool` en C, semejante al ya existente en C++. Este puede admitir los valores `true` y `false`, eliminando la confusión entre valores numéricos y booleanos.

Su uso es muy simple

	#include <stdbool.h>

	bool cumple=true;


### Buena práctica 3: Uso de `using` en lugar de `typedef` en C++

En C, el operador `typedef` permite crear tipos basados en un tipo ya existente.
Esto se puede usar para poner una abreviatura a un tipo de dato con nombre largo o complejo.

En C++, desde C++11, existe la expresión `using` para crear alias a tipos. Esta tiene la sintaxis

	using [alias] = tipo_original;

Esta expresión tiene múltiples ventajas respecto al `typedef` de C:

- **Creación de tipos basados en `templates`:** Se puede añadir un template a un alias, para que se aplique sobre el template del tipo original

		template<typename T1, typename T2>
		using dict = std::map<T1, std::vector<T2>>;

		dict<std::string, int> dict_int;
		dict<std::string, std::string> dict_str;
		dict<int, float> dict_int_float;

- **Creación de tipos sin nombre:** Esto sirve para eliminar el prefijo del *namespace* en los tipos provenientes de uno.

		using std::cout;
		cout<<"Hello world";

- **Fácil modificación:** El alias se puede redefinir para asignarse a un nuevo tipo

- **Sintaxis mas sencilla**: La sintaxis es mas sencilla e intuitiva que `typedef`. Por ejemplo, para crear un alias para una cadena basada en array de char.

		typedef char cadena[MAXLONG]; //¿el tipo original es char o cadena?
	
	vs
		
		using cadena = char[MAXLONG];	 //queda mas claro que cadena es el alias y char[] el tipo original

**Limitaciones**

Como diferencia respecto a `typedef`, **`using` no es una sentencia de inicialización**, por lo que no se puede utilizar en contextos de inicialización. Por ejemplo, no se puede definir el tipo en la cabecera de un bucle for.

En las últimas versiones de C++, a partir de C++23, ya se permite este tipo de uso, pero solo en algunos contextos.

Por ejemplo, en este código usamos `typedef` en la inicialización del bucle for para definir un tipo `Foo`, que se utiliza en la condición del bucle.

	for (typedef int Foo; Foo{} != 0;) //creamos el tipo dentro del inicializador del for
	{
	 ...
	}

Esto, con `typedef`, compila sin problemas. Pero si cambiamos el `typedef` por una sentencia `using`, nos saldrá un aviso indicando que esto solo es posible a partir de C++23.

	for (using Foo = int; Foo{} != 0;) {}

Nos saldrá este aviso

	 warning: alias-declaration in init-statement only available with ‘-std=c++23’ or ‘-std=gnu++23’ [-Wc++23-extensions]
	   14 |         for (using Foo = int; Foo{} != 0;) {


Por tanto, el uso de `using` en sentencias de inicialización solo está admitido a partir de C++23, 

### Buena práctica 4: Uso de `std::string` para cadenas de caracteres (C++)

En C++, la clase `std::string` permite crear cadenas de caracteres de tamaño dinámico, además de incluir una gran cantidad de operadores para poder manipularlas.

Esto aporta una gran cantidad de ventajas respecto a las cadenas basadas en arrays de char, propias de C:

- Se puede asignar con `=` y comparar con `==`
- Se puede concatenar con `+`
- No se necesita conocer el tamaño inicial de la cadena para poder asignarlo
	+ Tampoco hay que almacenarlo. Se puede obtener el tamaño con el método `size()`
	
Como único defecto, no permite definir cadenas de tamaño fijo, aunque se puede reservar un tamaño inicial en su constructor.

Su uso es bastante sencillo:

	#include <string>
	#include <iostream>
	
	int main(){
		std::string cadena1{"hola mundo"};
		std::string cadena2{cadena1}; //cadena2 valdrá "hola mundo"
		std::string cadena3 = "hola";
		
		cadena3 += " mundo"; //concatenamos la palabra " mundo" a cadena 3
		
		if(cadena3 == "hola mundo") std::cout<<"Cadena 3 concatenada correctamente\n";
		else std::cout<<"El contenido de cadena 3 es "<<cadena3<<"\n";
		
		if(cadena1 == cadena3) std::cout<<"cadena1 y cadena3 son iguales\n";
		else std::cout<<"cadena 1 y cadena 3 no son iguales\n";
		
		return 0;
	}


### Buena práctica 5: Uso de `std::vector` y `std::array` para vectores (dinámicos) y arrays (estáticos) en C++

C++ proporciona las clases `std::vector` y `std::array` para vectores (tamaño dinámico) o arrays (tamaño fijo).

Estas tienen múltiples ventajas respecto a los arrays de C, especialmente en `std::vector`

- `std::vector` no necesita establecer un tamaño inicial (aunque se puede indicar)
	+ En `std::array` se indica en el constructor
- Se pueden comparar con `==` 
- Se pueden asignar y copiar con `=`
- Se pueden pasar por copia (aunque es poco recomendable)
	+ Se pueden pasar por referencia de forma explícita (no degeneran a puntero)
- Se pueden devolver en una función con `return`

- Se puede obtener su tamaño con el método `size()`

- **Se pueden combinar con templates**

Su uso es bastante sencillo

	#include <vector>
	#include <array>
	#include <iostream>
	
	int main(){
		std::vector<std::string> v1;
		v1.push_back("abc"); //inserta abc al final del vector (0 en este caso, dado que está vacío)
		v1[1] = "fgh";
		
		std::vector<std::string> v2 = v1;
		
		if(v1 == v2){
			std::cout<<"Asignación correcta\n";
			std::cout<<"El tamaño de v2 es "<<v2.size()<<"\n";
		}
		
		//crea un vector de 5 float (tamaño inicial) con valor 6.7
		std::vector<float, 5, 6.7> vf; 
		vf.push_back(8.6); //inserta un nuevo elemento (expandiendo el vector) con valor 8.6
		
		std::vector<float> vf2{vf}; //crea un nuevo vector con el mismo contenido de vf
		vf2.erase(vf2.begin() + 2); //borra la posición 2, redimensionando el vector automáticamente
		
		std::array<int, 4> a = {1,2,3,4}; //crea un array de 4 posiciones de tipo int
		std::array<int, 4> a1;
  		a1 = {1,2,3,4};
  
  		std::array<int,4> a2{a1};
  
		for(int i = 0; i < a2.size(); i++){
		    std::cout<<a2[i]<<" ";
		}
		  
		a2[2] = 6;
		  
		std::cout<<"\n";
		for(int data: a2){ //iteramos el array con un for-range
		    std::cout<<data<<" "; //en cada iteración, data almacena el valor de una posición
		}
		
		return 0;
				
	}


### Buena práctica 6: Uso de `static_cast`, `dynamic_cast` ... para convertir tipos en C++

WIP


## Referencias

- https://cplusplus.com/articles/4z18T05o
- https://learn.microsoft.com/es-es/windows/console/clearing-the-screen
- https://stackoverflow.com/questions/32109646/how-to-write-a-struct-to-file-in-c-and-read-the-file
- https://parzibyte.me/blog/2022/01/01/guardar-recuperar-struct-archivo-cpp/
- https://stackoverflow.com/questions/21344106/serializing-struct-to-file-and-deserializing-it-again-with-string
- https://en.cppreference.com/w/cpp/string/basic_string_view
- https://stackoverflow.com/a/26083517
- https://en.cppreference.com/w/cpp/string/basic_string/stol
- https://marketsplash.com/tutorials/cpp/cplusplus-stringstream/
- https://en.cppreference.com/w/cpp/io/basic_stringstream
- https://stackoverflow.com/a/55875943 
- https://www.cppstories.com/2018/12/fromchars/
- https://en.cppreference.com/w/cpp/utility/from_chars
- https://www.learncpp.com/cpp-tutorial/introduction-to-stdstring_view/
- https://www.sandordargo.com/blog/2022/07/13/why_to_use_string_views
- https://rules.sonarsource.com/c/RSPEC-929/
- https://en.wikibooks.org/wiki/C_Programming/stdint.h
- https://en.cppreference.com/w/cpp/container/vector
- https://jesustorres.hashnode.dev/conversion-de-tipos-en-cpp
- https://stackoverflow.com/questions/18628918/c-new-operator-and-error-checking
- https://www.geeksforgeeks.org/if-memory-allocation-using-new-is-failed-in-c-then-how-it-should-be-handled/
- https://en.cppreference.com/w/cpp/memory/new/operator_new
- https://www.internalpointers.com/post/differences-between-using-and-typedef-modern-c
- https://www.geeksforgeeks.org/cpp-using-vstypedef/
- https://stackoverflow.com/a/62196340

