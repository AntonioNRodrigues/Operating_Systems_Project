#ifndef __CARDUME__
#define __CARDUME__

#include <stdio.h>

#include "posicao.h"

/**
 * Enumerado que representa os possíveis estados de um cardume.
 */
typedef enum {
	P_NADAR, P_REPRODUZIR, P_PESCADO, P_MORTO, P_FIM
} EstadoCardume;

/**
 * Atributos de um cardume.
 */
typedef struct {
	/**
	 * Estado actual do cardume.
	 */
	EstadoCardume estado;
	/**
	 * Posição em alto mar do cardume. Válido se o seu estado for P_NADAR,
	 * P_REPRODUZIR ou P_PESCADO.
	 */
	Posicao posicao;
	/**
	 * Quantidade de peixe no cardume.
	 */
	int tamanho;
} Cardume;

/**
 * Vector para converter o estado de um cardume numa cadeia de caracteres.
 */
extern const char *EPS[];

/**
 * Vector em memória partilhada com o estado dos vários cardumes.
 */
extern Cardume *cardumes;

/**
* Cria e inicia a memória partilhada que contém o estado dos cardumes.
 */
void iniciar_cardumes ();

/**
 * Destrói a memória partilhada.
 */
void destruir_cardumes ();

/**
 * Ciclo de vida da entidade cardume.
 */
void main_cardume (int id);

/**
 * Imprime uma linha de texto com uma descrição do estado do cardume.
 */
void imprimir_cardume (FILE *ficheiro, const Cardume *cardume);

#endif
