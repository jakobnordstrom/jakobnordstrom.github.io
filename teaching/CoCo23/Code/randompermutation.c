/*
 * NAME:
 *   randompermutation - reads a list of names and outputs a random
 *   permutation guaranteed not to contain any fixpoints or cycles of
 *   length 2.
 *
 * SYNTAX:
 *   randompermutation <filename> <seed>
 *
 * DESCRIPTION:
 *
 * EXAMPLE:
 *
 * VERSION DATE:
 *   Nov 2, 2016
 *
 * AUTHOR:
 *   Jakob Nordström
 *   Email: jakobn@kth.se
 *
 */

#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "boolean.h"
#include "msghandling.h"
#include "file_io.h"
#include "randomnumbers.h"

#define MAXNPERSONS       100
#define MAXTEXTLENGTH    1000



/* permute
 *
 * permute makes a random permutation of <nint> integers.
 *
 */


static void
permute (
    int      perm[],
    int      nint)
{
    int      i;
    int      swapval;
    int      swapi;

    for (i = 1; i <= nint; ++i)
    {
	perm[i] = i;
    }
    
    for (i = nint; i > 1; --i)
    {
	swapi       = random_integer (1, i);
	swapval     = perm[swapi]; 
	perm[swapi] = perm[i];
	perm[i]     = swapval;
    }
}


int
main (
    int          argc,
    char       **argv)
{
    int          perm[MAXNPERSONS + 1];
    int          nint;
    int          randomseed;
    Boolean      okpermutation;
    int          i;
    char         names[MAXNPERSONS][MAXTEXTLENGTH];
    FILE        *namefile;  
    Boolean      nl_deleted;

    set_progname (argv[0]);
    
    if (   argc != 3
	|| 1 != sscanf (argv[2], "%d", &randomseed))
    {
	handle_error (stderr, progname (), ERROR_SYNTAX, DO_EXIT,
		      "Syntax is \""
		      "%s <filename> <seed>"
		      "\"", progname ());	
	exit (ERROR_SYNTAX);
    }
    
    namefile = file_open_readfile (argv[1]);

    if (namefile == NULL)
    {
	handle_error (stderr, progname (), ERROR_SYNTAX, DO_EXIT,
		      "Could not open file%s", argv[1]);
	exit (ERROR_FILE_IO);
    }
    
    nint = 1;

    while (EOF != file_readline_del_nl (namefile, 
					names[nint], 
					MAXTEXTLENGTH - 1, 
					&nl_deleted))
    {
	++nint;
    }

    --nint;

    if (nint > MAXNPERSONS)
    {
	handle_error (stderr, progname (), ERROR_INTERNAL, DO_EXIT,
		      "Number of persons can be at most %d.",
		      MAXNPERSONS);
	exit (ERROR_SYNTAX);
	
    }

    okpermutation = FALSE;

    while (!okpermutation)
    {
	srandom (randomseed);

	permute (perm, nint);

	okpermutation = TRUE;
	
	for (i = 1; i <= nint; ++i)
	{
	    if (   perm[i]       == i
		|| perm[perm[i]] == i)
	    {
		okpermutation = FALSE;
	    }
	}
	
	randomseed += 1;
    }
    
    printf ("PERMUTATION GENERATED WITH RANDOM SEED %d\n", randomseed - 1);
    
    for (i = 1; i <= nint; ++i)
    {
	printf ("%3d %s\t%3d %s\n", i, names[i], perm[i], names[perm[i]]);
    }
    
    return 0;
}

