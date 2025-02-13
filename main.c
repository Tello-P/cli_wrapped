#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HISTORY_FILE "/home/tello/.zsh_history"
//#define MAX_COMANDOS_REPETICION 200
#define MAX_LONGITUD 256  



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int longitud_historial()
{
	FILE *f = fopen(HISTORY_FILE, "r");
    	if (f == NULL)
	{
        	perror("Archivo no encontrado");
        	exit(1);
	}

	int MAX_COMANDOS = 0;
	char buffer[MAX_LONGITUD];

	while (fgets(buffer,sizeof(buffer),f))
	{
		MAX_COMANDOS++;
	}
	fclose(f);
	return MAX_COMANDOS;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void obtener_comandos(const int MAX_COMANDOS, char comandos[MAX_COMANDOS][MAX_LONGITUD], int *num_comandos)
{    	
	/* Obtiene desde el fichero HISTORY_FILE
	 * todos los comandos almacenados en 
	 * formato string, en orden de uso, de mas
	 * antiguo a mas reciente.
	 */

	FILE *f = fopen(HISTORY_FILE, "r");
    	if (f == NULL)
	{
        	perror("Archivo no encontrado");
        	exit(1);
    	}

    	*num_comandos = 0;  

    	while (fgets(comandos[*num_comandos], MAX_LONGITUD, f) != NULL)
	{
        	comandos[*num_comandos][strcspn(comandos[*num_comandos], "\n")] = '\0';
        
        	(*num_comandos)++;  
	
        	if (*num_comandos >= MAX_COMANDOS)
		{
        	    	break;
        	}
    	}

	    fclose(f);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void contador_comandos(const int MAX_COMANDOS_REPETICION, const int MAX_COMANDOS, char comandos[MAX_COMANDOS][MAX_LONGITUD], char repeticion_comandos[MAX_COMANDOS_REPETICION][MAX_LONGITUD], int *num_repetidos)
{

	int coincidencia;
	int contador = 0;
	char num[20];
	int N;
	int len_comandos = MAX_COMANDOS;
	int len_repeticion = MAX_COMANDOS_REPETICION;
	for (int i=0; i<len_comandos; i++)
	{
	//	En principio esto no hace falta pero nunca se sabe
	//	coincidencia = 0;	
	
		/* Busca en el array comandos, extraido en bruto,
		 * si ya esta dentro de repeticion_comandos
		 * si es asi añade uno al contador de repeticiones
		 * y si no lo mete a la lista y añade el 1.
		 *
		 * repeticion_comandos tiene esta estructura:
		 * [palabra,numero,palabra,numero...]
		 * 
		 * ¡IMPORTANTE!
		 * tanto palabra como numero son strings y deben
		 * tratarse como tal.
		 */

		for (int j=0; j<len_repeticion; j=j+2)
		{
			if (strcmp(repeticion_comandos[j],comandos[i]) != 0)
			{
				coincidencia = 0;
				
			}
			else
			{
				coincidencia = 1;
				N = atoi(repeticion_comandos[j+1]);
				N++;
				sprintf(num, "%d",N);
				strcpy(repeticion_comandos[j+1],num);
				break;
			}
			
		}
		if (coincidencia == 0)
		{
			(*num_repetidos) = (*num_repetidos) + 2;
			strcpy(repeticion_comandos[contador],comandos[i]);
			N = 1;
			sprintf(num, "%d",N);
			strcpy(repeticion_comandos[contador + 1],num);
			contador++;
			contador++;
		}
	}
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void swap(const int MAX_COMANDOS_REPETICION, int j, char repeticion_comandos[MAX_COMANDOS_REPETICION][MAX_LONGITUD])
{
	char temp[MAX_LONGITUD];

	// Copiar los numeros
	strcpy(temp,repeticion_comandos[j]);
	strcpy(repeticion_comandos[j],repeticion_comandos[j+2]);
	strcpy(repeticion_comandos[j+2],temp);

	// Copiar los comandos
	strcpy(temp,repeticion_comandos[j-1]);
	strcpy(repeticion_comandos[j-1],repeticion_comandos[j+1]);
	strcpy(repeticion_comandos[j+1],temp);

}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void ordenar_cadena(const int MAX_COMANDOS_REPETICION, char repeticion_comandos[MAX_COMANDOS_REPETICION][MAX_LONGITUD], const int *num_repetidos, char comandos_ordenados[*num_repetidos][MAX_LONGITUD] )
{
	int num1,num2;
	int len = *num_repetidos;
	
	/* Se utiliza el algortimo de ordenacion de
	 * burbuja, se podría mejorar pero es lo que
	 * hay, los datos se guardan en repeticion_comandos
	 * pero solo son validos hasta len, luego hay
	 * espacios vacios con caracteres aleatorios
	 */

	for (int i=1; i<(len+1); i=i+2)
	{
		for (int j=1; j<(len-i); j=j+2)
		{
			num1 = atoi(repeticion_comandos[j]);
			num2 = atoi(repeticion_comandos[j+2]);
			//printf("NUM1 %d, NUM2 %d\n",num1,num2);
			if (num1 > num2)
			{
				swap(MAX_COMANDOS_REPETICION,j,repeticion_comandos);
			}
		}
	}

	// Copiar a la cadena comandos_ordenados
	for (int i=0; i<len; i++)
	{
		strcpy(comandos_ordenados[i],repeticion_comandos[i]);
	}
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void obtener_comandos_principales_ordenados(const int MAX_COMANDOS_REPETICION, const int *num_repetidos, char comandos_ordenados[*num_repetidos][MAX_LONGITUD], char comandos_principales[MAX_COMANDOS_REPETICION][MAX_LONGITUD], int *num_principales) 
{
	/* Añadir una lista con la primera palabra de
	 * repeticion_comandos o orden_repeticion_comandos,
	 * que tenga solo la primera palabra por ejemplo:
	 * [cat hola.txt,2, cat main.c,4] deberia ser [cat, 2, cat, 4]
	 */

    	int len = *num_repetidos;

   	 for (int i = 0; i < len; i++) 
	 {
        	int j = 0;

        	// Copiar caracteres hasta el primer espacio o fin de la cadena
        	while (comandos_ordenados[i][j] != ' ' && comandos_ordenados[i][j] != '\0' && j < MAX_LONGITUD) 
        	{
        	    comandos_principales[*num_principales][j] = comandos_ordenados[i][j];
        	    j++;
        	}

        	// Asegurar el final de la cadena
        	comandos_principales[*num_principales][j] = '\0';

        	// Incrementar el contador si hay una palabra válida
        	if (j > 0)
		{
        	    (*num_principales)++;
        	}
    	}
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void sumar_comandos_principales(const int MAX_COMANDOS_REPETICION, const int *num_principales, int *num_suma_principales,char suma_comandos_principales[MAX_COMANDOS_REPETICION][MAX_LONGITUD], char comandos_principales[MAX_COMANDOS_REPETICION][MAX_LONGITUD])
{
	int contador = 0;
	int num1,num2,suma;
	char N[20];
	int encontrado;

	for (int i=0; i+1<*num_principales; i=i+2)
	{
		encontrado = 0;
		for (int j=0; j+1<*num_principales; j=j+2)
		{
			if (strcmp(comandos_principales[i],suma_comandos_principales[j]) == 0)
			{
				// Pasar los numeros a int
				num1 = atoi(comandos_principales[i+1]);
				num2 = atoi(suma_comandos_principales[j+1]);
				// Sumarlos
				suma = num1 + num2;
				// Convertir a string de nuevo
				sprintf(N, "%d",suma);
				// Devolver a la posicion
				strcpy(suma_comandos_principales[j+1],N);
				encontrado = 1;
				break;
			}
		}
		if (!encontrado)
		{
			// Copiar el string
			strcpy(suma_comandos_principales[contador],comandos_principales[i]);
			contador++;
			//Copiar el numero
			strcpy(suma_comandos_principales[contador],comandos_principales[i+1]);
			contador++;
			(*num_suma_principales)++;
			(*num_suma_principales)++;
		}
		
	}
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int main()
{
	/* comandos --> history extraido en bruto
	 * repeticion_comandos --> history con el numero de veces que se repite
	 * cada palabra, [palabra,num,palabra,num...] ¡num es un string!
	 * num_comandos --> contador para imprimir los valores de contador
	 * probablemente se puede quitar si no hace falta listar contador
	 * num_repetidos --> numero de comandos que se han usado, sin repetirlos
	 */
	
	int MAX_COMANDOS,MAX_COMANDOS_REPETICION;
	MAX_COMANDOS = longitud_historial();
	MAX_COMANDOS_REPETICION = MAX_COMANDOS * 2;


	char comandos[MAX_COMANDOS][MAX_LONGITUD];
    	char repeticion_comandos[MAX_COMANDOS_REPETICION][MAX_LONGITUD];
	char comandos_principales[MAX_COMANDOS_REPETICION][MAX_LONGITUD];


	int num_comandos = 0;		//Num total de comandos en history, pueden estar repetidos
	int num_repetidos = 0;		//Num total de comandos unicos en histoty, sin repeticiones
	int num_principales = 0;	// Num total de comandos principales, solo la primera palabra
	int num_suma_principales = 0;



	// Meter los comandos en char comandos
    	obtener_comandos(MAX_COMANDOS, comandos, &num_comandos);


	// Cuenta el numero de veces que se repite cada comando y lo mete a repeticion_comandos
	contador_comandos(MAX_COMANDOS_REPETICION, MAX_COMANDOS, comandos,repeticion_comandos, &num_repetidos);
    	

	// Ordena repeticion_comandos, dejando el mayor en la ultima posicion
	char comandos_ordenados[num_repetidos][MAX_LONGITUD];
	ordenar_cadena(MAX_COMANDOS_REPETICION, repeticion_comandos, &num_repetidos, comandos_ordenados);


	// Obtiene solo la primera palabra del comando, el comando principal sin argumentos
	obtener_comandos_principales_ordenados(MAX_COMANDOS_REPETICION, &num_repetidos ,comandos_ordenados, comandos_principales, &num_principales);


	// Suma los valores de comandos_principales
	char suma_comandos_principales[MAX_COMANDOS_REPETICION][MAX_LONGITUD];
	sumar_comandos_principales(MAX_COMANDOS_REPETICION, &num_principales, &num_suma_principales, suma_comandos_principales, comandos_principales);
	

	// Ordenar los valores de suma_comandos_principales
	char suma_comandos_principales_ordenados[MAX_COMANDOS_REPETICION][MAX_LONGITUD];
	ordenar_cadena(MAX_COMANDOS_REPETICION, suma_comandos_principales, &num_suma_principales, suma_comandos_principales_ordenados);

	/* PRUEBAS DE SALIDA */
	for (int i = 0; i < num_suma_principales; i++) 
	{
        	printf("%s\n", suma_comandos_principales_ordenados[i]);
    	}



	return 0;
}

