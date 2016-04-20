#ifndef __CAPITAO__
#define __CAPITAO__

/**
 * Enumerado que representa os possíveis estados do capitão dos barcos de
 * pesca.
 */
typedef enum {
	C_PLANEAR, C_AGUARDAR, C_RECEBER, C_FIM
} EstadoCapitao;

/**
 * Vector para converter o estado do capitão numa cadeia de caracteres.
 */
extern const char *ECS[];

/**
 * Ciclo de vida da entidade capitão.
 */
void main_capitao ();

#endif
