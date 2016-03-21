#include <unistd.h>
#include <stdlib.h>

#include "capitao.h"
#include "mundo.h"
#include "barco.h"

#include "semaphore.h"

const char *ECS[] = {"PLA", "AGU", "REC", "FIM"};

static void selecionar_barcos ();
static void esperar_barcos ();
static void receber_captura_peixe ();
static void partilhar_lucros ();

void main_capitao ()
{
	while (mundo->jornadas_pesca > 0) {
		selecionar_barcos ();
		do {
			esperar_barcos ();
			receber_captura_peixe ();
		} while (mundo->barcos_cais < num_barcos);
		mundo->jornadas_pesca--;
	}
	partilhar_lucros ();
}

void selecionar_barcos ()
{
}

void esperar_barcos ()
{
}

void receber_captura_peixe ()
{
}

void partilhar_lucros ()
{
}
