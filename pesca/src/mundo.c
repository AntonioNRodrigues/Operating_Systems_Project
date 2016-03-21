#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "mundo.h"
#include "barco.h"
#include "cardume.h"

#include "util.h"
#include "semaphore.h"
#include "shared.h"

Mundo *mundo;

void iniciar_mundo ()
{
	int i, x, y;
	/* Criar a memória partilhada para o mundo */
	//INSERIR CÓDIGO
	/* Inicializar a memória partilhada do mundo */
	mundo->estado_capitao = C_PLANEAR;
	for (x = 0; x < DIMENSAO_MAR; x++) {
		for (y = 0; y < DIMENSAO_MAR; y++) {
			mundo->mar [x][y].barco = VAZIO;
			mundo->mar [x][y].cardume = VAZIO;
		}
	}
	mundo->hora_regressar = 0;
	fifoInit (&mundo->espera_barcos);
	mundo->barcos_cais = num_barcos;
	mundo->peixes_cais = 0;
	mundo->jornadas_pesca = num_jornadas_pesca;
}

void destruir_mundo ()
{
	//INSERIR CÓDIGO
}

int barco_posicao_mundo (const Posicao *p)
{
	return mundo->mar [p->x][p->y].barco;
}

int cardume_posicao_mundo (const Posicao *p)
{
	return mundo->mar [p->x][p->y].cardume;
}

void mover_barco_mundo (int id, const Posicao *np)
{
	assert (between (np->x, 0, DIMENSAO_MAR - 1) && between (np->y, 0, DIMENSAO_MAR - 1));
	mundo->mar [barcos [id].posicao.x][barcos [id].posicao.y].barco = VAZIO;
	mundo->mar [np->x][np->y].barco = id;
	barcos [id].posicao = *np;
}

void mover_cardume_mundo (int id, const Posicao *np)
{
	assert (between (np->x, 0, DIMENSAO_MAR - 1) && between (np->y, 0, DIMENSAO_MAR - 1));
	mundo->mar [cardumes [id].posicao.x][cardumes [id].posicao.y].cardume = VAZIO;
	mundo->mar [np->x][np->y].cardume = id;
	cardumes [id].posicao = *np;
}

void abrir_log_mundo ()
{
}

void fechar_log_mundo ()
{
}

void imprimir_mundo ()
{
}
