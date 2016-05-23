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
char *nameFile = "pesca.bin";
struct itimerval itv;
/*
* function to process the parametros
*/
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
/*
* function to terminate the program if the user sends a SIGINT 3 times 
* withing 10 seconds 
*/
void kill_logPlayer(int sig){

	// first ctrl-c 
	if(sig == SIGINT && counter == 0){
		counter ++;
		setTimer();
		printf("%s %d\n", "First turn counter is runing", counter);
	//3 ctrl-c	
	}else if(sig == SIGINT && counter == 2){
		printf("%s %d\n", " Exit with 3 Ctrl-c used withing 10 sec", counter);
		exit(-1);
	//
	}else if(sig == SIGINT && counter <= 1 ){
		counter++;
		printf("%s %d\n", "Ctrl-c used --> Counter is runing", counter);
	//	SIGALRM and the count is not 2
	}else if(sig == SIGALRM && counter < 3){
		counter = 0;
		printf("%s\n", " Timer expire ");
	}
}
/*
* function to initialize the timer to be generated only one time and to expire 
* within 10 secs
*/
void setTimer(){

	// the signal is generated only one time
	itv.it_interval.tv_sec = 0;  
	//This is the period between now and the first timer interrupt. 
	itv.it_value.tv_sec = 10; 

	setitimer (ITIMER_REAL, &itv, 0);
	
}
/*
* function to read the file and to print it to the cmd line
*/
void lerFile(){
		
	FILE *ficheiro;
	int numBarcos, numCardumes;
	struct timeval t0;
	struct timeval t1;
	Mundo *mundo = (Mundo *) malloc (sizeof(Mundo));
	Barco *barcos;
	Cardume *cardumes;
	
	ficheiro = fopen(nameFile, "r");
	if (ficheiro == NULL) {
		perror ("file does not exists");
		exit (1);
	}
	//read 4 bytes for the numBarcos
	fread(&numBarcos, 4, 1, ficheiro);
	//read 4 bytes for the numCardumes
	fread(&numCardumes, 4, 1, ficheiro);  
	
	printf("%d\n", numBarcos);
	printf("%d\n", numCardumes);
	t1.tv_sec = 0;

	barcos = (Barco *) malloc (sizeof(Barco) * numBarcos);
	cardumes = (Cardume *) malloc (sizeof(Cardume) * numCardumes);

	//read 8 bytes for the tempo
	while(fread(&t0, 8, 1, ficheiro)==1){

		printf("TEMPO %10ld \n", t0);
		
 		//read sizeof Mundo bytes for the Mundo
 		fread (&mundo, sizeof(mundo), 1, ficheiro);
 		printf("MUNDO %d \n", mundo);
 		
 		//read for each barco and fill the barcos[] 
 		for (int i = 0; i < numBarcos; i++){
			fread(&barcos[i], sizeof(Barco), 1, ficheiro);				
 			printf("BARCO %d %d\n", i, barcos[i].peixe_pescado);
 		}

 		//read for each barco and fill the cardumes[] 
 		for (int i = 0; i < numCardumes; i++){
 			fread(&cardumes[i], sizeof(Cardume), 1, ficheiro);
 			printf("CARDUME %d %d\n",  i, cardumes[i].tamanho);
 		
 		}
 	}
	 fclose(ficheiro);
	 exit(-1);
}

/*
* set the speed for giving the output
*/
int setSpeed(struct timeval t0, struct timeval t1){
	
	return (t0.tv_sec - t1.tv_sec)/velocidade;
}
/*
* main function
*/	
int main (int argc, char *argv[]){

	processa_parametros_logPlayer(argc, argv);

	signal(SIGINT, kill_logPlayer);

	signal(SIGALRM, kill_logPlayer);

	lerFile();

	return 0;
}