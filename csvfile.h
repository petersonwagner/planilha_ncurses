#ifndef __CSVFILE__
#define __CSVFILE__
#include "datastruct.h"
#include "commline.h"

//funcao para alocar matriz de strings em CSV_type
void allocate_matrix_buffer (SELECT_type *mult_select, int lin, int col);

//Funcao que le um arquivo csv em uma matriz de strings
void readcsv (CSV_type *csv, char *file_name);

//Funcao que escreve um arquivo csv lido de uma matriz de strings
//em uma stream com nome file_name
void writecsv (CSV_type *csv, char *file_name);

#endif