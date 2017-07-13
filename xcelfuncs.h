#ifndef __XCELFUNCS__
#define __XCELFUNCS__

void initialize (CSV_type *csv, SCR_type *scr, SELECT_type *mult_select);

//funcao para alocar matriz de strings em CSV_type
void allocate_matrix_buffer (SELECT_type *mult_select, int lin, int col);

void print_csv  (CSV_type *csv, SCR_type *scr, SELECT_type *mult_select);

#endif