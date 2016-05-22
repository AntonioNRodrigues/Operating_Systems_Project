#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "mundo.h"
#include "barco.h"
#include "cardume.h"
#include "parametros.h"

#include "util.h"
#include "semaphore.h"
#include "shared.h"

#include <sys/time.h>

Mundo *mundo;

sem_t *mutex;

sem_t *sem_capitao;

sem_t *sem_sair_cais;

sem_t **sem_barcos;

sem_t **sem_cardumes;

FILE *log_mundo;

void iniciar_mundo ()
{
	int i, x, y;
	/* Criar a memória partilhada para o mundo */
	mundo = sharedInit ("shm.pesca-mundo", sizeof (Mundo));
	/* Inicializar a memória partilhada do mundo */
	mundo->estado_capitao = C_PLANEAR;
	for (x = 0; x < DIMENSAO_MAR; x++) {
		for (y = 0; y < DIMENSAO_MAR; y++) {
			mundo->mar [x][y].barco = VAZIO;
			mundo->mar [x][y].cardume = VAZIO;
		}
	}
	mundo->hora_regressar = 0;
	fifoInit (&mundo->espera_barcos);
	mundo->barcos_cais = num_barcos;
	mundo->peixes_cais = 0;
	mundo->jornadas_pesca = num_jornadas_pesca;
	/* Criar e inicializar os semáforos */
	char *nome;
	int max_digitos;

    
    // ... semaforo mutex...
    mutex = semInit ("mutex", 1);
    
    
    // ... semaforo da saida do cais...
    sem_sair_cais = semInit ("sair_cais", 1);

    
    //... semaforo do capitao...
    sem_capitao = semInit ("capitao", 0);
    

	/* ... semaforos dos barcos... */
	max_digitos = log (num_barcos) / log (10) + 1;
	nome = malloc (strlen ("pesca-barco") + max_digitos + 1);
    
    //
    sem_barcos = malloc( sizeof(sem_t *)*num_barcos);
    
    for (i=0; i<num_barcos; i++) {
        sprintf (nome, "pesca-barco%0*d", max_digitos, i);
        printf("semInit: %s\n", nome);
        sem_barcos[i]=semInit (nome, 0);
    }
    //
    
	free (nome);
	
    /* .. semaforos dos cardumes... */
	max_digitos = log (num_cardumes) / log (10) + 1;
	nome = malloc (strlen ("pesca-cardume") + max_digitos + 1);
    
    //
    sem_cardumes = malloc( sizeof(sem_t *)*num_cardumes);
    
    for (i=0; i<num_cardumes; i++) {
        sprintf (nome, "pesca-cardume%0*d", max_digitos, i);
        printf("semInit: %s\n", nome);
        sem_cardumes[i]=semInit (nome, 0);
    }
    //
    
	free (nome);
    
    
}

void destruir_mundo ()
{
	int i;
	char *nome;
	int max_digitos;
	sharedDestroy ("shm.pesca-mundo", mundo, sizeof (Mundo));

	max_digitos = log (num_barcos) / log (10) + 1;
	nome = malloc (strlen ("pesca-barco") + max_digitos + 1);

    //
    for (i=0; i<num_barcos; i++) {
        sprintf (nome, "pesca-barco%0*d", max_digitos, i);
        printf("semDestroy: %s\n", nome);
        semDestroy(nome, sem_barcos[i]);
    }
    //
    
	free (nome);

	max_digitos = log (num_cardumes) / log (10) + 1;
	nome = malloc (strlen ("pesca-cardume") + max_digitos + 1);

    //
    for (i=0; i<num_cardumes; i++) {
        sprintf (nome, "pesca-cardume%0*d", max_digitos, i);
        printf("semDestroy: %s\n", nome);
        semDestroy(nome, sem_cardumes[i]);
    }
    //
    
	free (nome);
    
    //
    printf("semDestroy: capitao\n");
    semDestroy("capitao", sem_capitao);
    
    printf("semDestroy: sair_cais\n");
    semDestroy("sair_cais", sem_sair_cais);
    
    printf("semDestroy: mutex\n");
    semDestroy("mutex", mutex);
    //


}

int barco_posicao_mundo (const Posicao *p)
{
	return mundo->mar [p->x][p->y].barco;
}

int cardume_posicao_mundo (const Posicao *p)
{
	return mundo->mar [p->x][p->y].cardume;
}

void mover_barco_mundo (int id, const Posicao *np)
{
	assert (between (np->x, 0, DIMENSAO_MAR - 1) && between (np->y, 0, DIMENSAO_MAR - 1));
	mundo->mar [barcos [id].posicao.x][barcos [id].posicao.y].barco = VAZIO;
	mundo->mar [np->x][np->y].barco = id;
	barcos [id].posicao = *np;
}

void mover_cardume_mundo (int id, const Posicao *np)
{
	assert (between (np->x, 0, DIMENSAO_MAR - 1) && between (np->y, 0, DIMENSAO_MAR - 1));
	mundo->mar [cardumes [id].posicao.x][cardumes [id].posicao.y].cardume = VAZIO;
	mundo->mar [np->x][np->y].cardume = id;
	cardumes [id].posicao = *np;
}

