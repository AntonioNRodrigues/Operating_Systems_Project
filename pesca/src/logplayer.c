#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <signal.h>

#include <time.h>

#include "mundo.h"
#include "barco.h"
#include "cardume.h"

int counter = 0;
int velocidade = 100;
struct itimerval itv;
char *nameFile = "pesca-2.bin";

/*int main(int argc, char const *argv[])
{

	processa_parametros_logPlayer(argc, argv);
	// TESTES -->DELETE AFTER
	printf("velocidade:: %d\n", velocidade);
	printf("namefile:: %s\n", nameFile);

	//configTimer();
	//instalar_rotina_atendimento_sinal_ ();
	lerFile();
	
	while(1){
		sleep(5);
		printf("%s\n", "NO SIGINT");
	}

	return 0;
}*/
void configTimer(){
	itv.it_interval.tv_sec = 5; // 

	itv.it_value.tv_sec = 10; //

	setitimer (ITIMER_REAL, &itv, 0);
	
}
void processa_parametros_logPlayer(int argc, char *argv[]){

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

void lerFile(){
	
	FILE *ficheiro;
	int numBarcos, numCardumes;
	struct timeval  tempo;
	Mundo *mundo = (Mundo *) malloc (sizeof(Mundo));
	Barco *barcos;
	Cardume *cardumes;
	
	ficheiro = fopen(nameFile, "r");
	if (ficheiro == NULL) {
		perror ("cria_file()");
		exit (1);
	}
	//read 4 bytes for the numBarcos
	fread(&numBarcos, 4, 1, ficheiro);
	//read 4 bytes for the numcardumes
	fread(&numCardumes, 4, 1, ficheiro);  
	
	printf("%d\n", numBarcos);
	printf("%d\n", numCardumes);
	
	barcos = (Barco *) malloc (sizeof(Barco) * numBarcos);
	cardumes = (Cardume *) malloc (sizeof(Cardume) * numCardumes);
	
		//build a loop until the end of file---------------------------

	//read 8 bytes for the tempo
	fread(&tempo, 8, 1, ficheiro);
  	printf("%d \n", tempo);
 	
 	//read sizeof Mundo bytes for the Mundo
 	fread (&mundo, sizeof(mundo), 1, ficheiro);
 	printf("%d \n", mundo);

 	//read for each barco and fill the barcos[] 
 	for (int i = 0; i < numBarcos; i++){
 		fread(&barcos+sizeof(Barco), sizeof(Barco), 1, ficheiro);
 	printf("%d\n", barcos[i]);
 	}

 	//read for each barco the fill the cardumes[] 
 	for (int i = 0; i < numCardumes; i++){
 		fread(&cardumes+sizeof(Cardume), sizeof(Cardume), 1, ficheiro);
 		printf("%d\n",  cardumes[i]);
 	}
}
	
