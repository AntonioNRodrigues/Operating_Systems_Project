#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int velocidade = 100;

char *nameFile = "pesca.bin";

int main(int argc, char const *argv[])
{
	processa_parametros_logPlayer(argc, argv);

	printf("velocidade:: %d\n", velocidade);
	printf("namefile:: %s\n", nameFile);
	return 0;
}

processa_parametros_logPlayer(int argc, char *argv[]){
	char opt;
	while ((opt = getopt (argc, argv, "l:v:")) != -1) {
		switch (opt) {
		case 'l':
			nameFile = strdup(optarg);
			break;
		case 'v':
			velocidade = atoi (optarg);
			break;
		default: /* '?' */
			fprintf (stderr, "Modo de utilização: %s [-l name_file] [-v velocidade] \n",
			         argv[0]);
			exit (EXIT_FAILURE);
		}
	}
}