void _abrir_log_mundo ()
{
	int i;
	log_mundo = fopen ("pesca.log", "w");
	fprintf (log_mundo, "tempo      ECp Peixe Bc Jn Hora      ");
	for (i = 0; i < num_barcos; i++) {
		fprintf (log_mundo, " EBc OBc Dest Posi Peixe");
	}
	fprintf (log_mundo, " ");
	for (i = 0; i < num_cardumes; i++) {
		fprintf (log_mundo, " ECd Posi Taman");
	}
	fprintf (log_mundo, "\n");
	fflush (log_mundo);
}

void abrir_log_mundo ()
{
    log_mundo = fopen (nome_ficheiro, "w");
    
    if(fwrite(&num_barcos, sizeof(num_barcos), 1, log_mundo)!=1){
     perror("Escrita para ficheiro de dados");
     exit(1);
     }
    
    if(fwrite(&num_cardumes, sizeof(num_cardumes), 1, log_mundo)!=1){
        perror("Escrita para ficheiro de dados");
        exit(1);
    }
    fflush (log_mundo);
}

void fechar_log_mundo ()
{
	fclose (log_mundo);
}

static void _imprimir_log_mundo ()
{
	int i;
  	fprintf (log_mundo, "%10ld", time (NULL));
	fprintf (log_mundo, " %s %5d %2d %2d ", ECS [mundo->estado_capitao], mundo->peixes_cais, mundo->barcos_cais, mundo->jornadas_pesca);
	if (mundo->estado_capitao == C_RECEBER || mundo->estado_capitao == C_AGUARDAR)
		fprintf (log_mundo, "%10ld", mundo->hora_regressar);
	else
		fprintf (log_mundo, "          ");
	for (i = 0; i < num_barcos; i++) {
		fprintf (log_mundo, " ");
		imprimir_barco (log_mundo, barcos + i);
	}
	fprintf (log_mundo, " ");
	for (i = 0; i < num_cardumes; i++) {
		fprintf (log_mundo, " ");
		imprimir_cardume (log_mundo, cardumes + i);
	}
	fprintf (log_mundo, "\n");
	fflush (log_mundo);
}

static void imprimir_log_mundo ()
{
    struct timeval tempo;
    int i;
    
    gettimeofday(&tempo, NULL);
    
    if(fwrite(&tempo.tv_usec, sizeof(tempo.tv_usec), 1, log_mundo)!=1){

        perror("Escrita para ficheiro de dados");
        exit(1);
    }
 	printf("%s\n", tempo.tv_usec);
    if(fwrite(mundo, sizeof(mundo), 1, log_mundo)!=1){
        perror("Escrita para ficheiro de dados");
        exit(1);
    }
    
    for (i = 0; i < num_barcos; i++) {
    	printf("%s\n", barcos + i);
        if(fwrite(barcos + i, sizeof(barcos + i), 1, log_mundo)!=1){
         perror("Escrita para ficheiro de dados");
         exit(1);
        }
    }

    for (i = 0; i < num_cardumes; i++) {
    	printf("%s\n", cardumes + i);
        if(fwrite(cardumes + i, sizeof(cardumes + i), 1, log_mundo)!=1){
            perror("Escrita para ficheiro de dados");
            exit(1);
        }
    }

    fflush (log_mundo);
}

static void imprimir_bonito_mundo ()
{
	int linha;
  	printf ("%10ld", time (NULL));
	printf (" %s %5d %2d %2d", ECS [mundo->estado_capitao], mundo->peixes_cais, mundo->barcos_cais, mundo->jornadas_pesca);
	if (mundo->estado_capitao == C_RECEBER || mundo->estado_capitao == C_AGUARDAR)
		printf (" %10ld", mundo->hora_regressar);
	printf ("\n[");
	fifoPrint (&mundo->espera_barcos);
	printf ("]\n");
	int stop = max (num_barcos, max (num_cardumes, DIMENSAO_MAR));
	linha = 0;
	while (linha < stop) {
		if (linha < num_barcos) {
			printf ("| ");
			imprimir_barco (stdout, barcos + linha);
		}
		else
			printf ("                         ");
		printf ((linha < num_barcos || linha < num_cardumes) ? " | " : "   ");
		if (linha < num_cardumes)
			imprimir_cardume (stdout, cardumes + linha);
		else
			printf ("              ");
		if (linha < DIMENSAO_MAR) {
			printf (" |");
			int col;
			for (col = 0; col < DIMENSAO_MAR; col++) {
				static char *pattern[] = {"  ", "Ca", "Ba", "CB"};
				printf (pattern [
				    (mundo->mar [col][DIMENSAO_MAR - linha - 1].cardume == VAZIO ? 0 : 1)
				  + (mundo->mar [col][DIMENSAO_MAR - linha - 1].barco == VAZIO ? 0 : 2)]);
			}
			printf ("|");
		}
		printf ("\n");
		linha++;
	}
}

void imprimir_mundo ()
{
	/* imprimir_bonito_mundo (); */
    
    //
    semDown("mutex", mutex);

	imprimir_log_mundo ();
    
    //
    semUp("mutex", mutex);

}
