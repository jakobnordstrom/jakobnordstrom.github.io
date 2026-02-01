#include "file_io.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "msghandling.h"


#undef  NUL
#undef  NEWLINE
#define NUL           '\0'
#define NEWLINE       '\n'



FILE *
file_open_readfile (
    char     *name)
{
    FILE     *f;
    
    if (name == NULL)
    {
	handle_error (stderr, progname (), ERROR_FILE_IO, DO_EXIT,
		      "File name string is NULL in %s: line %d.",
		      __FILE__,
		      __LINE__);
	exit (ERROR_FILE_IO);	
    }
    
    f = fopen (name, "r");
    
    if (f == NULL)
    {
	handle_error (stderr, progname (), ERROR_FILE_IO, DO_EXIT,
		      "Failed to open file \"%s\" for reading.",
		      name);
	exit (ERROR_FILE_IO);
    }
    
    return f;
}



FILE *
file_open_writefile (
    char     *name)
{
    FILE     *f;
    
    if (name == NULL)
    {
	handle_error (stderr, progname (), ERROR_FILE_IO, DO_EXIT,
		      "File name string is NULL in %s: line %d.",
		      __FILE__,
		      __LINE__);
	exit (ERROR_FILE_IO);	
    }
    
    f = fopen (name, "w");
    
    if (f == NULL)
    {
	handle_error (stderr, progname (), ERROR_FILE_IO, DO_EXIT,
		      "Failed to open file \"%s\" for writing.",
		      name);
	exit (ERROR_FILE_IO);
    }
    
    return f;
}



FILE *
file_open_appendfile (
    char     *name)
{
    FILE     *f;
    
    if (name == NULL)
    {
	handle_error (stderr, progname (), ERROR_FILE_IO, DO_EXIT,
		      "File name string is NULL in %s: line %d.",
		      __FILE__,
		      __LINE__);
	exit (ERROR_FILE_IO);	
    }
    
    f = fopen (name, "a");
    
    if (f == NULL)
    {
	handle_error (stderr, progname (), ERROR_FILE_IO, DO_EXIT,
		      "Failed to open file \"%s\" for appending.",
		      name);
	exit (ERROR_FILE_IO);
    }
    
    return f;
}



void
file_close (
    FILE    *f)
{
    if (f == NULL)
    {
	handle_error (stderr, progname (), ERROR_FILE_IO, DO_EXIT,
		      "File NULL pointer detected when trying to close file.");

	exit (ERROR_FILE_IO);
    }
    
    if (fclose (f) == EOF)
    {
	handle_error (stderr, progname (), ERROR_FILE_IO, DO_EXIT,
		      "Failed to close file.");
	exit (ERROR_FILE_IO);
    }
}



int
file_readline_del_nl (
    FILE     *f,
    char     *line,
    int        max_nchars,
    Boolean   *nl_deleted)
{
    int       i;
    Boolean   eof_reached;
    
    *nl_deleted = FALSE;
    eof_reached = FALSE;
    
    if (   f    == NULL
	|| line == NULL)
    {
	handle_error (stderr, progname (), ERROR_FILE_IO, DO_EXIT,
		      "File NULL pointer detected when "
		      "trying to read line.");
	exit (ERROR_FILE_IO);
    }
    else if (max_nchars <= 0)
    {
	handle_error (stderr, progname (), ERROR_FILE_IO, DO_EXIT,
		      "Max number of characters to read from "
		      "file is %d <= 0.",
		      max_nchars);
	exit (ERROR_FILE_IO);
    }
    
    if (fgets (line, max_nchars, f) == NULL)
    {
	if (ferror (f))
	{
	    handle_error (stderr, progname (), ERROR_FILE_IO, DO_EXIT,
			  "Error when reading line from file.");
	    exit (ERROR_FILE_IO);
	}
	else if (feof (f))
	{
	    eof_reached = TRUE;
	}
	else
	{
	    handle_error (stderr, progname (), ERROR_INTERNAL, DO_EXIT,
			  "Internal error on line %d in %s.\n",
			  __LINE__,
			  __FILE__);
	    exit (ERROR_INTERNAL);
	}
    }
    else
    {
	eof_reached = FALSE;

	i = 0;
	
	while (line[i] != NUL && line[i] != NEWLINE)
	{
	    ++i;
	}
	
	if (line[i] == NEWLINE)
	{
	    line[i]     = NUL;
	    *nl_deleted = TRUE;
	}
	else
	{
	    *nl_deleted = FALSE;
	}
    }

    return (eof_reached ? EOF : 0);
}



void
file_skip_to_nl_incl (
    FILE     *f)
{
    int       c;
    
    if (f == NULL)
    {
	handle_error (stderr, progname (), ERROR_FILE_IO, DO_EXIT,
		      "File NULL pointer detected when "
		      "trying to skip to newline.",
		      __FILE__,
		      __LINE__);
	exit (ERROR_FILE_IO);
    }
    
    c = fgetc (f);
    
    while (c != EOF && c!= NEWLINE)
    {
	c = fgetc (f);
    }
}


void
file_set_unbuffered (
    FILE      *f)
{
    int        retval;
    
    if (f == NULL)
    {
	handle_error (stderr, progname (), ERROR_FILE_IO, DO_EXIT,
		      "File NULL pointer detected when "
		      "trying to make file unbuffered.");
	exit (ERROR_FILE_IO);
    }
    
    retval = setvbuf (f, NULL, _IONBF, 0); 
    
    if (retval != 0)
    {
	handle_error (stderr, progname (), ERROR_FILE_IO, DO_EXIT,
		      "Failed to set file as unbuffered (return code %d)."
		      "in %s: line %d.",
		      retval);
    }
}

