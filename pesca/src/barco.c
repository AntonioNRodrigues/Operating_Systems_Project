#include <time.h>
#include <unistd.h>

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include <sys/mman.h>
#include <sys/stat.h>        
#include <fcntl.h>  

#include <unistd.h>
#include <sys/types.h>

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
	barcos = (Barco *) sharedInit("/barcos", sizeof(Barco) * num_barcos);
	/* Inicializar a memória partilhada dos barcos */
	int i;
	for (i = 0; i < num_barcos; i++){
		iniciar_barco(&barcos[i]);
	}

}

void destruir_barcos (){
	sharedDestroy("/barcos", barcos, sizeof(Barco)*num_barcos);
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
	bool fim = true;
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
}

bool chegou_a (int id, const Posicao *destino)
{
	return true;
}

void ir_para (int id, const Posicao *destino)
{
}

bool hora_regressar_cais (id)
{
	return true;
}

bool cardume_detectado (int bid)
{
	return false;
}

void capturar_peixe (int bid)
{
}

void navegar (int id)
{
}

void entrar_cais (int id)
{
}
