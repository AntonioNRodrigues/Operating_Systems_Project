#ifndef __POSICAO__
#define __POSICAO__

typedef struct {
	int x, y;
} Posicao;

/**
 * Tamanho da vizinhança.  Os barcos e os cardumes podem mover-se na
 * horizontal e na vertical.
 */
#define TV 4

/**
 * Vector que representa os movimentos que os cardumes e os barcos podem
 * fazer.  Somando a uma posição p os elementos deste vector, obtém-se as
 * posições vizinhas de p.
 */
extern const Posicao DELTA[TV];

/**
 * Representa uma posição inválida.
 */
extern const Posicao NIL_POS;

/**
 * Calcula o vector do array DELTA que mais se assemalha à subtração do
 * vector para com o vector de.  Pode ser usado para calcular em que
 * direcção se deve deslocar.
 */
void calcula_delta (const Posicao *de, const Posicao *para, Posicao *delta);

/**
 * Soma as posição a e b e coloca o resultado em resultado.
 */
void somar_posicoes (Posicao *resultado, const Posicao *a, const Posicao *b);

#endif
