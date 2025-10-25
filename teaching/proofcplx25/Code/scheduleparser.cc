/*
 * Schedule parser for generating HTML-formatted schedules
 * Compile with g++, e.g., with command line
 *
 *    g++ -o scheduleparser scheduleparser.cc 
 *
 * The program reads from standard input and writes to standard output.
 * Run it by doing something like
 *
 * scheduleparser < schedule-contents-proofcplx25.txt > proofcplx25-schedule.html
 *
 */


#include <iostream>
#include <sstream>
#include <cstdio>
#include <boost/algorithm/string.hpp>

using namespace std;

const char    delimiter = '$';

const int     widths[] = {9, 7, 6, 6, 3, 52, 17};

const char   *colours[] = {"#ffffff" , "#ccffff"};

#define ITEMSTRING    "\"%s\" "

#define TABLE_PREAMBLE							\
    "</p><table border=\"0\" cellpadding=\"2\" cellspacing=\"0\" width=\"100%%\"> \n" \
    "<tbody> \n"							\
    "<tr> \n"								\
    "<td bgcolor=\"#ccffff\" width=\"%d%%\"><em>&nbsp;Weekday</em></td> \n" \
    "<td bgcolor=\"#ccffff\" width=\"%d%%\"><em>Date</em></td> \n"	\
    "<td bgcolor=\"#ccffff\" width=\"%d%%\"><em>Time</em></td> \n"	\
    "<td bgcolor=\"#ccffff\" width=\"%d%%\"><em>Room</em></td> \n"	\
    "<td align=\"right\" bgcolor=\"#ccffff\" width=\"%d%%\"><em>#&nbsp;</em></td> \n" \
    "<td bgcolor=\"#ccffff\" width=\"%d%%\"><em>Plan of lectures</em></td> \n" \
    "<td bgcolor=\"#ccffff\" width=\"%d%%\"><em>References</em></td> \n" \
    "</tr> \n " 

#define TABLE_POSTAMBLE  \
    "</tbody> \n "	 \
    "</table> \n" 

#define ROW_PREAMBLE     "<tr>  \n"
#define ROW_POSTAMBLE    "</tr> \n"

#define ROW_GENERIC_ITEM				\
    "<td bgcolor=\"%s\" width=\"%d%%\">%s%s</td> \n"

#define ROW_NUMBER_ITEM					\
    "<td align=\"right\" bgcolor=\"%s\" width=\"%d%%\">%d.</td> \n"

#define NBSP   "&nbsp;"

#define ROW_REFSWITHNOTES_ITEM				\
    "<td bgcolor=\"%s\" width=\"%d%%\">%s%s "           \
    "<a href=\"lecturenotes/%s\">notes</a></td> \n"


#define ROW_REFSWITHSLIDES_ITEM				\
    "<td bgcolor=\"%s\" width=\"%d%%\">%s%s "            \
    "<a href=\"lecturenotes/%s\">slides</a></td> \n"


typedef struct Lecture {
    string    weekday;
    string    date;
    string    time; 
    string    room;
    string    content;
    string    refs;
    string    notes;    
} Lecture;



static void
print_row_item (
    const char     *colour, 
    int             width,
    const char     *extraspace,
    const char     *item)
{
    printf (ROW_GENERIC_ITEM, 
	    colour,
	    width, 
	    extraspace,
	    item);
}

static void
print_row_number_item (
    const char     *colour, 
    int             width,
    int             number)
{
    printf (ROW_NUMBER_ITEM,
            colour,
	    width, 
	    number);
}


static void
print_row_refs_item (
    const char     *colour, 
    int             width,
    const char     *refs,
    const char     *notes)
{
    if (strcmp (notes, "") == 0)
    {
        printf (ROW_GENERIC_ITEM, 
		colour,
		width,
		refs,
		"");
    }
    else if (notes[0] == 'n' || notes[0] == 'N') 
    {
        printf (ROW_REFSWITHNOTES_ITEM,
		colour,
		width,
		refs,
		(strcmp (refs, "") == 0) ? "" : ",",
		notes);
    }
    else if (notes[0] == 's' || notes[0] == 'S') 
    {
	printf (ROW_REFSWITHSLIDES_ITEM,
		colour,
		width,
		refs,
		(strcmp (refs, "") == 0) ? "" : ",",
		notes);
    }
    else
    {
      fprintf (stderr, "Unexpected name of notes/slides file.\n");
      exit (1);
    }
}



int 
main ( ) 
{
    int        numlines;
    char      *colour;

    scanf ("%d\n", &numlines);
    
    Lecture    lectures[numlines];
    string     line;
    
    // Read time and date info
    for (int i = 0; i < numlines; ++i)
    {
	getline (cin, line);
	
	istringstream    tokenizer (line);
	getline (tokenizer, lectures[i].weekday, delimiter);
	getline (tokenizer, lectures[i].date,    delimiter);
	getline (tokenizer, lectures[i].time,    delimiter);
	getline (tokenizer, lectures[i].room,    delimiter);
	
	boost::algorithm::trim (lectures[i].weekday);
	boost::algorithm::trim (lectures[i].date);
	boost::algorithm::trim (lectures[i].time);
	boost::algorithm::trim (lectures[i].room);
   }
    
    // Read lecture planning
    for (int i = 0; i < numlines; ++i)
    {
	getline (cin, line);
	
	istringstream    tokenizer(line);
	
	getline (tokenizer, lectures[i].content, delimiter);
	getline (tokenizer, lectures[i].refs, delimiter);
	getline (tokenizer, lectures[i].notes, delimiter);

	boost::algorithm::trim (lectures[i].content);
	boost::algorithm::trim (lectures[i].refs);
	boost::algorithm::trim (lectures[i].notes);
    }
    
    // Output schedule
    
    //    for (int i = 0; i < numlines; ++i)
    //    {
    //	// Note the call to c_str() to use a string with printf
    //	printf (ITEMSTRING ITEMSTRING ITEMSTRING ITEMSTRING "%d. " ITEMSTRING ITEMSTRING ITEMSTRING "\n",
    //		lectures[i].weekday.c_str(),
    //		lectures[i].date.c_str(),
    //		lectures[i].time.c_str(),
    //		lectures[i].room.c_str(),
    //		i + 1,
    //   	lectures[i].content.c_str(),
    //		lectures[i].refs.c_str(),
    //		lectures[i].notes.c_str());
    //    }


    printf(TABLE_PREAMBLE,
    //    printf("%d %d %d %d %d %d %d \n",
	   widths[0],	   
	   widths[1],
	   widths[2],
	   widths[3],
	   widths[4],
	   widths[5],
	   widths[6]);
	

    for (int i = 0; i < numlines; ++i)
    {
	printf (ROW_PREAMBLE);

	print_row_item (colours[i % 2], 
			widths[0],
			NBSP,
			lectures[i].weekday.c_str());

	print_row_item (colours[i % 2], 
			widths[1],
			"",
			lectures[i].date.c_str());

	print_row_item (colours[i % 2], 
			widths[2],
			"",
			lectures[i].time.c_str());

	print_row_item (colours[i % 2], 
			widths[3],
			"",
			lectures[i].room.c_str());


	print_row_number_item (colours[i % 2],
			       widths[4],
			       i + 1);

	print_row_item (colours[i % 2], 
			widths[5],
			"",
			lectures[i].content.c_str());

	print_row_refs_item (colours[i % 2], 
			     widths[6],
			     lectures[i].refs.c_str(),
			     lectures[i].notes.c_str());

	printf (ROW_POSTAMBLE);
    }

    printf (TABLE_POSTAMBLE);
}


