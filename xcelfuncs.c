#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "datastruct.h"
#include "formulas.h"

//funcao para alocar matriz de strings em CSV_type
void allocate_matrix_buffer (SELECT_type *mult_select, int lin, int col)
{
	int i, j;
	 
	// aloca um vetor de LIN ponteiros para linhas
	mult_select->buffer = (char***) malloc (lin * sizeof (char**)) ;
	 
	// aloca um vetor com todos os elementos da matriz
	mult_select->buffer[0] = (char**) malloc (lin * col * sizeof (char *)) ;
	 
	// ajusta os demais ponteiros de linhas (i > 0)
	for (i = 1; i < lin; i++)
		mult_select->buffer[i] = mult_select->buffer[0] + i * col;
	 
	// aloca uma string para cada elemento da matriz
	for (i = 0; i < lin; i++)
		for (j = 0; j < col; j++)
			mult_select->buffer[i][j] = (char *) malloc (256 * sizeof(char));

	mult_select->lin = lin;
	mult_select->col = col;
}


void initialize (CSV_type *csv, SCR_type *scr, SELECT_type *mult_select)
{
	initscr();
	clear();
	noecho();
	raw();
	keypad(stdscr, TRUE);
	curs_set(0);

	csv->begin.x = 0;
	csv->begin.y = 0;

	scr->cursor.x = 3;
	scr->cursor.y = 1;

	csv->cursor.x = 0;
	csv->cursor.y = 0;

	mult_select->begin.x = 0;
	mult_select->begin.y = 0;

	mult_select->end.x = 0;
	mult_select->end.y = 0;

	mult_select->state = IDLE;

	mult_select->buffer = NULL;

	scr->up = 1;
	scr->left = 3;
}

void print_csv (CSV_type *csv, SCR_type *scr, SELECT_type *mult_select)
{
	char str_calculated[256] = "";
	int csv_i, scr_i;
	int csv_j, scr_j;

	clear();

	getmaxyx (stdscr, scr->down, scr->right);
	scr->down -= 3;
	scr->right -= 3;


	attron(A_REVERSE);
	mvprintw (0, 0, "   ");

	//IMPRIME OS INDICES SUPERIORES
	for (csv_i = csv->begin.x, scr_i = 0;
	(scr_i < (scr->right/8) && (csv_i < 26));
	++csv_i, ++scr_i)
		mvprintw(0, (scr_i * 8) + 3, "   %c    ", csv_i + 65);


	//IMPRIME OS INDICES LATERAIS
	for (csv_j = csv->begin.y + 1, scr_j = 1;
	(scr_j < scr->down+1) && (csv_j <= 100);
	++csv_j, ++scr_j)
		mvprintw (scr_j, 0, "%*d", 3, csv_j);

	attroff(A_REVERSE);


	//IMPRIME A MATRIZ DE STRINGS
	for (int csv_i = csv->begin.y, scr_i = 1;
	(scr_i < scr->down+1) && (csv_i < 100);
	++csv_i, ++scr_i)
		for (int csv_j = csv->begin.x, scr_j = 0;
		(scr_j < (scr->right/8)) && (csv_j < 26);
		++csv_j, ++scr_j)
		{
			//VERIFICA SE É FORMULA E CALCULA
			calc_form (csv, csv->matrix[csv_i][csv_j], str_calculated);

			//IMPRIME A CELULA
			mvprintw (scr_i, scr_j * 8 + 3, "%.8s", str_calculated);


			//DESTAQUE ESPECIAL PARA A MULTI-SELEÇÃO
			if (mult_select->state != IDLE)
				if ((csv_i >= mult_select->begin.y) && (csv_i <= mult_select->end.y)) //SE A CELULA ESTA DENTRO DA MULTI-SELEÇAO (VERTICAL)
					if ((csv_j >= mult_select->begin.x) && (csv_j <= mult_select->end.x)) //SE A CELULA ESTA DENTRO DA MULTI-SELEÇAO (HORIZONTAL)
						mvchgat (scr_i, scr_j * 8 + 3, 8, A_REVERSE, 0, NULL); //DESTACA A CELULA
		}

	mvprintw (scr->down+2, 0, "Edit: %s", csv->matrix[csv->cursor.y][csv->cursor.x]);
	mvchgat (scr->down+2, 0, 5, A_REVERSE, 0, NULL);

	csv->end.x = csv_i;
	csv->end.y = csv_j;

	refresh();
}