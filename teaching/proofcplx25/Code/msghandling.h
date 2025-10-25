
#ifndef MSGHANDLING_H
#define MSGHANDLING_H

/*
 * MODULE:
 *   msghandling -- formatted printing of messages to file with possible exit
 *
 * MODULE DESCRIPTION:
 *   This module contains procedures for writing formatted messages to files
 *   and possibly exiting (in the case of error messages). The module
 *   contains the following functions and procedures.
 *
 *	Procedure	Description
 *	---------	-----------
 *
 *	handle_error	Prints error messages (and possibly exits)
 *
 *	print_logmsg   	Prints log messages to file
 *
 *	set_progname	Sets program name (stored as static variable in module)
 *			Should be called once at beginning of program
 *
 *	Function 	Returns    
 *	-------- 	-------    
 *
 *	progname   	string with name of program (as set by set_progname)
 *
 * MODULE VERSION:
 *   1.1 (August 22, 2003)
 *
 * MODULE AUTHOR:
 *   Jakob Nordström
 *   E-mail: jakobn@kth.se
 *
 */


#include <stdio.h>

#include "boolean.h"


/*
 * symbolic constants for communication with the error handling procedures
 *
 */

#undef DO_EXIT
#undef DO_NOT_EXIT
#undef ADD_TIMESTAMP
#undef NO_TIMESTAMP
#undef ADD_NEWLINE
#undef NO_NEWLINE
#undef IS_WARNING
#undef IS_OTHER

#define DO_EXIT          TRUE 	  /* exit after printing error message       */
#define DO_NOT_EXIT      FALSE	  /* don't exit after printing error message */

#define ADD_TIMESTAMP    TRUE	  /* add time stamp to log message           */
#define NO_TIMESTAMP     FALSE	  /* don't add time stamp to log message     */

#define ADD_NEWLINE      TRUE	  /* add newline to log message              */
#define NO_NEWLINE       FALSE	  /* don't add newline to log message        */

#define IS_WARNING       0    	  /* used when the warning message shouldn't */
				  /*     contain an error code               */
#define IS_OTHER        -1	  /* indicator of message other than warning */
				  /*     or error                            */


/*
 * exit and error codes
 *
 */

#undef NORMAL_TERMINATION
#undef ERROR_SYNTAX
#undef ERROR_FORMAT
#undef ERROR_FILE_IO
#undef ERROR_ALLOC
#undef ERROR_UNKNOWN
#undef ERROR_INTERNAL

#define NORMAL_TERMINATION   0
#define ERROR_SYNTAX         1   /* wrong syntax in call to program         */
#define ERROR_FORMAT         2   /* wrong format in indata or arguments     */
#define ERROR_FILE_IO        3   /* error when reading/writing files        */
#define ERROR_ALLOC          4   /* error when allocating memory            */
#define ERROR_UNKNOWN       42   /* error for unknown reasons               */
#define ERROR_INTERNAL      99   /* internal error                          */




/* handle_error
 *
 * handle_error prints error and warning messages, after which it exits the
 * program if the corresponding flag is set. The message is always
 * terminated by a newline, and the file to which the message is printed
 * is flushed.
 *
 * The arguments to handle_error are: 
 * - the file on which to print the message,
 * - a string identifying the calling party (typically the program name which 
 *   can be obtained by calling progname) or NULL,
 * - an integer with the error code or code IS_WARNING for warning,
 * - a flag indicating whether the program should exit (if == DO_EXIT),
 * - a format string containing the error message with conversion specifi-
 *   cations if any, 
 * - an optional list of arguments matching the conversion specifications
 *   in the error message.
 *
 * The flag to exit can only be set for an error, not for a warning.
 *
 * Insertion of arguments into the error message is made with the same
 * conversion specifications that apply for the fprintf function in the
 * stdio-library, and the same rules as for fprintf apply for the list of
 * arguments to be inserted.
 *
 */

void
handle_error (
    FILE         *outfile,	/* file on which to print error message    */
    const char   *callingParty,	/* identifier for calling function/program */
    int           errorCode,	/* number specifying the error type        */
    Boolean       doExit,	/* == DO_EXIT if the program should exit   */
    char         *errorMessage,	/* format string containing error message  */
    ...);			/* arguments for the error message         */




/* print_logmsg
 *
 * print_logmsg prints formatted log messages to files. 
 *
 * The arguments to print_logmsg are: 
 * - the file on which to print the message,
 * - a string identifying the calling party (typically the program name which 
 *   can be obtained by calling progname) or NULL if no calling party should
 *   be identified,
 * - a flag indicating whether a time stamp should be added to the message,
 * - a flag indicating whether a newline should be appended at the end of
 *   the message,
 * - a message string with conversion specifications if any, 
 * - an optional list of arguments matching the conversion specifications
 *   in the message.
 *
 * Insertion of arguments into the message is made with the same
 * conversion specifications that apply for the fprintf function in the
 * stdio-library, and the same rules as for fprintf apply for the list of
 * arguments to be inserted.
 *
 * Note that the log file is not flushed by print_logmsg. This could be done
 * by the user either by setting the log file as unbuffered or by explicitly
 * calling fflush ().
 *
 */

void
print_logmsg (
    FILE         *outfile,	/* file on which to print message          */
    const char   *callingParty,	/* identifier for calling function/program */
    Boolean       addTimestamp,	/* indicates if time stamp should be added */
    Boolean       addNewline,	/* indicates if newline should be added    */
    char         *msgstring,	/* format string containing message        */
    ...);			/* arguments for the  message              */




/* set_progname
 * 
 * set_progname sets the program name. It should be called once at the
 * beginning of the program.
 * 
 * set_progname assumes the the program name is the substring in the
 * argument string from the first character after the last slash ("/") or
 * backslash ("\") to the end of the string. (Thus typically the call to
 * set_progname would look like "set_progname (argv[0]);").
 * 
 */

void
set_progname (
    char    *progname);


/* progname 
 * 
 * progname returns the name of the program as set by set_progname
 *
 */

const char *
progname (void);


#endif				/* #ifndef MSGHANDLING_H */



