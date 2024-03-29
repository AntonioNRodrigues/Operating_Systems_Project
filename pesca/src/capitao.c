#include <unistd.h>
#include <stdlib.h>

#include "capitao.h"
#include "mundo.h"
#include "barco.h"

#include "semaphore.h"

//

const char *ECS[] = {"PLA", "AGU", "REC", "FIM"};

static void selecionar_barcos ();
static void esperar_barcos ();
static void receber_captura_peixe ();
static void partilhar_lucros ();

void main_capitao ()
{
	while (mundo->jornadas_pesca > 0) {
		selecionar_barcos ();
		do {
			esperar_barcos ();
			receber_captura_peixe ();
		} while (mundo->barcos_cais < num_barcos);
		mundo->jornadas_pesca--;
	}
	partilhar_lucros ();
}

void selecionar_barcos ()
{

	mundo->estado_capitao = C_PLANEAR;
    //numero de barcos nao pode ser 1, provoca divide by zero na instrucao abaixo
	int barcos_escolhidos = num_barcos / 2 + random () % (num_barcos / 2);
	int i;
	for (i = 0; i < barcos_escolhidos; i++) {
        barcos [i].ordem = O_PESCAR;
		barcos [i].destino.x = random () % DIMENSAO_MAR;
		barcos [i].destino.y = random () % DIMENSAO_MAR;
		semUp ("barco", sem_barcos [i]);
	}
	mundo->hora_regressar = time (NULL) + DURACAO_JORNADA_PESCA;
	imprimir_mundo ();

}

void esperar_barcos ()
{
	mundo->estado_capitao = C_AGUARDAR;
    
    //
    printf("O capitão está à espera dos barcos \n");
    
    // Aguardar - o capitão aguarda a chegada dos barcos.
    semDown("capitao", sem_capitao);

	imprimir_mundo ();
}

void receber_captura_peixe ()
{

    
	mundo->estado_capitao = C_RECEBER;
	
    
    
    int barco_chegou = fifoPop (&mundo->espera_barcos);
    printf("O capitão recebeu os %d peixes capturados pelo barco [%d] \n", barcos [barco_chegou].peixe_pescado, barco_chegou);


    mundo->peixes_cais += barcos [barco_chegou].peixe_pescado;

	barcos [barco_chegou].peixe_pescado = 0;
    
	barcos [barco_chegou].ordem = O_NENHUMA;

	imprimir_mundo ();

	usleep ((__useconds_t) (500 + random () % 1500));

}

void partilhar_lucros ()
{
	int i;


	mundo->estado_capitao = C_FIM;
	for (i = 0; i < num_barcos; i++) {
        //
        printf("O capitão deu ordem de festejar ao barco [%d] \n", i);

		barcos [i].ordem = O_FESTEJAR;
        
        //
        semUp ("barco", sem_barcos [i]);

	}
	imprimir_mundo ();

}
