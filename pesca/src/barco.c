#include <time.h>
#include <unistd.h>

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include "barco.h"
#include "mundo.h"
#include "cardume.h"
#include "parametros.h"
#include "posicao.h"

#include "semaphore.h"
#include "shared.h"
#include "util.h"

const char *EBS[] = {"CAS", "SAI", "DTN", "ALM", "PES", "REG", "DCR", "FIM"};

const char *OBS[] = {"NEN", "PES", "FES"};

Barco *barcos;

static OrdemBarco ordem_capitao (int id);
static void sair_cais (int id);
static bool chegou_a (int id, const Posicao *destino);
static void ir_para (int id, const Posicao *destino);
static bool hora_regressar_cais (int id);
static bool cardume_detectado (int id);
static void capturar_peixe (int bid);
static void navegar (int id);
static void entrar_cais (int id);

static void iniciar_barco (Barco *barco)
{
	barco->estado = B_CAIS;
	barco->ordem = O_NENHUMA;
	barco->destino = NIL_POS;
	barco->posicao = NIL_POS;
	barco->peixe_pescado = 0;
}

void iniciar_barcos ()
{
	/* Criar a memória partilhada para os barcos */
	barcos = sharedInit ("shm.pesca-barcos", num_barcos * sizeof (Barco));
	/* Inicializar a memória partilhada dos barcos */
	int i;
	for (i = 0; i < num_barcos; i++) {
		iniciar_barco (barcos + i);
	}
}

void destruir_barcos ()
{
	sharedDestroy ("shm.pesca-barcos", barcos, num_barcos * sizeof (Barco));
}

void imprimir_barco (FILE *ficheiro, const Barco *barco)
{
	fprintf (ficheiro, "%s %s ", EBS [barco->estado], OBS [barco->ordem]);
	if (barco->estado == B_DESTINO)
		fprintf (ficheiro, "%2d%2d", barco->destino.x, barco->destino.y);
	else
		fprintf (ficheiro, "----");
	fprintf (ficheiro, " ");
	if (barco->estado == B_DESTINO || barco->estado == B_PESCAR || barco->estado == B_ALTO_MAR || barco->estado == B_REGRESSAR)
		fprintf (ficheiro, "%2d%2d", barco->posicao.x, barco->posicao.y);
	else
		fprintf (ficheiro, "----");
	fprintf (ficheiro, " %5d", barco->peixe_pescado);
}

void main_barco (int id)
{
	bool fim = false;
	while (!fim) {
		switch (ordem_capitao (id)) {
		case O_PESCAR:
			sair_cais (id);
			while (!chegou_a (id, &barcos [id].destino)) {
				ir_para (id, &barcos [id].destino);
			}
			while (!hora_regressar_cais (id)) {
				if (cardume_detectado (id)) {
					capturar_peixe (id);
				}
				else {
					navegar (id);
				}
			}
			while (!chegou_a (id, &posicao_cais)) {
				ir_para (id, &posicao_cais);
			}
			entrar_cais (id);
			break;
		case O_FESTEJAR:
			fim = true;
			break;
		case O_NENHUMA:
			break;
		}
	}
}

OrdemBarco ordem_capitao (int id)
{

	return barcos [id].ordem;
}

void sair_cais (int id)
{

	barcos [id].estado = B_DESTINO;
	barcos [id].posicao = posicao_cais;
	mundo->mar [posicao_cais.x][posicao_cais.y].barco = id;
	mundo->barcos_cais--;
	imprimir_mundo ();

}

bool chegou_a (int id, const Posicao *destino)
{
	if ((barcos [id].posicao.x == destino->x) &&
		 (barcos [id].posicao.y == destino->y)) {
		return true;
	}
	else
		return false;
}

void ir_para (int id, const Posicao *destino)
{
	Posicao delta, np;
	usleep (1000000 + random () % 2000000);
	calcula_delta (&barcos [id].posicao, destino, &delta);

	somar_posicoes (&np, &barcos [id].posicao, &delta);
	if (mundo->mar [np.x][np.y].barco == VAZIO) {
		mundo->mar [barcos [id].posicao.x][barcos [id].posicao.y].barco = VAZIO;
		mundo->mar [np.x][np.y].barco = id;
		barcos [id].posicao = np;
		imprimir_mundo ();
	}

}

bool hora_regressar_cais (id)
{
	if ((barcos [id].peixe_pescado >= CAPACIDADE_BARCO_PESCA) ||
	    (time (NULL) > mundo->hora_regressar)) {

		barcos [id].estado = B_REGRESSAR;
		imprimir_mundo ();

		return true;
	}
	return false;
}

bool cardume_detectado (int bid)
{
	bool resultado = false;

	int k;
	for (k = 0; k < TV; k++) {
		Posicao np;
		int cid = -1;
		somar_posicoes (&np, &barcos [bid].posicao, &DELTA [k]);
		if (between (np.x, 0, DIMENSAO_MAR - 1) &&
		    between (np.y, 0, DIMENSAO_MAR - 1) &&
		    (barco_posicao_mundo (&np) == VAZIO) &&
		    ((cid = cardume_posicao_mundo (&np)) != VAZIO) &&
		    (cardumes [cid].estado == P_NADAR) &&
		    (cardumes [cid].tamanho > 0)) {
			cardumes [cid].estado = P_PESCADO;
			barcos [bid].estado = B_PESCAR;
			mover_barco_mundo (bid, &np);
			imprimir_mundo ();
			resultado = true;
			break;
		}
	}

	return resultado;
}

void capturar_peixe (int bid)
{
	usleep (1000000 + random () % 2000000);

	int cid = cardume_posicao_mundo (&barcos [bid].posicao);
	assert (cid != VAZIO);
	int peixe_pescado = min (cardumes [cid].tamanho, min (CAPACIDADE_REDE_PESCA, CAPACIDADE_BARCO_PESCA - barcos [bid].peixe_pescado));
	cardumes [cid].tamanho -= peixe_pescado;
	barcos [bid].peixe_pescado += peixe_pescado;

	cardumes [cid].estado = P_NADAR;
	imprimir_mundo ();

}

void navegar (int id)
{

	barcos [id].estado = B_ALTO_MAR;
	int i = random () % TV; /* escolher uma posição vizinha aleatória para começar a procurar uma posição vazia */
	int j = i + TV;
	while (i < j) {
		int k = i % TV;
		Posicao np;
		somar_posicoes (&np, &barcos [id].posicao, &DELTA [k]);
		if (between (np.x, 0, DIMENSAO_MAR - 1) &&
			 between (np.y, 0, DIMENSAO_MAR - 1) &&
			 (barco_posicao_mundo (&np) == VAZIO)) {
			mover_barco_mundo (id, &np);
			imprimir_mundo ();
			break;
		}
		i++;
	}

	usleep ((__useconds_t) (3800000 + random () % 1500000));
}

void entrar_cais (int id)
{

	barcos [id].estado = B_DESCARREGAR;
	barcos [id].ordem = O_NENHUMA;
	mundo->barcos_cais++;

	mundo->mar [barcos [id].posicao.x][barcos [id].posicao.y].barco = VAZIO;
	fifoPush (&mundo->espera_barcos, id);
	imprimir_mundo ();


}
