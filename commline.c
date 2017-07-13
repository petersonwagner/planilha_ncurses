#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
FUNCAO QUE RETORNA O NOME DO ARQUIVO A SER LIDO
PELA LINHA DE COMANDO
*/
void get_filename (char **argv, char *file_name)
{
	if (argv[1] == NULL)
		strcpy (file_name, "");
	else
		strcpy (file_name, argv[1]);
}