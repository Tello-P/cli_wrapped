#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HISTORY_FILE "/home/tello/.zsh_history"
#define MAX_COMANDOS 100
#define MAX_LONGITUD 256  

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

int main()
{
	char comandos[MAX_COMANDOS][MAX_LONGITUD];
    	int num_comandos = 0;

    	obtener_comandos(comandos, &num_comandos);


    	for (int i = 0; i < num_comandos; i++) 
	{
        	printf("%d: %s\n", i + 1, comandos[i]);
    	}

    return 0;
}

