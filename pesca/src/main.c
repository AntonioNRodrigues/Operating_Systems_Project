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
	//INSERIR CÓDIGO
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
	//INSERIR CÓDIGO
	return 0;
}

void criar_processos (pid_t *pid_filhos)
{
	int i;
	/* lançar os processos barcos */
	//INSERIR CÓDIGO
	/* lançar os processos cardumes */
	//INSERIR CÓDIGO
	/* lançar o processo capitão */
	//INSERIR CÓDIGO
}

void esperar_processos (const pid_t *pid_filhos)
{
	int i;
	int terminados = 0;
	//INSERIR CÓDIGO
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
	//INSERIR CÓDIGO
}

void instalar_rotina_atendimento_sinal ()
{
	//INSERIR CÓDIGO
}
