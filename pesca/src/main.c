#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "capitao.h"
#include "barco.h"
#include "cardume.h"
#include "mundo.h"
#include "semaphore.h"

static void criar_processos (pid_t *pid_filhos);
static void esperar_processos (const pid_t *pid_filhos);
static void instalar_rotina_atendimento_sinal ();
static bool tratar_saida_retorno_filho (int status);

int main (int argc, char *argv[])
{
	pid_t *pid_filhos;
	processa_parametros (argc, argv);
	/* criar memória dinâmica para pid_filhos */
	pid_filhos = (pid_t *) malloc(sizeof(pid_t) * num_barcos+num_cardumes+1);

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

void forkCalls(int quant, pid_t *pid_filhos, char *name){
	int i, pid_filho;
	for (i = 0; i < quant; i++){
		pid_filho = fork();
		if(pid_filho == -1)
			exit(-1);
		else{
			if (pid_filho == 0)
				exit(0);
			else{
				pid_filhos[i] = pid_filho;
				quant == 1 ? 
					printf("Lancou o processo filho (PID=%d) para o %s\n", pid_filhos[i], name):
					printf("Lancou o processo filho (PID=%d) para o %s %d\n", pid_filhos[i], name, i);				
			}
		}
	}
}
void criar_processos (pid_t *pid_filhos){
	/*lancar processos para os barcos*/
	forkCalls(num_barcos, pid_filhos, "barco");
	/*lancar processos para os cardumes*/
	forkCalls(num_cardumes, pid_filhos, "cardume");
	/*lancar processos para o capitao*/
	forkCalls(1, pid_filhos, "capitao");
}

void esperar_processos (const pid_t *pid_filhos)
{
	int i, status;
	int terminados = 0;
	for (i=0; i<num_barcos+num_cardumes+1; i++){
		waitpid(pid_filhos[i], &status, 0);
		printf("Processo filho para o cardume %d, %s\n", i, tratar_saida_retorno_filho(status));

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
	//INSERIR CÓDIGO
}

void instalar_rotina_atendimento_sinal ()
{
	//INSERIR CÓDIGO
}
