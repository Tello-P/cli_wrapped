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


void graficos(const int num_comandos)
{
	titulo_inicio(num_comandos);	

}

int main()
{

	int num_comandos = 24323;
	graficos(num_comandos);
	

	return 0;
}
