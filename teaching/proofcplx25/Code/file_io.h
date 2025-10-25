#ifndef FILE_IO_H
#define FILE_IO_H

/*
 * MODULE:
 *   file_io -- interface to stdio.h
 *
 * DESCRIPTION:
 *   The module file_io is an interface to the C standard library file I/O
 *   functions. The functions in file_io are guaranteed either to succeed or
 *   to exit. Thus no error checking is needed on return of the functions.
 *
 *	Function		Returns
 *	--------		-------
 *
 *	file_open_readfile	an opened file for reading
 *
 *	file_open_writefile	an opened file for writing
 *
 *	file_open_appendfile	an opened file for writing at the end
 *
 *
 *	Procedure		Action
 *	---------		------
 *
 *	file_close		closes a file
 *
 *	file_readline_del_nl	reads a line, deleting any newline character;
 *				if a newline was read TRUE is returned
 *
 *	file_skip_to_nl_incl	reads past the next newline    
 *
 *	file_set_unbuffered	turns off buffering for file (must be called
 *				before any operation on file)
 *
 * MODULE VERSION:
 *   1.0 (August 22, 2003)
 *
 * MODULE AUTHOR:
 *   Jakob Nordström
 *   E-mail: jakobn@kth.se
 *
 */

#include <stdio.h>

#include "boolean.h"



/* file_open_readfile 
 * 
 * file_open_readfile returns a file opened for reading. file_open_readfile
 * is guaranteed either to return an open file or to exit.
 *
 */

FILE *
file_open_readfile (
    char     *name);



/* file_open_writefile 
 * 
 * file_open_writefile returns a file opened for writing. file_open_writefile
 * is guaranteed either to return an open file or to exit.
 *
 */

FILE *
file_open_writefile (
    char     *name);



/* file_open_appendfile 
 * 
 * file_open_appendfile returns a file opened for appending (i.e. writing
 * at end-of-file). file_open_appendfile is guaranteed either to return an
 * open file or to exit.
 *
 */

FILE *
file_open_appendfile (
    char     *name);



/* file_close 
 * 
 * file_close successfully closes a file or exits upon failure.
 *
 */

void
file_close (
    FILE    *f);



/* file_readline_del_nl 
 * 
 * file_readline_del_nl reads a line from a file into a string and deletes
 * a possible final newline character ('\n'). If a newline was read then 
 * nl_deleted is set to TRUE. file_readline_del_nl returns EOF if end
 * of file was reached and zero otherwise. If an error occurs during
 * reading, file_readline_del_nl exits.
 *
 */

int
file_readline_del_nl (
    FILE      *f,
    char      *line,
    int        max_nchars,
    Boolean   *nl_deleted);



/* file_skip_to_nl_incl 
 * 
 * file_skip_to_nl_incl reads characters from a file until it encounters
 * newline or end-of-file (i.e. the procedure reads *past* the next newline). 
 *
 */

void
file_skip_to_nl_incl (
    FILE     *f);



/* file_set_unbuffered
 *
 * file_set_unbuffered turns off buffering for file. It must be called
 * before any operation on the file.
 *
 */

void
file_set_unbuffered (
    FILE      *f);


#endif				/* #ifndef FILE_IO_H */
