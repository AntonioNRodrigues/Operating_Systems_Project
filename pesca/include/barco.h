#ifndef __BARCO__
#define __BARCO__

#include <stdio.h>

#include "posicao.h"

/**
 * Enumerado que representa os possíveis estados de um barco de pesca.
 */
typedef enum {
	B_CAIS, B_SAIR, B_DESTINO, B_ALTO_MAR, B_PESCAR, B_REGRESSAR, B_DESCARREGAR, B_FIM
} EstadoBarco;

/**
 * Ordens que o capitão pode dar aos barcos de pesca.
 */
typedef enum {
	O_NENHUMA, O_PESCAR, O_FESTEJAR
} OrdemBarco;

/**
 * Atributos de um barco de pesca.
 */
typedef struct {
	/**
	 * Estado actual do barco.
	 */
	EstadoBarco estado;
	/**
	 * Ordem dada pelo capitão ao barco.
	 */
	OrdemBarco ordem;
	/**
	 * Posição no alto-mar para o barco deve dirigir-se.  Válido se o estado
	 * do barco for B_DESTINO.
	 */
	Posicao destino;
	/**
	 * Posição actual do barco no alto-mar.  Válido se o estado do barco for
	 * B_DESTINO, B_PESCAR, B_ALTO_MAR ou B_REGRESSAR.
	 */
	Posicao posicao;
	/**
	 * Quantidade de peixe pescado na jornada actual de pesca.
	 */
	int peixe_pescado;
} Barco;

/**
 * Vector para converter o estado de um barco numa cadeia de caracteres.
 */
extern const char *EBS[];

/**
 * Vector para converter ordem de um barco numa cadeia de caracteres.
 */
extern const char *OBS[];

/**
 * Vector com o estado dos barcos.
 */
extern Barco *barcos;

/**
 * Cria e inicia a memória partilhada que contém o estado dos barcos.
 */
void iniciar_barcos ();

/**
 * Destrói a memória partilhada que contém o estado dos barcos.
 */
void destruir_barcos ();

/**
 * Imprime uma linha de texto com uma descrição do estado do barco.
 */
void imprimir_barco (FILE *ficheiro, const Barco *barco);

/**
 * Ciclo de vida da entidade barco.
 */
void main_barco (int id);

#endif
