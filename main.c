#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define RED     "\x1b[31m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"

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
	//system("clear");
	
        printf("\033[H\033[J"); // Limpia la pantalla en terminal
	if (num_comandos == 0)
	{
		perror("No hay comandos usados todavia");
		exit(1);
	}

	printf(BLUE"%d",num_comandos);
	printf("   ____ _     ___  __        ______      _    ____  ____  _____ ____  %d\n",num_comandos);
	printf("%d",num_comandos);
	printf("  / ___| |   |_ _| \\ \\      / /  _ \\    / \\  |  _ \\|  _ \\| ____|  _ \\ %d\n",num_comandos);
	printf("%d",num_comandos);
        printf(" | |   | |    | |   \\ \\ /\\ / /| |_) |  / _ \\ | |_) | |_) |  _| | | | |%d\n",num_comandos);
	printf("%d",num_comandos);
        printf(" | |___| |___ | |    \\ V  V / |  _ <  / ___ \\|  __/|  __/| |___| |_| |%d\n",num_comandos);
	printf("%d",num_comandos);
        printf("  \\____|_____|___|    \\_/\\_/  |_| \\_\\/_/   \\_\\_|   |_|   |_____|____/ %d\n", num_comandos);
	printf("%d                                                                      %d\n\n" RESET, num_comandos, num_comandos);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void titulo_meses_barras()
{
	printf(BLUE "  _____ ___ __  __ _____      _     ___ _   _ _____ \n");
	printf(" |_   _|_ _|  \\/  | ____|    | |   |_ _| \\ | | ____|\n");
    	printf("   | |  | || |\\/| |  _|      | |    | ||  \\| |  _|  \n");
    	printf("   | |  | || |  | | |___     | |___ | || |\\  | |___ \n");
    	printf("   |_| |___|_|  |_|_____|    |_____|___|_| \\_|_____|\n");
    	printf("                                                  \n\n" RESET);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void titulo_comandos_mas_usados()
{
	printf(BLUE "  __  __    _    ____     _   _ ____    _    ____   ___   ____  \n");
    	printf(" |  \\/  |  / \\  / ___|   | | | / ___|  / \\  |  _ \\ / _ \\ / ___| \n");
    	printf(" | |\\/| | / _ \\ \\___ \\   | | | \\___ \\ / _ \\ | | | | | | \\___ \\ \n");
    	printf(" | |  | |/ ___ \\ ___) |  | |_| |___) / ___ \\| |_| | |_| |___) |\n");
    	printf(" |_|  |_/_/   \\_\\____/   \\____/|____/_/   \\_\\____/ \\___/|____/ \n");
	printf("                                                                \n\n" RESET);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void titulo_combinacion_comandos_mas_usados()
{
	printf(BLUE "   ____ ___  __  __ ____ ___ _   _    _    ____ ___ ___  _   _ _____ ____  \n");
    	printf("  / ___/ _ \\|  \\/  | __ )_ _| \\ | |  / \\  / ___|_ _/ _ \\| \\ | | ____/ ___| \n");
    	printf(" | |  | | | | |\\/| |  _ \\| ||  \\| | / _ \\| |    | | | | |  \\| |  _| \\___ \\ \n");
    	printf(" | |__| |_| | |  | | |_) | || |\\  |/ ___ \\ |___ | | |_| | |\\  | |___ ___) |\n");
    	printf("  \\____\\___/|_|  |_|____/___|_| \\_/_/   \\_\\____|___\\___/|_| \\_|_____|____/ \n");
    	printf("                                                                            \n\n" RESET);
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void titulo_top_3_comandos()
{
      	printf("──────────────────────────────────────────────────────────────────────────────\n");
	printf(BLUE"  _____             _____                                       _           \n");
    	printf(" |_   _|__  _ __   |___ /    ___ ___  _ __ ___   __ _ _ __   __| | ___  ___ \n");
    	printf("   | |/ _ \\| '_ \\    |_ \\   / __/ _ \\| '_ ` _ \\ / _` | '_ \\ / _` |/ _ \\ __|\n");
    	printf("   | | (_) | |_) |  ___) | | (_| (_) | | | | | | (_| | | | | (_| | (_) \\__ \\\n");
    	printf("   |_|\\___/| .__/  |____/   \\___\\___/|_| |_| |_|\\__,_|_| |_|\\__,_|\\___/|___/\n");
    	printf("           |_|                                                               \n\n"RESET);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void titulo_top_3_combinaciones()
{
      	printf("\n──────────────────────────────────────────────────────────────────────────────\n");
	printf(BLUE"  _____             _____                       _               \n");
    	printf(" |_   _|__  _ __   |___ /    ___ ___  _ __ ___ | |__   ___  ___ \n");
    	printf("   | |/ _ \\| '_ \\    |_ \\   / __/ _ \\| '_ ` _ \\| '_ \\ / _ \\ __|\n");
    	printf("   | | (_) | |_) |  ___) | | (_| (_) | | | | | | |_) | (_) \\__ \\\n");
    	printf("   |_|\\___/| .__/  |____/   \\___\\___/|_| |_| |_|_.__/ \\___/|___/\n");
    	printf("           |_|                                                   \n\n"RESET);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void meses_barras(const int num_comandos,const int MAX_COMANDOS, char fechas_comandos[MAX_COMANDOS][MAX_LONGITUD])
{
	
	//system("clear");
        printf("\033[H\033[J"); // Limpia la pantalla en terminal
	titulo_meses_barras();
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
		else
		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		{febrero++;febrero2++;};	// esta linea puede dar algun error pero va bien de momento
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
	   printf(CYAN"█"RESET); \
        }\
	printf(YELLOW" [%d]"RESET,valor2);

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

    printf(BLUE"\n┌────────────────────────────────────┐\n");
    	printf("│                                    │\n"RESET);
    printf(BLUE"│"RESET);
    	printf("       Dias con mas comandos        ");
    					 printf(BLUE"│\n"RESET);
    	printf(BLUE"│       		             │\n");	
	printf("└────────────────────────────────────┘\n\n"RESET);
	printf("Nº 1:"CYAN" %s "RESET"|"YELLOW" %s "RESET"veces\n", fechas_comandos[num_comandos-2],fechas_comandos[num_comandos-1]);
	printf("Nº 2:"CYAN" %s "RESET"|"YELLOW" %s "RESET"veces\n", fechas_comandos[num_comandos-4],fechas_comandos[num_comandos-3]);
	printf("Nº 3:"CYAN" %s "RESET"|"YELLOW" %s "RESET"veces\n", fechas_comandos[num_comandos-6],fechas_comandos[num_comandos-5]);
	printf("Nº 4:"CYAN" %s "RESET"|"YELLOW" %s "RESET"veces\n", fechas_comandos[num_comandos-8],fechas_comandos[num_comandos-7]);
	printf("Nº 5:"CYAN" %s "RESET"|"YELLOW" %s "RESET"veces\n", fechas_comandos[num_comandos-10],fechas_comandos[num_comandos-9]);

}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void comandos_mas_usados(const int num_suma_principales, const int MAX_COMANDOS_REPETICION, char suma_comandos_principales_ordenados[MAX_COMANDOS_REPETICION][MAX_LONGITUD])
{
	//system("clear");
        printf("\033[H\033[J"); // Limpia la pantalla en terminal
	titulo_comandos_mas_usados();
	printf(RED"\n\nEnter"RESET" para descubrir...\n");

	getchar();
	
	for (int i=0; i<5; i++)
	{
      	      	printf(BLUE"─────────────"RESET);
		fflush(stdout);
		sleep(1);
	}

	printf("\n\nComando Nº 1: ");
      printf("\n─────────────\n");
	printf(CYAN" %s\n"RESET,suma_comandos_principales_ordenados[num_suma_principales-2]);
	printf(" [usado"YELLOW" %s"RESET" veces]\n",suma_comandos_principales_ordenados[num_suma_principales-1]);
	
	sleep(2);
	printf("\nComando Nº 2: ");
      printf("\n─────────────\n");
	printf(CYAN" %s\n"RESET,suma_comandos_principales_ordenados[num_suma_principales-4]);
	printf(" [usado "YELLOW"%s "RESET"veces]\n",suma_comandos_principales_ordenados[num_suma_principales-3]);

	sleep(2);
	printf("\nComando Nº 3: ");
      printf("\n─────────────\n");
	printf(CYAN" %s\n"RESET,suma_comandos_principales_ordenados[num_suma_principales-6]);
	printf(" [usado"YELLOW" %s"RESET" veces]\n",suma_comandos_principales_ordenados[num_suma_principales-5]);
	
	sleep(2);
	printf("\nComando Nº 4: ");
      printf("\n─────────────\n");
	printf(CYAN" %s\n"RESET,suma_comandos_principales_ordenados[num_suma_principales-8]);
	printf(" [usado"YELLOW" %s"RESET" veces]\n",suma_comandos_principales_ordenados[num_suma_principales-7]);

	sleep(2);
	printf("\nComando Nº 5: ");
      printf("\n─────────────\n");
	printf(CYAN" %s\n"RESET,suma_comandos_principales_ordenados[num_suma_principales-10]);
	printf(" [usado"YELLOW" %s"RESET" veces]\n",suma_comandos_principales_ordenados[num_suma_principales-9]);
	
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void combinacion_comandos_mas_usados(const int num_repetidos, char comandos_ordenados[num_repetidos][MAX_LONGITUD])
{
	//system("clear");
        printf("\033[H\033[J"); // Limpia la pantalla en terminal
	titulo_combinacion_comandos_mas_usados();
	printf(RED"\n\nEnter"RESET" para descubrir...\n");

	getchar();
	
	for (int i=0; i<6; i++)
	{
      	      	printf(BLUE"─────────────"RESET);
		fflush(stdout);
		sleep(1);
	}

	printf("\n\nComando Numero 1: ");
      printf("\n─────────────\n");
	printf(CYAN" %s\n"RESET,comandos_ordenados[num_repetidos-2]);
	printf(" [usado "YELLOW"%s"RESET" veces]\n",comandos_ordenados[num_repetidos-1]);
	
	sleep(2);
	printf("\nComando Numero 2: ");
      printf("\n─────────────\n");
	printf(CYAN" %s\n"RESET,comandos_ordenados[num_repetidos-4]);
	printf(" [usado "YELLOW"%s"RESET" veces]\n",comandos_ordenados[num_repetidos-3]);

	sleep(2);
	printf("\nComando Numero 3: ");
      printf("\n─────────────\n");
	printf(CYAN" %s\n"RESET,comandos_ordenados[num_repetidos-6]);
	printf(" [usado "YELLOW"%s"RESET" veces]\n",comandos_ordenados[num_repetidos-5]);
	
	sleep(2);
	printf("\nComando Numero 4: ");
      printf("\n─────────────\n");
	printf(CYAN" %s\n"RESET,comandos_ordenados[num_repetidos-8]);
	printf(" [usado "YELLOW"%s"RESET" veces]\n",comandos_ordenados[num_repetidos-7]);
	
	sleep(2);
	printf("\nComando Numero 5: ");
      printf("\n─────────────\n");
	printf(CYAN" %s\n"RESET,comandos_ordenados[num_repetidos-10]);
	printf(" [usado "YELLOW"%s"RESET" veces]\n",comandos_ordenados[num_repetidos-9]);

}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void resumen(const int num_comandos_date, const int num_suma_principales, const int MAX_COMANDOS_REPETICION, char suma_comandos_principales_ordenados[MAX_COMANDOS_REPETICION][MAX_LONGITUD], const int num_repetidos, char comandos_ordenados[num_repetidos][MAX_LONGITUD])
{
	//system("clear");
        printf("\033[H\033[J"); // Limpia la pantalla en terminal

	titulo_inicio(num_comandos_date);

	titulo_top_3_comandos();
	
  	printf("       ┌──────────────────────────────────────┐         \n");
    	printf("       │                Nº1                   │  "CYAN"%s"RESET" [usado "YELLOW"%s"RESET" veces]  \n",suma_comandos_principales_ordenados[num_suma_principales - 2], suma_comandos_principales_ordenados[num_suma_principales - 1]);
    	printf("       └──────────────────────────────────────┘		\n");
    	printf("       ┌─────────────────────────────┐            	\n");
    	printf("       │             Nº2             │   "CYAN"%s"RESET" [usado "YELLOW"%s"RESET" veces]  \n", suma_comandos_principales_ordenados[num_suma_principales - 4], suma_comandos_principales_ordenados[num_suma_principales - 3]);
    	printf("       └─────────────────────────────┘			\n");
	printf("       ┌─────────────────────┐             		\n");
    	printf("       │         Nº3         │    "CYAN"%s"RESET" [usado "YELLOW"%s"RESET" veces]  \n", suma_comandos_principales_ordenados[num_suma_principales - 6], suma_comandos_principales_ordenados[num_suma_principales - 5]);
    	printf("       └─────────────────────┘				\n");
	


	titulo_top_3_combinaciones();

	printf("       ┌──────────────────────────────────────┐         \n");
    	printf("       │                Nº1                   │  "CYAN"%s"RESET" [usado "YELLOW"%s"RESET" veces]  \n",comandos_ordenados[num_repetidos-2], comandos_ordenados[num_repetidos-1]);

	printf("       └──────────────────────────────────────┘		\n");
    	printf("       ┌─────────────────────────────┐            	\n");
    	printf("       │             Nº2             │   "CYAN"%s"RESET" [usado "YELLOW"%s"RESET" veces]  \n", comandos_ordenados[num_repetidos - 4], comandos_ordenados[num_repetidos - 3]);
    	printf("       └─────────────────────────────┘			\n");
	printf("       ┌─────────────────────┐             		\n");
    	printf("       │         Nº3         │    "CYAN"%s"RESET" [usado "YELLOW"%s"RESET" veces]  \n", comandos_ordenados[num_repetidos - 6], comandos_ordenados[num_repetidos - 5]);
    	printf("       └─────────────────────┘				\n");
	
      	printf("\n──────────────────────────────────────────────────────────────────────────────");



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
	 * Comandos para el hist_file
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
	 * Comandos para el hist_file_date 
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
	 * Comandos para los Graficos
	 */

	/*
	titulo_inicio(num_comandos_date);
	meses_barras(num_comandos_date, MAX_COMANDOS, comandos_date);
	dias_mas_comandos(num_comandos_date_suma, num_comandos_date*2, comandos_date_suma);
	comandos_mas_usados(num_suma_principales, MAX_COMANDOS_REPETICION, suma_comandos_principales_ordenados);
	combinacion_comandos_mas_usados(num_repetidos, comandos_ordenados);
	resumen(num_comandos_date, num_suma_principales, MAX_COMANDOS_REPETICION, suma_comandos_principales_ordenados, num_repetidos, comandos_ordenados);
	*/


	/*
	 * Menu incial
	 */

    	int indice = 0;
    	int key;
	while (1)
	{
        	printf("\033[H\033[J"); // Limpia la pantalla en terminal
	
        	// Llamar a la función según el índice
        	if (indice == 0) titulo_inicio(num_comandos_date);
        	if (indice == 1) {
        	    meses_barras(num_comandos_date, MAX_COMANDOS, comandos_date);
        	    dias_mas_comandos(num_comandos_date_suma, num_comandos_date * 2, comandos_date_suma);
        	}
        	if (indice == 2) comandos_mas_usados(num_suma_principales, MAX_COMANDOS_REPETICION, suma_comandos_principales_ordenados);
        	if (indice == 3) combinacion_comandos_mas_usados(num_repetidos, comandos_ordenados);
        	if (indice == 4) resumen(num_comandos_date, num_suma_principales, MAX_COMANDOS_REPETICION, suma_comandos_principales_ordenados, num_repetidos, comandos_ordenados);

        	printf("\n\nPresiona "RED"'s'"RESET" para avanzar, "RED"'a'"RESET" para retroceder, "RED"'q'"RESET" para salir...\n");

        	key = getchar();
        	while (getchar() != '\n'); // Limpiar buffer

        	if (key == 'q') 
		{
			
        		printf("\033[H\033[J"); // Limpia la pantalla en terminal
			break;
		}
        	if (key == 's' && indice < 4) indice++;
        	if (key == 'a' && indice > 0) indice--;
    	}


	return 0;
}

