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
	
	int en=0, fe=0, mz=0, ab=0, my=0, jn=0, jl=0, ag=0, sp=0, oc=0, no=0, di=0;


	for (int i = 0; i < num_comandos; i++)
	{	

		char mes1 = fechas_comandos[i][0][5];  
		char mes2 = fechas_comandos[i][0][6];  

		if (mes1 == '0')
		{
			if (mes2 == '1') en++;
			else if (mes2 == '2') fe++;
			else if (mes2 == '3') mz++;
			else if (mes2 == '4') ab++;
			else if (mes2 == '5') my++;
			else if (mes2 == '6') jn++;
			else if (mes2 == '7') jl++;
			else if (mes2 == '8') ag++;
			else if (mes2 == '9') sp++;
		}
		else if (mes1 == '1')
		{
			if (mes2 == '0') oc++;
			else if (mes2 == '1') no++;
			else if (mes2 == '2') di++;
		}
	}

}	



void graficos(const int num_comandos, char *fechas_comandos[][10])

{
	titulo_inicio(num_comandos);	
	meses_barras(num_comandos, fechas_comandos);
}

int main()
{

	int num_comandos = 2;
	char *fechas_comandos[][10] = {{"2025-11-4", "3"},{"2025-11-18", "8"},};

	graficos(num_comandos, fechas_comandos);
	

	return 0;
}
