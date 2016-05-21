#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <signal.h>

int counter = 0;
int velocidade = 100;

char *nameFile = "pesca.bin";

int main(int argc, char const *argv[])
{

	processa_parametros_logPlayer(argc, argv);
	// TESTES -->DELETE AFTER
	printf("velocidade:: %d\n", velocidade);
	printf("namefile:: %s\n", nameFile);

	instalar_rotina_atendimento_sinal_ ();
	
	while(1){
		sleep(5);
		printf("%s\n", "NO SIGINT");
	}

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
		default: 
			fprintf (stderr, "Modo de utilização: %s [-l name_file] [-v velocidade] \n",
			         argv[0]);
			exit (EXIT_FAILURE);
		}
	}
}
void kill_logPlayer(){
	if(counter < 2){
		printf("%s %d\n", "counter is runing", counter);	
		counter++;
	}else{
		exit(1);
	}

}
void instalar_rotina_atendimento_sinal_ (){
	struct sigaction action;
	action.sa_handler = kill_logPlayer;
	//sigemptyset (&action.sa_mask);
	action.sa_flags = 0;
	//action.si_timerid;
	if (sigaction (SIGINT, &action, NULL) != 0) {
		perror ("Erro ao instalar rotina de atendimento de sinal");
		exit (1);
	}
	

}