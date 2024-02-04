#include "msghandling.h"

#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <assert.h>
#include <string.h>

#undef  DATESTR_MAXLEN
#define DATESTR_MAXLEN    160


/*
 * macros for allocating memory
 *
 */

#undef NEW
#undef NEW_ARRAY
#define NEW(type)              (malloc (sizeof(type)))
#define NEW_ARRAY(type, size)  (calloc ((size), sizeof(type)))


static char      *program_name        = "Unknown program name";
static Boolean    program_name_set    = FALSE;
static Boolean    name_warning_issued = FALSE;
 

/* print_message
 *
 * print_message does the actual conversion and printing of the
 * messages.
 *
 */

static void
print_message (
    FILE         *outfile,	/* file on which to print error message      */
    int           msgtype,	/* number specifying type of message         */
    Boolean       addTimestamp,	/* if == ADD_TIMESTAMP time stamp is printed */
    Boolean       addNewline,	/* if == ADD_NEWLINE a newline is added      */
    const char   *callingParty,	/* identifier for calling function/program   */
    char         *formatString,	/* string with message and conversion spec   */
    va_list       argsPointer)	/* list of arguments to format string        */
{
    time_t       *timep;
    char         *datestr;
     
    assert (outfile      != NULL);
    assert (formatString != NULL);
    assert (addTimestamp == ADD_TIMESTAMP || addTimestamp == NO_TIMESTAMP);
    assert (addNewline   == ADD_NEWLINE   || addNewline   == NO_NEWLINE);
    assert (msgtype == IS_WARNING || msgtype == IS_OTHER || msgtype > 0);
    
    if (addTimestamp == ADD_TIMESTAMP)
    {
	datestr = NEW_ARRAY (char, DATESTR_MAXLEN);
	timep   = NEW (time_t);
	
	if (datestr == NULL || timep == NULL)
	{
	    fprintf (stderr,
		     "Internal error on line %d in %s.\n",
		     __LINE__,
		     __FILE__);
	    exit (ERROR_INTERNAL);
	}
	
	if (-1 == time (timep)) 
	{
	    fprintf (stderr,
		     "Internal error on line %d in %s.\n",
		     __LINE__,
		     __FILE__);
	    exit (ERROR_INTERNAL);
	}
	
	strftime (datestr,
		  DATESTR_MAXLEN - 1,
		  "%b %d %H:%M:%S",
		  localtime (timep));
	
	fprintf (outfile,
		 "%s : ",
		 datestr);
	
	free (timep);
	free (datestr);
    }
    
    if (callingParty != NULL)
    {
	fprintf (outfile,
		 "%s : ",
		 callingParty);
    }
    
    if (msgtype == IS_WARNING)
    {
	fprintf (outfile,
		 "Warning : ");
    }
    else if (msgtype > 0)	/* error message; msgtype is error code */
    {
	fprintf (outfile,
		 "Error %d : ",
		 msgtype);
    }
    
    vfprintf (outfile,
	      formatString,
	      argsPointer);
    
    if (addNewline == ADD_NEWLINE)
    {
	fprintf (outfile,
		 "\n");
    }
}



void
handle_error (
    FILE         *outfile,	/* file on which to print error message    */
    const char   *callingParty,	/* identifier for calling function/program */
    int           errorCode,	/* number specifying the error type        */
    Boolean       doExit,	/* == DO_EXIT if the program should exit   */
    char         *errorMessage,	/* format string containing error message  */
    ...)			/* arguments for the error message         */
{
    va_list       argsPointer;	/* list of arguments to error message      */
    
    if (   outfile      == NULL
	|| errorMessage == NULL)
    {
	fprintf (stderr,
		 "Illegal NULL pointer detected (%s: line %d).",
		 __FILE__,
		 __LINE__);
	exit (ERROR_INTERNAL);
    }

    if (errorCode == IS_WARNING && doExit == DO_EXIT)
    {
	fprintf (stderr,
		 "No exiting after warning only (%s: line %d).",
		 __FILE__,
		 __LINE__);
	exit (ERROR_INTERNAL);	
    }
    
    va_start (argsPointer, errorMessage);
    
    print_message (outfile, 
		   errorCode,
		   NO_TIMESTAMP,
		   ADD_NEWLINE,
		   callingParty,
		   errorMessage,
		   argsPointer);
    
    va_end (argsPointer);
    
    fflush (outfile);
    
    if (doExit == DO_EXIT)
    {
	exit (errorCode);
    }
}



void
print_logmsg (
    FILE         *outfile,	
    const char   *callingParty,	
    Boolean       addTimestamp,	
    Boolean       addNewline,	
    char         *msgstring,	
    ...)			
{
    va_list       argsPointer;	/* list of arguments to message */
    
    if (   outfile   == NULL
	|| msgstring == NULL)
    {
	fprintf (stderr,
		 "Illegal NULL pointer detected (%s: line %d).",
		 __FILE__,
		 __LINE__);
	exit (ERROR_INTERNAL);
    }
    
    if (   addTimestamp != ADD_TIMESTAMP && addTimestamp != NO_TIMESTAMP
	|| addNewline   != ADD_NEWLINE   && addNewline   != NO_NEWLINE)
    {
	fprintf (stderr,
		 "Illegal flag value detected (%s: line %d).",
		 __FILE__,
		 __LINE__);
	exit (ERROR_INTERNAL);	
    }
    
    va_start (argsPointer, msgstring);
    
    print_message (outfile, 
		   IS_OTHER,
		   addTimestamp,
		   addNewline,
		   callingParty,
		   msgstring,
		   argsPointer);
    
    va_end (argsPointer);
}



void
set_progname (
    char    *progname)
{
    char    *namesubstring;
    
    if (progname == NULL)
    {
        handle_error (stderr,
		      "Message handling module",
		      ERROR_INTERNAL,
		      DO_EXIT,
		      "Program name string is NULL pointer (%s: line %d).",
		      __FILE__,
		      __LINE__);
	exit (ERROR_INTERNAL);
    }
    
    if (program_name_set)
    {
        handle_error (stderr,
		      "Message handling module",
		      ERROR_INTERNAL,
		      DO_EXIT,
		      "Attempt to set program name twice (%s: line %d).",
		      __FILE__,
		      __LINE__);	
        exit (ERROR_INTERNAL);
    }
    
    namesubstring = progname;
    
    while (*progname != '\0')
    {
	if (*progname == '/' || *progname == '\\')
	{
	    namesubstring = ++progname;
	}
	else
	{
	    ++progname;
	}
    }
    
    program_name = NEW_ARRAY (char, strlen (namesubstring) + 1);
    
    if (program_name == NULL)
    {
        handle_error (stderr,
		      "Message handling module",
		      ERROR_ALLOC,
		      DO_EXIT,
		      "Failed allocation of memory (%s: line %d).",
		      __FILE__,
		      __LINE__);	
	exit (ERROR_ALLOC);
    }
    
    strcpy (program_name, namesubstring);
    program_name_set = TRUE;
}



const char *
progname (void)
{
    if (!program_name_set && !name_warning_issued)
    {
        handle_error (stderr,
		      "Message handling module",
		      IS_WARNING,
		      DO_NOT_EXIT,
		      "Program name not set (%s: line %d).",
		      __FILE__,
		      __LINE__);
	
	name_warning_issued = TRUE;	/* issue this warning only once */
    }
    
    return program_name;
}
