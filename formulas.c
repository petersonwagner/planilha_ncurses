#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "datastruct.h"

/*
	Função para retirar zeros a direita da virgula
	apenas porque isso é muito irritante
*/
void cut_zeros (char *string)
{
	int i;

	for (i = strlen(string) - 1; i >= 0; --i)
	{
		if (string[i] == '0')
			string[i] = '\0';
		else if (string[i] == '.')
		{
			string[i] = '\0';
			break;
		}
		else
			break;
	}
}

/**

	Esta funcao recebe uma string (str) e devolve
	outra string (str_calculated) no terceiro parametros
	com a formula pronta.
	Se o primeiro elemento da string for um '=',
		então é feito o cálculo
	Se não, str_calculated recebe a própria str.

	A função fmemopen retorna uma stream usando
	uma string como entrada. Usei esta função para facilitar
	a leitura ao poder usar a função scanf e fseek.
	(Talvez tenha um método muito melhor de fazer isso, sorry about that)

	Usei recursão para calcular formulas que dependem
	de outras formulas (como no exemplo da soma e média de notas).

	Esta funcao resolve formulas dos tipos:
	- numero op numero
	- numero op formula
	- formula op numero
	- formula op formula

	Para formulas incompletas, a função retorna '0' ou uma string vazia

 */

void calc_form (CSV_type *csv, char *str, char *str_calculated)
{
	FILE *stream;
	coordinates_type ref1, ref2;
	float num1 = 0, num2 = 0, result = 0;
	char operator, ch;

	strcpy (str_calculated, "");

	if (str[0] == '=') //SE STRING COMEÇAR COM '=', É UMA FORMULA
	{
		stream = fmemopen (str, strlen(str), "r");

		fscanf(stream, "=%c", &ch);
		
		if (ch == '\0') //VERIFICA SE A FORMULA ESTA INCOMPLETA
			return;

		if (ch >= '0' && ch <= '9')
		{
			fseek (stream, -1, SEEK_CUR);
			fscanf (stream, "%f", &num1);
		}
		else
		{
			fseek(stream, -1, SEEK_CUR);

			fscanf(stream, "%c", &ch);
			ref1.x = ch - 65;
			if (ref1.x < 0 || ref1.x > 26) //VERIFICA SE INDICES SAO INVALIDOS
				return;

			fscanf(stream, "%d", &ref1.y);
			if (ref1.y < 1 || ref1.y > 100 || ref1.y == '\0') //VERIFICA SE INDICES SAO INVALIDOS OU FORMULA INCOMPLETA
				return;
			ref1.y--;


			if (csv->matrix[ref1.y][ref1.x][0] == '=')
			{
				calc_form (csv, csv->matrix[ref1.y][ref1.x], str_calculated); //RECURSAO PARA FORMULAS QUE DEPENDEM DE OUTRAS FORMULAS
				num1 = atof (str_calculated);
			}
			else
				num1 = atof (csv->matrix[ref1.y][ref1.x]);


		}

		fscanf(stream, "%c", &operator);
		if (operator == '\0')  //VERIFICA SE FORMULA ESTA INCOMPLETA
			return;

		fscanf(stream, "%c", &ch);
		if (ch == '\0')  //VERIFICA SE FORMULA ESTA INCOMPLETA
			return;

		if (ch >= '0' && ch <= '9')
		{
			fseek (stream, -1, SEEK_CUR);
			fscanf (stream, "%f", &num2);
		}
		else
		{
			fseek(stream, -1, SEEK_CUR);
			fscanf(stream, "%c", &ch);
			ref2.x = ch - 65;
			if (ref2.x < 0 || ref2.x > 26)  //VERIFICA SE INDICES SAO INVALIDOS
				return;

			fscanf(stream, "%d", &ref2.y);
			if (ref2.y < 1 || ref2.y > 100 || ref2.y == '\0')  //VERIFICA SE INDICES SAO INVALIDOS OU FORMULA INCOMPLETA
				return;
			ref2.y--;

			if (csv->matrix[ref2.y][ref2.x][0] == '=')
			{
				calc_form (csv, csv->matrix[ref2.y][ref2.x], str_calculated); //RECURSAO PARA FORMULAS QUE DEPENDEM DE OUTRAS FORMULAS
				num2 = atof (str_calculated);
			}
			else
				num2 = atof (csv->matrix[ref2.y][ref2.x]);

		}

		fclose (stream);

		//CALCULO ARITMETICO DOS VALORES OBTIDOS
		switch (operator)
		{
			case '+':
				result = num1 + num2;
				break;
			
			case '-':
			    result = num1 - num2;
				break;

			case '/':
				if (num2 != 0)
			    	result = num1 / num2;
			    else
			    	result = 0;
				break;

			case '*':
			    result = num1 * num2;
				break;

			default: //SE OPERACAO FOR INVALIDA
				result = 0;
				break;
		}

		sprintf (str_calculated, "%f", result); //JOGA A STRING RESULT COMO FLOAT EM STR_CALCULATED
		cut_zeros (str_calculated); //TIRA OS ZEROS DESNECESSARIOS DPS DA VIRGULA
	}

	else //SE STRING NÃO COMEÇA COM '=', JOGA ELA MESMA EM STR_CALCULATED
		strcpy (str_calculated, str);

}