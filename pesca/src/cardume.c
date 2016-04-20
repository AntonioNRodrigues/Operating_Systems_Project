#include <unistd.h>

#include <stdbool.h>
#include <stdlib.h>

#include "cardume.h"
#include "parametros.h"
#include "mundo.h"

#include "semaphore.h"
#include "shared.h"
#include "util.h"

const char *EPS[] = {"NAD", "REP", "PES", "MRT", "FIM"};

Cardume *cardumes;

static bool esta_vivo (int id);
static void apanhado_rede (int id);
static void nadar (int id);
static bool proximo_outro_cardume (int id);
static void reproduzir (int id);

static void iniciar_cardume (Cardume *cardume)
{
	int x, y;
	cardume->estado = P_NADAR;
	cardume->tamanho = TAMANHO_INICIAL_CARDUME;
	do {
		x = random () % DIMENSAO_MAR;
		y = random () % DIMENSAO_MAR;
	} while (mundo->mar [x][y].cardume != VAZIO);
	cardume->posicao.x = x;
	cardume->posicao.y = y;
}

void iniciar_cardumes ()
{
	/* Criar a memória partilhada para os cardumes */
	cardumes = sharedInit ("shm.pesca-cardumes", num_cardumes * sizeof (Cardume));
	/* Inicializar a memória partilhada dos cardumes */
	int i;
	for (i = 0; i < num_cardumes; i++) {
		iniciar_cardume (cardumes + i);
		mundo->mar [cardumes [i].posicao.x][cardumes [i].posicao.y].cardume = i;
	}
}

void destruir_cardumes ()
{
	sharedDestroy ("shm.pesca-cardumes", cardumes, num_cardumes * sizeof (Cardume));
}

void imprimir_cardume (FILE *ficheiro, const Cardume *cardume)
{
	fprintf (ficheiro, "%s ", EPS [cardume->estado]);
	if (cardume->estado == P_PESCADO || cardume->estado == P_REPRODUZIR || cardume->estado == P_NADAR)
		fprintf (ficheiro, "%2d%2d", cardume->posicao.x, cardume->posicao.y);
	else
		fprintf (ficheiro, "----");
	fprintf (ficheiro, " %5d", cardume->tamanho);
}

void main_cardume (int id)
{
	while (esta_vivo (id)) {
		apanhado_rede (id);
		nadar (id);
		if (proximo_outro_cardume (id)) {
			reproduzir (id);
		}
	}
}

bool esta_vivo (int id)
{
	bool resultado = true;
	if (cardumes [id].tamanho <= 0) {

		cardumes [id].estado = P_MORTO;
		mundo->mar [cardumes [id].posicao.x][cardumes [id].posicao.y].cardume = VAZIO;
		imprimir_mundo ();

		resultado = false;
	}
	else if (mundo->jornadas_pesca == 0) {

		cardumes [id].estado = P_FIM;
		imprimir_mundo ();

		resultado = false;
	}
	return resultado;
}

void apanhado_rede (int id)
{
	if (cardumes [id].estado == P_PESCADO) {

	}
}

void nadar (int id)
{

	if (cardumes [id].estado == P_PESCADO) {

		return ;
	}
	cardumes [id].estado = P_NADAR;
	int i = random () % TV; /* escolher uma posição vizinha aleatória para começar a procurar uma posição vazia */
	int j = i + TV;
	while (i < j) {
		int k = i % TV;
		Posicao np;
		somar_posicoes (&np, &cardumes [id].posicao, &DELTA [k]);
		if (between (np.x, 0, DIMENSAO_MAR - 1) &&
			 between (np.y, 0, DIMENSAO_MAR - 1) &&
			 (mundo->mar [np.x][np.y].cardume == VAZIO)) {
			mover_cardume_mundo (id, &np);
			imprimir_mundo ();
			break;
		}
		i++;
	}

	usleep ((__useconds_t) (1200000 + random () % 1500000));
}

bool proximo_outro_cardume (int id)
{

	if (cardumes [id].estado == P_PESCADO) {

		return false;
	}
	bool resultado = false;
	int k;
	for (k = 0; k < 4; k++) {
		int x = cardumes [id].posicao.x + DELTA [k].x;
		int y = cardumes [id].posicao.y + DELTA [k].y;
		if (between (x, 0, DIMENSAO_MAR - 1) &&
			 between (y, 0, DIMENSAO_MAR - 1) &&
			 (mundo->mar [x][y].cardume != VAZIO)) {
			resultado = true;
			break;
		}
	}
	if (resultado) {
		cardumes [id].estado = P_REPRODUZIR;
		imprimir_mundo ();
	}

	return resultado;
}

void reproduzir (int id)
{

	if (cardumes [id].estado == P_PESCADO) {

		return ;
	}
	cardumes [id].tamanho += CRESCIMENTO_CARDUME;
	imprimir_mundo ();

	usleep ((__useconds_t) (5000000 + random () % 15000000));
}
