#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "capitao.h"
#include "barco.h"
#include "cardume.h"
#include "mundo.h"
#include "semaphore.h"


#define NUM_CAPITAO 1

static void criar_processos (pid_t *pid_filhos);
static void esperar_processos (const pid_t *pid_filhos);
static void instalar_rotina_atendimento_sinal ();
static bool tratar_saida_retorno_filho (int status);

int main (int argc, char *argv[])
{

	pid_t *pid_filhos;
	processa_parametros (argc, argv);
	/* criar memória dinâmica para pid_filhos */
	pid_filhos = (pid_t *) malloc(sizeof(pid_t) * (num_barcos+num_cardumes+NUM_CAPITAO));
	if(pid_filhos == NULL){
		exit(-1);
	}
	iniciar_mundo ();
	iniciar_barcos ();
	iniciar_cardumes ();
	abrir_log_mundo ();
	criar_processos (pid_filhos);
	instalar_rotina_atendimento_sinal ();

	printf ("Simulação arrancou\n");
	esperar_processos (pid_filhos);
	destruir_mundo ();
	destruir_barcos ();
	destruir_cardumes ();
	fechar_log_mundo ();
	/* libertar memória dinâmica de pid_filhos */
	free(pid_filhos);
	return 0;
}

void forkCalls(int start, int quant, pid_t *pid_filhos, char *name){
	int i, pid_filho;
	for (i = start; i < quant; i++){
		pid_filho = fork();
		if(pid_filho == -1)
			exit(-1);
		else{
			if (pid_filho == 0){
				if(strcmp(name, "barco"))
					main_barco(i);
				if(strcmp(name, "cardume"))
					main_cardume(i);
				else{
					main_capitao(i);
				}
				exit(0);
			}
			else{
				pid_filhos[i] = pid_filho;
				if (quant == NUM_CAPITAO+num_barcos+num_cardumes)
					printf("Lancou o processo filho (PID=%d) para o %s\n", pid_filhos[i], name);
				else if(start == 0)
					printf("Lancou o processo filho (PID=%d) para o %s %d\n", pid_filhos[i], name, i);
				else
					printf("Lancou o processo filho (PID=%d) para o %s %d\n", pid_filhos[i], name, i-num_barcos);				
				
			}
		}
	}
}

void criar_processos (pid_t *pid_filhos){
	/*lancar processos para os barcos*/
	forkCalls(0, num_barcos, pid_filhos, "barco");
	/*lancar processos para os cardumes*/
	forkCalls(num_barcos, num_cardumes+num_barcos, pid_filhos, "cardume");
	/*lancar processos para o capitao*/
	forkCalls(num_barcos+num_cardumes, num_cardumes+num_barcos+NUM_CAPITAO, pid_filhos, "capitao");
}

void esperar_processos (const pid_t *pid_filhos){
	int i, status, pid, terminados = 0;
	
	while(terminados < NUM_CAPITAO+num_barcos+num_cardumes){
		pid =  wait(&status);
		for(i=0; i<NUM_CAPITAO+num_cardumes+num_barcos; i++){
			if(pid == pid_filhos[i]){
				if(i<num_barcos)
					printf("Processo filho para (PID=%d) para o barco %d", pid, i%num_barcos);
				if(i>=num_barcos && i<num_cardumes+num_barcos)
					printf("Processo filho para (PID=%d) para o cardume %d", pid, i-num_barcos%num_cardumes);
				if(i>=num_cardumes+num_barcos)
					printf("Processo filho para (PID=%d) para o capitao", pid);
			}
		}
		if(tratar_saida_retorno_filho(status))
			terminados++;
	}
}

bool tratar_saida_retorno_filho (int status){
	bool terminou = false;
	if (WIFEXITED (status)) {
		printf (", terminou com status = %d\n", WEXITSTATUS(status));
		terminou = true;
	}
	else if (WIFSIGNALED (status)) {
		printf (", morto pelo sinal %d\n", WTERMSIG(status));
		terminou = true;
	}
	else if (WIFSTOPPED (status)) {
		printf (", parou pelo sinal %d\n", WSTOPSIG(status));
		terminou = false;
	}
	else if (WIFCONTINUED (status)) {
		printf (", continou\n");
		terminou = false;
	}
	return terminou;
}

void tratar_sinal (int sinal)
{
	printf ("Rotina de atendimento de sinal\n");
	printf("-----------%d\n", getpid());
}

void instalar_rotina_atendimento_sinal (){
	signal(SIGINT, tratar_sinal);
}
