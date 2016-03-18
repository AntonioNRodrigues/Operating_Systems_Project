/**
 * Auxiliary functions.
 *
 * @author Pedro Mariano plmariano@ciencias.ulisboa.pt
 */
#ifndef __UTIL__
#define __UTIL__

#include <stdbool.h>

/**
 * @return true if the first parameter is between the second and the third.
 */
bool between (int value, int min, int max);

/**
 * @return the minimum of the arguments.
 */
int min (int a, int b);

/**
 * @return the maximum of the arguments.
 */
int max (int a, int b);

#endif
