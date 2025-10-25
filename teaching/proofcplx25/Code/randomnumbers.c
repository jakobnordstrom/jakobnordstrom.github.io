#include "randomnumbers.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

long int random(void);  



/* random_integer
 *
 * IMPLEMENTATION COMMENT:
 *
 * Under Linux, RAND_MAX is the max value of random (). On the Ultra Sparcs
 * at NADA, KTH running Solaris, this is not so. Therefore, we use
 * random () % RAND_MAX to be safe.
 *
 */

long int
random_integer (
    long int      lower,
    long int      upper)
{
    long int      intlength;
    long int      randnum;
    double        randval;

    assert (upper > lower);
    assert (upper - lower + 1 <= RAND_MAX);
    
    intlength = upper - lower + 1;
    randval   = (random () % RAND_MAX) / ((double) RAND_MAX);
    
    assert (0 <= randval && randval < 1);
    
    randnum = floor (lower + intlength * randval);
    
    assert (randnum >= lower);
    assert (randnum <= upper);
    
    return randnum;
}



/* random_real
 *
 */

double
random_real (
    double        lower,
    double        upper)
{
    double        intlength;
    double        randval;

    assert (upper > lower);
    
    intlength = upper - lower;
    randval   = random () / ((double) RAND_MAX);
    
    assert (0 <= randval && randval <= 1);

    return lower + intlength * randval;
}

