#ifndef __PARAMETROS__
#define __PARAMETROS__

#include "posicao.h"

/**
 * Número de barcos de pesca.
 */
extern int num_barcos;

/**
 * Número de cardumes.
 */
extern int num_cardumes;

/**
 * Número de jornadas de pesca.
 */
extern int num_jornadas_pesca;

/**
 * Posição do cais no mundo.
 */
extern const Posicao posicao_cais;

#define DIMENSAO_MAR 10

#define TAMANHO_INICIAL_CARDUME 1000

#define CAPACIDADE_BARCO_PESCA 2000

#define CAPACIDADE_REDE_PESCA 1000

#define CRESCIMENTO_CARDUME 600

/**
 * Duração de uma jornada de pesca em segundos.
 */
#define DURACAO_JORNADA_PESCA 60

/**
 * Lê os parâmetros da simulação a partir da linha de comandos.
 */
void processa_parametros (int argc, char *argv[]);

/**
 * Lê os parâmetros da simulação a partir do ficheiro com o nome dado.
 */
void le_parametros (const char *nome_ficheiro);

#endif
