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
	pid_filhos = malloc (sizeof (pid_t) * (num_barcos + num_cardumes + 1));
	iniciar_mundo ();
	iniciar_barcos ();
	iniciar_cardumes ();
	abrir_log_mundo ();
	criar_processos (pid_filhos);
	instalar_rotina_atendimento_sinal ();
	/* arranque da simulacao */
	semUp ("pesca-mutex", mutex);
	printf ("Simulação arrancou\n");
	esperar_processos (pid_filhos);
	destruir_mundo ();
	destruir_barcos ();
	destruir_cardumes ();
	fechar_log_mundo ();
	free (pid_filhos);
	return 0;
}

void criar_processos (pid_t *pid_filhos)
{
	int i;
	/* lançar os processos barcos */
	for (i = 0; i < num_barcos; i++) {
		pid_filhos [i] = fork ();
		switch (pid_filhos [i]) {
		case -1:
			perror ("Erro ao criar o processo filho para um barco");
			exit (1);
		case 0:
			main_barco (i);
			exit (0) ;
		default:
			printf ("Lancou o processo filho (PID=%d) para o barco %d\n", pid_filhos [i], i);
		}
	}
	/* lançar os processos cardumes */
	for (i = 0; i < num_cardumes; i++) {
		pid_filhos [num_barcos + i] = fork ();
		switch (pid_filhos [num_barcos + i]) {
		case -1:
			perror ("Erro ao criar o processo filho para um cardume");
			exit (1);
		case 0:
			main_cardume (i);
			exit (0) ;
		default:
			printf ("Lancou o processo filho (PID=%d) para o cardume %d\n", pid_filhos [num_barcos + i], i);
		}
	}
	/* lançar o processo capitão */
	pid_filhos [num_barcos + num_cardumes] = fork ();
	switch (pid_filhos [num_barcos + num_cardumes]) {
	case -1:
		perror ("Erro ao criar o processo filho para o capitão");
		exit (1);
	case 0:
		main_capitao ();
		exit (0) ;
	default:
		printf ("Lancou o processo filho (PID=%d) para o capitão\n", pid_filhos [num_barcos + num_cardumes]);
	}
}

void esperar_processos (const pid_t *pid_filhos)
{
	int i;
	int terminados = 0;
	while (terminados < num_barcos + num_cardumes + 1) {
		pid_t filho;
		int status;
		bool terminou;
		filho = wait (&status);
		if (filho == -1) {
			perror ("Erro ao esperar por um processo filho");
			exit (0);
		}
		else if (filho == pid_filhos [num_barcos + num_cardumes]) {
			printf ("Processo filho para o capitão");
			terminou = tratar_saida_retorno_filho (status);
		}
		else {
			bool achou = false;
			for (i = 0; i < num_barcos; i++) {
				if (filho == pid_filhos [i]) {
					printf ("Processo filho para o barco %d", i);
					terminou = tratar_saida_retorno_filho (status);
					achou = true;
					break;
				}
			}
			if (!achou) {
				for (i = 0; i < num_cardumes; i++) {
					if (filho == pid_filhos [num_barcos + i]) {
						printf ("Processo filho para o cardume %d", i);
						terminou = tratar_saida_retorno_filho (status);
						break;
					}
				}
			}
		}
		if (terminou)
			terminados++;
	}
}

bool tratar_saida_retorno_filho (int status)
{
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
	destruir_mundo ();
	destruir_barcos ();
	destruir_cardumes ();
	fechar_log_mundo ();
	exit (0);
}

void instalar_rotina_atendimento_sinal ()
{
	struct sigaction action;
	action.sa_handler = tratar_sinal;
	sigemptyset (&action.sa_mask);
	action.sa_flags = 0;
	if (sigaction (SIGQUIT, &action, NULL) != 0) {
		perror ("Erro ao instalar rotina de atendimento de sinal");
		exit (1);
	}
	if (sigaction (SIGINT, &action, NULL) != 0) {
		perror ("Erro ao instalar rotina de atendimento de sinal");
		exit (1);
	}
	if (sigaction (SIGTERM, &action, NULL) != 0) {
		perror ("Erro ao instalar rotina de atendimento de sinal");
		exit (1);
	}
}
