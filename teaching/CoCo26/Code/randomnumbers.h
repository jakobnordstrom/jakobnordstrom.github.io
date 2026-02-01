

#ifndef RANDOMNUMBERS_H
#define RANDOMNUMBERS_H

/*
 * MODULE:
 *   randomnumbers -- a quick-and-dirty lousy random number generator
 *
 * MODULE DESCRIPTION:
 *   This module contains quick-and-dirty functions producing (rather lousy)
 *   pseudorandom numbers. The random seed is set by calling srandom.
 *
 *	Function			Returns
 *	--------			-------
 *
 *	random_integer			a random integer in specified interval
 *
 *	random_real			a random float in specified interval
 *
 * MODULE VERSION:
 *   1.0 (August 14, 2003)
 *
 * MODULE AUTHOR:
 *   Jakob Nordström
 *   E-mail: jakobn@kth.se
 *
 */


void srandom (unsigned int seed);


/* random_integer
 *
 * random_integer returns a (rather bad) pseudorandom integer between
 * the lower and upper limits inclusive. The interval must not be larger
 * than RAND_MAX.
 *
 */

long int
random_integer (
    long int      lower,
    long int      upper);



/* random_real
 *
 * random_real returns a (rather bad) pseudorandom real between the lower and
 * upper limits inclusive.
 *
 */

double
random_real (
    double        lower,
    double        upper);


#endif				/* #ifndef RANDOMNUMBERS_H */

