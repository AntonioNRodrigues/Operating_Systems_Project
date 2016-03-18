#ifndef __POSICAO__
#define __POSICAO__

/**
 * Posição num espaço a duas dimensões.
 */
typedef struct {
	int x, y;
} Posicao;

/**
 * Tamanho da vizinhança.
 */
#define TV 4

/**
 * Vector que representa os movimentos que se podem fazer na vertical e na
 * horizontal.  Somando a uma posição p os elementos deste vector, obtém-se as
 * posições vizinhas de p.
 */
extern const Posicao DELTA[TV];

extern const Posicao NIL_POS;

void calcula_delta (const Posicao *de, const Posicao *para, Posicao *delta);

void somar_posicoes (Posicao *resultado, const Posicao *a, const Posicao *b);

#endif
