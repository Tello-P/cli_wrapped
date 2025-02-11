#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HISTORY_FILE "/home/tello/.zsh_history"
#define MAX_COMANDOS 10
#define MAX_COMANDOS_REPETICION 20
#define MAX_LONGITUD 256  

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void obtener_comandos(char comandos[MAX_COMANDOS][MAX_LONGITUD], int *num_comandos)
{    	
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
void contador_comandos(char comandos[MAX_COMANDOS][MAX_LONGITUD], char repeticion_comandos[MAX_COMANDOS_REPETICION][MAX_LONGITUD], int *num_repetidos)
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
void swap(int j, char repeticion_comandos[MAX_COMANDOS_REPETICION][MAX_LONGITUD])
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
void ordenar_repeticion_comandos(char repeticion_comandos[MAX_COMANDOS_REPETICION][MAX_LONGITUD], int *num_repetidos )
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
				swap(j,repeticion_comandos);
			}
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

	char comandos[MAX_COMANDOS][MAX_LONGITUD];
    	char repeticion_comandos[MAX_COMANDOS_REPETICION][MAX_LONGITUD];
	int num_comandos = 0;
	int num_repetidos = 0;

    	obtener_comandos(comandos, &num_comandos);

	contador_comandos(comandos,repeticion_comandos,&num_repetidos);
    	
	ordenar_repeticion_comandos(repeticion_comandos,&num_repetidos);

	for (int i = 0; i < num_repetidos; i++) 
	{
        	printf("%s\n", repeticion_comandos[i]);
    	}

	
	return 0;
}

