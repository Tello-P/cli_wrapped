#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#define HISTORY_FILE "/home/tello/.zsh_history"
#define HISTORY_FILE_DATE "/home/tello/.zsh_history_date"
#define MAX_LONGITUD 500 



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

void obtener_comandos_date(const int MAX_COMANDOS, char comandos_date[MAX_COMANDOS][MAX_LONGITUD],int *num_comandos_date)
{
	/* Obtiene desde el fichero HISTORY_FILE_DATE
	 * todos los comandos almacenados en 
	 * formato string, en orden de uso, de mas
	 * antiguo a mas reciente.
	 */

	FILE *f = fopen(HISTORY_FILE_DATE, "r");
    	if (f == NULL)
	{
        	perror("Archivo no encontrado");
        	exit(1);
    	}

    	*num_comandos_date = 0;  

    	while (fgets(comandos_date[*num_comandos_date], MAX_LONGITUD, f) != NULL)
	{
        	comandos_date[*num_comandos_date][strcspn(comandos_date[*num_comandos_date], "\n")] = '\0';
        
        	(*num_comandos_date)++;  
	
        	if (*num_comandos_date >= MAX_COMANDOS)
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
void sumar_comandos_date(int *num_comandos_date_suma, const int MAX_COMANDOS, const int *num_comandos_date, char comandos_date[MAX_COMANDOS][MAX_LONGITUD], char comandos_date_suma[(*num_comandos_date) * 2][MAX_LONGITUD]) {
    int len = *num_comandos_date;
    int contador_fecha = 0;
    char comandos_solo_date[MAX_COMANDOS][MAX_LONGITUD];

    for (int i = 0; i < len; i++) {
        int j = 0;

        // Copiar caracteres hasta el primer espacio o fin de la cadena
        while (comandos_date[i][j] != ' ' && comandos_date[i][j] != '\0' && j < MAX_LONGITUD) {
            comandos_solo_date[contador_fecha][j] = comandos_date[i][j];
            j++;
        }

        // Asegurar el final de la cadena
        comandos_solo_date[contador_fecha][j] = '\0';

        // Incrementar el contador si hay una palabra válida
        if (j > 0) {
            contador_fecha++;
        }
    }

    // Ahora tenemos la lista con solo las fechas, solo queda sumarlas
    int contador = 0;
    int num1, num2, suma;
    char N[20];

    for (int i = 0; i < contador_fecha; i++) {
        int coincidencia = 0;

        // Comparar si la fecha ya está en comandos_date_suma
        for (int t = 0; t < contador; t += 2) {
            if (strcmp(comandos_solo_date[i], comandos_date_suma[t]) == 0) {
                // Si la fecha existe, sumamos el valor
                num1 = 1;  // Se asume que cada fecha aparece al menos una vez
                num2 = atoi(comandos_date_suma[t + 1]);

                suma = num1 + num2;

                // Convertir la suma a string y actualizar
                sprintf(N, "%d", suma);
                strcpy(comandos_date_suma[t + 1], N);

                coincidencia = 1;
                break;
            }
        }

        // Si la fecha no está en comandos_date_suma, la agregamos
        if (coincidencia == 0) {
            strcpy(comandos_date_suma[contador], comandos_solo_date[i]);
            contador++;
            strcpy(comandos_date_suma[contador], "1");
            contador++;
        }
    }
    *num_comandos_date_suma = contador;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void titulo_inicio(const int num_comandos)
{
	
	if (num_comandos == 0)
	{
		perror("No hay comandos usados todavia");
		exit(1);
	}

	printf("%d",num_comandos);
	printf("   ____ _     ___  __        ______      _    ____  ____  _____ ____  %d\n",num_comandos);
	printf("%d",num_comandos);
	printf("  / ___| |   |_ _| \\ \\      / /  _ \\    / \\  |  _ \\|  _ \\| ____|  _ \\ %d\n",num_comandos);
	printf("%d",num_comandos);
        printf(" | |   | |    | |   \\ \\ /\\ / /| |_) |  / _ \\ | |_) | |_) |  _| | | | |%d\n",num_comandos);
	printf("%d",num_comandos);
        printf(" | |___| |___ | |    \\ V  V / |  _ <  / ___ \\|  __/|  __/| |___| |_| |%d\n",num_comandos);
	printf("%d",num_comandos);
        printf("  \\____|_____|___|    \\_/\\_/  |_| \\_\\/_/   \\_\\_|   |_|   |_____|____/ %d\n", num_comandos);
	printf("%d\n",num_comandos);

}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void meses_barras(const int num_comandos,const int MAX_COMANDOS, char fechas_comandos[MAX_COMANDOS][MAX_LONGITUD])
{
	
	int enero=0, febrero=0, marzo=0, abril=0, mayo=0, junio=0, julio=0, agosto=0, septiembre=0, octubre=0, noviembre=0, diciembre=0;
	int enero2=0, febrero2=0, marzo2=0, abril2=0, mayo2=0, junio2=0, julio2=0, agosto2=0, septiembre2=0, octubre2=0, noviembre2=0, diciembre2=0;


	for (int i = 0; i < num_comandos; i++)
	{

		char mes1 = fechas_comandos[i][5];  
		char mes2 = fechas_comandos[i][6];  

		if (mes1 == '0')
		{
			if (mes2 == '1') enero++;
			else if (mes2 == '2'){febrero++;febrero2++;}
			else if (mes2 == '3'){marzo++;marzo2++;}
			else if (mes2 == '4'){abril++;abril2++;}
			else if (mes2 == '5') mayo++;
			else if (mes2 == '6') junio++;
			else if (mes2 == '7') julio++;
			else if (mes2 == '8') agosto++;
			else if (mes2 == '9') septiembre++;
		}
		else if (mes1 == '1')
		{
			if (mes2 == '0') octubre++;
			else if (mes2 == '1') noviembre++;
			else if (mes2 == '2') diciembre++;
		}
	}

	int max_hashes = 40;
	int max_value = enero;
    	if (febrero > max_value) max_value = febrero;
    	if (marzo > max_value) max_value = marzo;
    	if (abril > max_value) max_value = abril;
    	if (mayo > max_value) max_value = mayo;
    	if (junio > max_value) max_value = junio;
    	if (julio > max_value) max_value = julio;
    	if (agosto > max_value) max_value = agosto;
    	if (septiembre > max_value) max_value = septiembre;
    	if (octubre > max_value) max_value = octubre;
    	if (noviembre > max_value) max_value = noviembre;
    	if (diciembre > max_value) max_value = diciembre;

	#define PRINT_MES(nombre, valor, valor2) \
        printf("\n" nombre " "); \
        for (int i = 0; i < (valor * max_hashes) / max_value; i++) { \
	   printf("#"); \
        }\
	printf(" [%d]",valor2);

    	// Imprimir los meses con los '#' escalados
    	PRINT_MES("Enero", enero, enero2);
    	PRINT_MES("Febrero", febrero, febrero2);
    	PRINT_MES("Marzo", marzo, marzo2);
    	PRINT_MES("Abril", abril, abril2);
    	PRINT_MES("Mayo", mayo, mayo2);
    	PRINT_MES("Junio", junio, junio2);
    	PRINT_MES("Julio", julio, julio2);
    	PRINT_MES("Agosto", agosto, agosto2);
    	PRINT_MES("Septiembre", septiembre, septiembre2);
    	PRINT_MES("Octubre", octubre, octubre2);
    	PRINT_MES("Noviembre", noviembre, noviembre2);
    	PRINT_MES("Diciembre", diciembre, diciembre2);
	printf("\n");
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void dias_mas_comandos(const int num_comandos, const int MAX_COMANDOS, char fechas_comandos[MAX_COMANDOS][MAX_LONGITUD])
{
	/* coger el ultimo valor
	* que es el mas alto y 
	* listo
	*/
	
	printf("\nDias con mas comandos\n");
	printf("Numero 1: %s\n", fechas_comandos[num_comandos-2]);
	printf("Numero 2: %s\n", fechas_comandos[num_comandos-4]);
	printf("Numero 3: %s\n", fechas_comandos[num_comandos-6]);
	printf("Numero 4: %s\n", fechas_comandos[num_comandos-8]);
	printf("Numero 5: %s\n", fechas_comandos[num_comandos-10]);

}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void comandos_mas_usados(const int num_suma_principales, const int MAX_COMANDOS_REPETICION, char suma_comandos_principales_ordenados[MAX_COMANDOS_REPETICION][MAX_LONGITUD])
{
	printf("COMANDOS MAS USADOS\n");
	printf("Enter para descubrir...\n");

	getchar();
	
	printf("Comando Numero 1: ");
	for (int i=0; i<3; i++)
	{
		printf(".");
		fflush(stdout);
		sleep(1);
	}
	printf("\n");
	printf(" %s\n",suma_comandos_principales_ordenados[num_suma_principales-2]);
	
	sleep(1);
	printf("Comando Numero 2: ");
	for (int i=0; i<3; i++)
	{
		printf(".");
		fflush(stdout);
		sleep(1);
	}
	printf("\n");
	printf(" %s\n",suma_comandos_principales_ordenados[num_suma_principales-4]);

	sleep(1);
	printf("Comando Numero 3: ");
	for (int i=0; i<3; i++)
	{
		printf(".");
		fflush(stdout);
		sleep(1);
	}
	printf("\n");
	printf(" %s\n",suma_comandos_principales_ordenados[num_suma_principales-6]);
	
	sleep(1);
	printf("Comando Numero 4: ");
	for (int i=0; i<3; i++)
	{
		printf(".");
		fflush(stdout);
		sleep(1);
	}
	printf("\n");
	printf(" %s\n",suma_comandos_principales_ordenados[num_suma_principales-8]);

	sleep(1);
	printf("Comando Numero 5: ");
	for (int i=0; i<3; i++)
	{
		printf(".");
		fflush(stdout);
		sleep(1);
	}
	printf("\n");
	printf(" %s\n",suma_comandos_principales_ordenados[num_suma_principales-10]);
	
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


	/*
	 * Para el hist_file
	 */

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

	/* 
	 * Repetimos todo para el hist_file_date 
	 */

	// Aqui puede error por max_comandos pero si se limpian antes los dos archivos y hay el mismo numero no pasa nada
	char comandos_date[MAX_COMANDOS][MAX_LONGITUD];
	int num_comandos_date = 0;

	obtener_comandos_date(MAX_COMANDOS, comandos_date, &num_comandos_date);

	// Sumar todos los comandos que se han hecho en un dia
	char comandos_date_suma[num_comandos_date*2][MAX_LONGITUD];
	int num_comandos_date_suma = 0;		// Tamaño de comandos_date_suma
	sumar_comandos_date(&num_comandos_date_suma, MAX_COMANDOS, &num_comandos_date, comandos_date, comandos_date_suma);

	// Ordenar comandos_date_suma
	int len = num_comandos_date_suma;
	char comandos_date_suma_ordenados[num_comandos_date_suma][MAX_LONGITUD];
	ordenar_cadena(len, comandos_date_suma, &len, comandos_date_suma_ordenados);

	/*
	 * Graficos
	 */

	titulo_inicio(num_comandos);
	meses_barras(num_comandos_date, MAX_COMANDOS, comandos_date);
	dias_mas_comandos(num_comandos_date_suma, num_comandos_date*2, comandos_date_suma);
	comandos_mas_usados(num_suma_principales, MAX_COMANDOS_REPETICION, suma_comandos_principales_ordenados);

	/* PRUEBAS DE SALIDA */

/*	for (int i = 0; i < num_suma_principales; i++) 
	{
        	printf("%s\n", suma_comandos_principales_ordenados[i]);
    	}

*/

	return 0;
}

