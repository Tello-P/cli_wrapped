#include <stdio.h>
#include <stdlib.h>
#include <math.h>



void titulo_inicio(const int num_comandos)
{
	system("clear");
	
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


void meses_barras(const int num_comandos, char *fechas_comandos[][10])
{
	
	int enero=0, febrero=0, marzo=0, abril=0, mayo=0, junio=0, julio=0, agosto=0, septiembre=0, octubre=0, noviembre=0, diciembre=0;


	for (int i = 0; i < num_comandos; i++)
	{	

		char mes1 = fechas_comandos[i][0][5];  
		char mes2 = fechas_comandos[i][0][6];  

		if (mes1 == '0')
		{
			if (mes2 == '1') enero++;
			else if (mes2 == '2') febrero++;
			else if (mes2 == '3') marzo++;
			else if (mes2 == '4') abril++;
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

	#define PRINT_MES(nombre, valor) \
        printf("\n" nombre " "); \
        for (int i = 0; i < (valor * max_hashes) / max_value; i++) { \
	    printf("#"); \
        }\
	printf(" [%d]",valor);

    	// Imprimir los meses con los '#' escalados
    	PRINT_MES("Enero", enero);
    	PRINT_MES("Febrero", febrero);
    	PRINT_MES("Marzo", marzo);
    	PRINT_MES("Abril", abril);
    	PRINT_MES("Mayo", mayo);
    	PRINT_MES("Junio", junio);
    	PRINT_MES("Julio", julio);
    	PRINT_MES("Agosto", agosto);
    	PRINT_MES("Septiembre", septiembre);
    	PRINT_MES("Octubre", octubre);
    	PRINT_MES("Noviembre", noviembre);
    	PRINT_MES("Diciembre", diciembre);
	printf("\n");
}	



void graficos(const int num_comandos, char *fechas_comandos[][10])

{
	titulo_inicio(num_comandos);	
	meses_barras(num_comandos, fechas_comandos);
}

int main()
{

	int num_comandos = 2;
	char *fechas_comandos[][10] = {{"2025-01-4", "3"},{"2025-11-18", "8"},};

	graficos(num_comandos, fechas_comandos);
	

	return 0;
}
