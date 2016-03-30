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

	printf("iniciar_cardumes::::\n");
/* Criar a memória partilhada para os cardumes */
	cardumes = (Cardume *) sharedInit("/cardume", sizeof (Cardume) * num_cardumes);
	
/* Inicializar a memória partilhada dos cardumes */
	int i;
	for (int i = 0; i < num_cardumes; i++){
		iniciar_cardume(&cardumes[i]);	
		printf("iniciar_cardumes:cicloFor:: %d :::sizeCardumes %d \n", cardumes[i].posicao.x, i+1);
	}

}

void destruir_cardumes (){
	sharedDestroy("/cardume", &cardumes, sizeof(Cardume)*num_cardumes);

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
	return false;
}

void apanhado_rede (int id)
{
}

void nadar (int id)
{
}

bool proximo_outro_cardume (int id)
{
	return false;
}

void reproduzir (int id)
{
}
