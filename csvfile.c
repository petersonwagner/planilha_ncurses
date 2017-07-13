#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "commline.h"
#include "datastruct.h"

#define LIN 100
#define COL 26
#define MAXLENGTH 256

//funcao para alocar matriz de strings em CSV_type
void allocate_matrix_csv (CSV_type *csv, int lin, int col)
{
	int i, j;
	 
	// aloca um vetor de LIN ponteiros para linhas
	csv->matrix = (char***) malloc (lin * sizeof (char**)) ;
	 
	// aloca um vetor com todos os elementos da matriz
	csv->matrix[0] = (char**) malloc (lin * col * sizeof (char *)) ;
	 
	// ajusta os demais ponteiros de linhas (i > 0)
	for (i = 1; i < lin; i++)
		csv->matrix[i] = csv->matrix[0] + i * col;
	 
	// aloca uma string para cada elemento da matriz
	for (i = 0; i < lin; i++)
		for (j = 0; j < col; j++)
			csv->matrix[i][j] = (char *) malloc (256 * sizeof(char));
}

//Funcao que le um arquivo csv em uma matriz de strings
void readcsv (CSV_type *csv, char *file_name)
{
	FILE *stream;
	char *buffer, c;
	unsigned int i = 0, j = 0, row = 0, col = 0;
	int QuotationOnOff = 0; //0 - off, 1 - on

	buffer = (char*) malloc (MAXLENGTH);
	allocate_matrix_csv (csv, LIN, COL);

	for (i = 0; i < LIN; i++)
			for (j = 0; j < COL; j++)
				strcpy (csv->matrix[i][j], "");

	stream = fopen (file_name, "r");

	if (stream != NULL)
	{
		i = 0;
		//le um caractere enquanto nao chegou no fim da stream
		//e enquanto não houver linhas > 26 e colunas > 100
		while (( (c = fgetc(stream)) != EOF) && row < LIN && col < COL)
		{
			switch(c)
			{
				case ',':
					if(!QuotationOnOff)
					{
						buffer[i] = '\0'; //fim da string anterior
						strcpy (csv->matrix[row][col], buffer); //joga a string anterior no seu lugar
						strcpy (buffer, ""); //zera o buffer
						i = 0; //zera o indice para começar leitura de outra string
						col++;
					}
					else
						buffer[i++] = c;

					break;

				case '\n':
					buffer[i] = '\0'; //fim da string anterior
					strcpy (csv->matrix[row][col], buffer); //joga a string anterior no seu lugar
					strcpy (buffer, ""); //zera o buffer
					i = 0; //zera o indice para começar leitura de outra string
					col = 0; //zera o indice de colunas para começar leitura de outra linha
					row++;
					break;

				case '\"':
					if(!QuotationOnOff)
						QuotationOnOff = 1;
					else
						QuotationOnOff = 0;

					break;

				default:
					buffer[i++] = c;
					break;
			}
		}
		fclose (stream);
	}

	csv->lin = LIN;
	csv->cols = COL;
}

//Funcao que retorna 1 se a string passada tiver virgula
//e 0 caso contrario
int searchComma (char *string)
{
	int i;
	
	for (i = 0; i < strlen (string); ++i)
		if (string[i] == ',')
			return 1;

	return 0;
}


//Funcao que escreve um arquivo csv lido de uma matriz de strings
//em uma stream com nome file_name
void writecsv (CSV_type *csv, char *file_name)
{
	FILE *stream;
	bool found;
	int lin, col;
	int i, j;

	stream = fopen (file_name, "w+"); //MODO DE ESCRITA QUE APAGA O CONTEUDO ANTERIOR

	if (stream != NULL)
	{
		//ACHA A ULTIMA LINHA QUE FOI PREENCHIDA
		found = false;
		for (i = 99; i >= 0 && !found; --i)
			for (j = 0; j < 26 && !found; ++j)
				if (strcmp (csv->matrix[i][j], "")) //SE STRING NAO VAZIA
				{
					lin = i;
					found = true; //FLAG DE PARADA
				}

		//ACHA A ULTIMA COLUNA QUE FOI PREENCHIDA
		found = false;
		for (i = 0; i < 100 && !found; ++i)
			for (j = 25; j >= 0 && !found; --j)
				if (strcmp (csv->matrix[i][j], "")) //SE STRING NAO VAZIA
				{
					col = j;
					found = true; //FLAG DE PARADA
				}

		for (i = 0; i <= lin; ++i)
		{
			for (j = 0; j < col; ++j)
			{
				if (searchComma (csv->matrix[i][j])) //CASO A STRING CONTENHA UMA VIRGULA, PRINTA ENTRE ASPAS
					fprintf(stream, "\"%s\",", csv->matrix[i][j]);
				else
					fprintf(stream, "%s,", csv->matrix[i][j]);
			}

			//PRINTA A ULTIMA COLUNA DE CADA LINHA SEM UMA VIRGULA NO FINAL
			if (searchComma (csv->matrix[i][j]))
				fprintf(stream, "\"%s\"\n", csv->matrix[i][j]);
			else
				fprintf(stream, "%s\n", csv->matrix[i][j]);
		}

		fprintf(stream, "\n");

		fclose (stream);
	}
}