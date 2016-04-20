#ifndef __MUNDO__
#define __MUNDO__

#include <semaphore.h>
#include <time.h>

#include "capitao.h"
#include "parametros.h"

#include "posicao.h"
#include "fifo.h"

#define VAZIO -1

/**
 * Uma posição no mar pode ter um barco de pesca e/ou um cardume.
 */
typedef struct {
	/**
	 * Índice do barco de pesca presente na posição do mar.  Se não houver
	 * nenhum barco de pesca, contém o valor VAZIO.
	 */
	int barco;
	/**
	 * Índice do cardume presente na posição do mar.  Se não houver nenhum
	 * cardume, contém o valor VAZIO.
	 */
	int cardume;
} Mar;

/**
 * Representa o estado do mundo excepto o estado dos barcos e dos cardumes.
 */
typedef struct {
	/**
	 * Estado actual do capitão.
	 */
	EstadoCapitao estado_capitao;
	/**
	 * Matriz que representa o estado do mar.
	 */
	Mar mar[DIMENSAO_MAR][DIMENSAO_MAR];
	/**
	 * Hora de regresso da hora actual.
	 */
	time_t hora_regressar;
	/**
		* Fila de espera onde os barcos que chegam ao cais aguardam para
		* descarregar a sua carga e entregá-la ao capitão.
	 */
	FIFO espera_barcos;
	/**
	 * Quantos barcos estão no cais.
	 */
	int barcos_cais;
	/**
	 * Quantidade de peixe pescada.
	 */
	int peixes_cais;
	/**
	 * Quantas jornadas de pesca faltam.
	 */
	int jornadas_pesca;
} Mundo;

/**
 * Estado do mundo.
 */
extern Mundo *mundo;

/**
 * Semáforo para exclusão mútua no acesso à memória partilhada.
 */
extern sem_t *mutex;

/**
 * Semáforo para sincronização das actividades do capitão.
 */
extern sem_t *sem_capitao;

/**
 * Semáforo para sincronização da saída do cais.
 */
extern sem_t *sem_sair_cais;

/**
* Vector de semáforos para sincronização das actividades dos barcos.
 */
extern sem_t **sem_barcos;

/**
* Vector de semáforos usados na sincronização das actividades dos cardumes.
 */
extern sem_t **sem_cardumes;

/**
* Cria e inicia a memória partilhada que contem o estado do mundo (excepto o
* estado dos barcos e dos cardumes) e cria e inicia os semáforos.
 */
void iniciar_mundo ();

/**
 * Destrói a memória partilhada e os semáforos.
 */
void destruir_mundo ();

/**
* Devolve o identificador do barco que está na posição dada. Devolve VAZIO se
* não houver barco.
 */
int barco_posicao_mundo (const Posicao *p);

/**
* Devolve o identificador do cardume que está na posição dada. Devolve VAZIO se
* não houver cardume.
 */
int cardume_posicao_mundo (const Posicao *p);

/**
 * Move o barco com identificador id para a posição np.
 */
void mover_barco_mundo (int id, const Posicao *np);

/**
 * Move o cardume com identificador id para a posição np.
 */
void mover_cardume_mundo (int id, const Posicao *np);

/**
 * Imprime o estado do problema.  A impressão pode ir para um ficheiro de
 * log ou para o terminal.
 */
void imprimir_mundo ();

/**
 * Abre o ficheiro de log onde é escrito o estado do problema.
 */
void abrir_log_mundo ();

/**
 * Fecha o ficheiro de log onde é escrito o estado do problema.
 */
void fechar_log_mundo ();

#endif
