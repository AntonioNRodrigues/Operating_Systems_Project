#include "util.h"

bool between (int value, int min, int max)
{
	return min <= value && value <= max;
}

int min (int a, int b)
{
	return a < b ? a : b;
}

int max (int a, int b)
{
	return a > b ? a : b;
}
