#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "parametros.h"

int num_barcos = 6;

int num_cardumes = 20;

int num_jornadas_pesca = 4;

const Posicao posicao_cais = {0, 0};

void processa_parametros (int argc, char *argv[])
{
	char opt;
	while ((opt = getopt (argc, argv, "b:c:j:")) != -1) {
		switch (opt) {
		case 'b':
			num_barcos = atoi (optarg);
			break;
		case 'c':
			num_cardumes = atoi (optarg);
			break;
		case 'j':
			num_jornadas_pesca = atoi (optarg);
			break;
		default: /* '?' */
			fprintf (stderr, "Modo de utilização: %s [-b num_barcos] [-c num_cardumes] [-j num_jornadas_pesca]\n",
			         argv[0]);
			exit (EXIT_FAILURE);
		}
	}
}

void le_parametros (const char *nome_ficheiro)
{
	FILE *ficheiro = fopen (nome_ficheiro, "r");
	fscanf (ficheiro, "%d %d", &num_barcos, &num_cardumes);
	fclose (ficheiro);
}
