#include "posicao.h"

const Posicao DELTA[TV] = {{-1, 0}, {1, 0}, {0, 1}, {0, -1}};

const Posicao NIL_POS = {-1, -1};

void calcula_delta (const Posicao *de, const Posicao *para, Posicao *delta)
{
	if (de->x > para->x) {
		delta->x = -1;
		delta->y = 0;
	}
	else if (de->x == para->x) {
		delta->x = 0;
		if (de->y > para->y) {
			delta->y = -1;
		}
		else {
			delta->y = 1;
		}
	}
	else {
		delta->x = 1;
		delta->y = 0;
	}
}

void somar_posicoes (Posicao *resultado, const Posicao *a, const Posicao *b)
{
	resultado->x = a->x + b->x;
	resultado->y = a->y + b->y;
}
