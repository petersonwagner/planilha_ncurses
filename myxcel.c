#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "csvfile.h"
#include "xcelfuncs.h"

int main(int argc, char *argv[])
{
	int character;
	bool exit = false;
	char file_name[20] = "";
	CSV_type csv;
	SCR_type scr;
	SELECT_type mult_select;

	mult_select.buffer = NULL;
	get_filename (argv, file_name);
	readcsv (&csv, file_name);
	initialize (&csv, &scr, &mult_select);
	print_csv (&csv, &scr, &mult_select);
	mvchgat (scr.cursor.y, scr.cursor.x, 8, A_REVERSE, 0, NULL); //CURSOR NA PRIMEIRA CELULA
	
	while (1)
	{
		character = getch();
		switch (character)
		{
			case KEY_UP:
				if (scr.cursor.y > scr.up) //SE CURSOR ESTA DENTRO DO LIMITE SUPERIOR DA TELA
				{
					chgat (8, A_NORMAL, 0, NULL);
				    scr.cursor.y--;
				    csv.cursor.y--;
				}
				else if (csv.begin.y > 0) //SE ESTA DENTRO DO LIMITE SUP DA MATRIZ
				{
					csv.begin.y--;
					csv.cursor.y--;
				}

				print_csv (&csv, &scr, &mult_select);
				break;


			case KEY_DOWN:
				//SE CURSOR ESTA DENTRO DO LIMITE INFERIOR DA TELA
				//E O CURSOR NAO ULTRAPASSA O TAMANHO VERTICAL QUE ESTA SENDO MOSTRADO NA TEL
				//(ESTA ULTIMA CONDIÇÃO PREVINE UM CASO ESPECIAL DE SEG FAULT DEPOIS DE REDIMENSIONAR A TELA)
				if ((scr.cursor.y < scr.down) && (scr.cursor.y < csv.end.y - csv.begin.y - 1))
				{
					chgat (8, A_NORMAL, 0, NULL);
				    scr.cursor.y++;
				    csv.cursor.y++;
				}
				else if (csv.end.y <= 100)  //SE ESTA DENTRO DO LIMITE INFERIOR DA MATRIZ
				{
					csv.begin.y++;
					csv.cursor.y++;
				}
				
				print_csv (&csv, &scr, &mult_select);
				break;


			case KEY_LEFT:
				if (scr.cursor.x > scr.left) //SE CURSOR ESTA DENTRO DO LIMITE ESQUERDO DA TELA
				{
					chgat (8, A_NORMAL, 0, NULL);
				    scr.cursor.x-=8;
				    csv.cursor.x--;
				}
				else if (csv.begin.x > 0) //SE ESTA DENTRO DO LIMITE ESQUERDO DA MATRIZ
				{
					csv.begin.x--;
					csv.cursor.x--;
				}

				print_csv (&csv, &scr, &mult_select);
			    break;


			case KEY_RIGHT:
				//SE CURSOR ESTA DENTRO DO LIMITE DIREITO DA TELA
				//E O CURSOR NAO ULTRAPASSA O TAMANHO HORIZONTAL QUE ESTA SENDO MOSTRADO NA TEL
				//(ESTA ULTIMA CONDIÇÃO PREVINE UM CASO ESPECIAL DE SEG FAULT DEPOIS DE REDIMENSIONAR A TELA)
				if ((scr.cursor.x < scr.right - 12) && (scr.cursor.x < (csv.end.x - csv.begin.x - 1) * 8))
				{
					chgat (8, A_NORMAL, 0, NULL);
				    scr.cursor.x+=8;
				    csv.cursor.x++;
				}
				else if (csv.end.x < 26)  //SE ESTA DENTRO DO LIMITE DIREITO DA MATRIZ
				{
					csv.begin.x++;
					csv.cursor.x++;
				}

				print_csv (&csv, &scr, &mult_select);
				break;


			case KEY_RESIZE:
				//VOLTA O CURSOR NA PRIMEIRA POSIÇÃO DA TELA (PARA FACILITAR) E ATUALIZA O COMEÇO E FIM DA TELA
				print_csv (&csv, &scr, &mult_select);
				csv.cursor = csv.begin;
				scr.cursor.x = scr.left;
				scr.cursor.y = scr.up;
				break;


			case ENTER:
				attron(A_REVERSE);
				move (scr.down+2, 0);
				clrtoeol(); //LIMPA A LINHA INTEIRA
				printw ("Edit:");
				attroff(A_REVERSE);
				
				echo();
			    mvscanw (scr.down+2, 6, "%s", csv.matrix[csv.cursor.y][csv.cursor.x]);
			    noecho();
			    print_csv (&csv, &scr, &mult_select);
				break;


			case CTRL_S:
				move (scr.down+2, 0);
				clrtoeol();
				attron (A_REVERSE);

				if (!strcmp (file_name, ""))
				{
					printw ("File name to save:");
					
					echo();
					mvscanw (scr.down+2, 19, "%s", file_name);
					noecho();
				}

				writecsv (&csv, file_name);
				clrtoeol();
				mvprintw (scr.down+2, 0, "File saved");
				attroff (A_REVERSE);
				break;


			case CTRL_R:
				move (scr.down+2, 0);
				clrtoeol();

				printw ("Open new file:");
				mvchgat (scr.down+2, 0, 14, A_REVERSE, 0, NULL);
				
				echo();
				mvscanw (scr.down+2, 15, "%s", file_name);
				noecho();

				//REINICIALIZA A APLICAÇÃO
				readcsv (&csv, file_name);
				initialize (&csv, &scr, &mult_select);
				print_csv (&csv, &scr, &mult_select);
				mvchgat (scr.cursor.y, scr.cursor.x, 8, A_REVERSE, 0, NULL);
				break;


			case CTRL_SPACE:
				if (mult_select.state == IDLE) //SE NAO ESTIVER EM MULTI-SELEÇÃO
				{
					mult_select.begin = csv.cursor;
					mult_select.end   = mult_select.begin;
					mult_select.state = SELECTING;
				}

				else if (mult_select.state == SELECTING) //SE ESTIVER SELECIONANDO
				{
					mult_select.end   = csv.cursor;
					mult_select.state = SELECTED;
				}

				else
					mult_select.state = IDLE;

				print_csv (&csv, &scr, &mult_select);
				break;

			case CTRL_C:
				if (mult_select.state == SELECTED)
					mult_select.state = IDLE;
				else if (mult_select.state == IDLE)
				{
					mult_select.begin = csv.cursor;
					mult_select.end   = csv.cursor;
					mult_select.state = SELECTED;
				}
				else //SELECTING
				{
					mult_select.end = csv.cursor;
					print_csv (&csv, &scr, &mult_select);
					mult_select.state = SELECTED;
				}

				mult_select.lin = mult_select.end.y - mult_select.begin.y + 1;
				mult_select.col = mult_select.end.x - mult_select.begin.x + 1;

				//ALOCA UM BUFFER PARA MULTI-SELEÇÃO
				allocate_matrix_buffer (&mult_select, mult_select.lin, mult_select.col);

				//COPIA O BLOCO DE CONTEUDO PARA O BUFFER DE MULTI-SELEÇÃO
				for (int i = 0; i < mult_select.lin; ++i)
					for (int j = 0; j < mult_select.col; ++j)
						strcpy (mult_select.buffer[i][j], csv.matrix[mult_select.begin.y + i][mult_select.begin.x + j]);

				move (scr.down+2, 0);
				clrtoeol();

				printw ("Copied");
				mvchgat (scr.down+2, 0, 6, A_REVERSE, 0, NULL);
				break;

			case CTRL_X:
				if (mult_select.state == SELECTED)
					mult_select.state = IDLE;
				else if (mult_select.state == IDLE)
				{
					mult_select.begin = csv.cursor;
					mult_select.end   = csv.cursor;
					mult_select.state = IDLE;
				}
				else //SELECTING
				{
					mult_select.end = csv.cursor;
					mult_select.state = SELECTED;
				}

				mult_select.lin = mult_select.end.y - mult_select.begin.y + 1;
				mult_select.col = mult_select.end.x - mult_select.begin.x + 1;

				//ALOCA UM BUFFER PARA MULTI-SELEÇÃO
				allocate_matrix_buffer (&mult_select, mult_select.lin, mult_select.col);

				//COPIA O BLOCO DE CONTEUDO PARA O BUFFER DE MULTI-SELEÇÃO E APAGA O QUE ESTAVA ANTERIORMENTE NA MATRIZ
				for (int i = 0; i < mult_select.lin; ++i)
					for (int j = 0; j < mult_select.col; ++j)
					{
						strcpy (mult_select.buffer[i][j], csv.matrix[mult_select.begin.y + i][mult_select.begin.x + j]);
						strcpy (csv.matrix[mult_select.begin.y + i][mult_select.begin.x + j], "");
					}

				print_csv (&csv, &scr, &mult_select);

				move (scr.down+2, 0);
				clrtoeol();

				printw ("Cut");
				mvchgat (scr.down+2, 0, 3, A_REVERSE, 0, NULL);
				break;

			case CTRL_V:
				mult_select.state = IDLE;

				//COPIA O BLOCO DE CONTEUDO DO BUFFER PARA A MATRIZ
				for (int i = 0; i < mult_select.lin && (csv.cursor.y + i < csv.lin); ++i)
					for (int j = 0; j < mult_select.col && (csv.cursor.x + j < csv.cols); ++j)
						strcpy (csv.matrix[csv.cursor.y + i][csv.cursor.x + j], mult_select.buffer[i][j]);

				print_csv (&csv, &scr, &mult_select);

				move (scr.down+2, 0);
				clrtoeol();

				printw ("Paste");
				mvchgat (scr.down+2, 0, 5, A_REVERSE, 0, NULL);
				break;

			case KEY_BACKSPACE:
			case KEY_DELETE:
				if (mult_select.state == SELECTED)
					mult_select.state = IDLE;
				else if (mult_select.state == IDLE)
				{
					mult_select.begin = csv.cursor;
					mult_select.end   = csv.cursor;
					mult_select.state = IDLE;
				}
				else //SELECTING
				{
					mult_select.end = csv.cursor;
					mult_select.state = SELECTED;
				}

				mult_select.lin = mult_select.end.y - mult_select.begin.y + 1;
				mult_select.col = mult_select.end.x - mult_select.begin.x + 1;


			    //APAGA O BLOCO DE CONTEUDO DA MATRIZ
				for (int i = 0; i < mult_select.lin; ++i)
					for (int j = 0; j < mult_select.col; ++j)
						strcpy (csv.matrix[mult_select.begin.y + i][mult_select.begin.x + j], "");

			    print_csv (&csv, &scr, &mult_select);

			    move (scr.down+2, 0);
				clrtoeol();
				printw ("Deleted");
				mvchgat (scr.down+2, 0, 7, A_REVERSE, 0, NULL);
				break;

			case CTRL_Q:
				exit = true;
				break;
		}

		if (exit)
			break;
		else
		{
		    mvchgat (scr.cursor.y, scr.cursor.x, 8, A_REVERSE, 0, NULL);
		    refresh();

			move (scr.cursor.y, scr.cursor.x);
		}

		
	}

	for (int i = 0; i < 100; ++i)
		for (int j = 0; j < 26; ++j)
			free (csv.matrix[i][j]);

	free (csv.matrix[0]) ;
	free (csv.matrix);

	refresh();
	endwin();
	return 0;
}