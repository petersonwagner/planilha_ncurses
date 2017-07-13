#include <stdbool.h>

#ifndef __DATASTRUCT__
#define __DATASTRUCT__

#define ENTER 10
#define CTRL_S 19
#define CTRL_R 18
#define CTRL_SPACE 0
#define CTRL_C 3
#define CTRL_X 24
#define CTRL_V 22
#define CTRL_Q 17
#define KEY_DELETE 330

//ESTADOS PARA O MULTI-SELECTION
#define IDLE 0
#define SELECTING 1
#define SELECTED 2

//STRUCT PARA COORDENADAS
typedef struct coordinates_type
{
	int y;
	int x;
} coordinates_type;

/*
SCR: SCREEN
NESSA STRUCT, GUARDO AS COORDENADAS DO CURSOR NA TELA
E OS LIMITES ATUAIS DA TELA
*/
typedef struct SCR_type
{
	int up;
	int down;
	int left;
	int right;
	coordinates_type cursor;
} SCR_type;

/*
NESSA STRUCT, GUARDO A MATRIZ DE STRINGS LIDAS DO CSV,
AS COORDENADAS DO CURSOR E DO PRIMEIRO E ULTIMO ELEMENTO
DA MATRIZ QUE ESTA SENDO MOSTRADA NA TELA (COORDENADAS DA MATRIZ, E NÃO DA TELA)
*/
typedef struct CSV_type
{
	int cols, lin;
	char ***matrix;
	coordinates_type begin, end, cursor;
} CSV_type;

/*
NESSA STRUCT, GUARDO UM BUFFER QUE PARA A COPIA EM MULTI-SELECAO
O ESTADO DA MULTI-SELECAO, O TAMANHO DESSA SELEÇÃO
E TAMBEM AS COORDENADAS DO PRIMEIRO E ULTIMO ELEMENTO DA SELECAO
*/
typedef struct SELECT_type
{
	char ***buffer;
	int state;
	int lin, col;
	coordinates_type begin, end;
} SELECT_type;

#endif