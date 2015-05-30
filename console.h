/* file:       console.h                                             */
/* purpose:    header file for direct terminal I/O functions         */
/*             supporting both Windows and Unix Platforms            */
/* written by: Danny Abesdris, Catherine Leung                       */
/* date:       May 2012                                              */

#ifndef _CONSOLE_H_   /* this conditional statement is used to guard */
                      /* against the possibility of including this   */
                      /* header file more than once                  */
   #define _CONSOLE_H_

   #define WINDOWS 1
   #define UNIX 2
   #define OSX 3

   /* the line below must be changed to reflect the platform */
   /* that these routines will be compiled on */
   #define PLATFORM WINDOWS

   #if PLATFORM == UNIX || PLATFORM == OSX
      #include <ncurses.h>
   #else
      #include <conio.h>
      #include <windows.h>
   #endif
#include <stdio.h>
#include <string.h>

   void consoleOn(void);       /* initialize screen */
   void consoleOff(void);        /* restore/shutdown screen use */
   int  consoleGetRows(void);        /* returns # of rows in screen */
   int  consoleGetCols(void);        /* returns # of columns in screen */
   void consoleClear(void);       /* clears screen */
   void consoleFlush(void);      /* refreshes screen to show changes */
   int  consoleGetKey(void);      /* picks up and returns a single key press */
   void consoleMove(int, int);  /* moves cursor to specific screen position */
   void consolePutC(int);         /* outputs a single character */
   void consolePutS(char *);    /* outputs a string */
   int consoleBox(int row, int col, int l, int h, char edge, char top_btm, char ls_rs); /*draws a box*/
   void consoleDraw(char *sd, int row, int col, int len); /*draws a field of text*/
   int consoleEdit(char *se, int slen, int row, int col, int flen, int *start, int *offset, int *insert, int ctype); /*creates an editable field*/


   /* some platform-dependent keys (obtained by experimentation) */
   #if PLATFORM == UNIX
		#define UP    259
		#define DOWN  258
		#define LEFT  260
		#define RIGHT 261
		#define PGUP 339 	
		#define PGDN 338
		#define ENTER 10
		#define TAB 9
		#define BS 263
		#define DEL 330
		#define HOME 362
		#define END 385
		#define ESC 27
		#define INS 331
		#define F1 265
		#define F2 266
		#define F3 267
		#define F4 268
		#define F5 269
		#define F6 270
		#define F7 271
		#define F8 272
		#define F9 273
		#define F10 274
   #elif PLATFORM == OSX
   #elif PLATFORM == WINDOWS
		#define UP 1072
		#define DOWN 1080
		#define	LEFT 1075
		#define RIGHT 1077
		#define PGUP 1073 	
		#define PGDN 1081
		#define ENTER 13
		#define TAB 9
		#define BS 8
		#define DEL 1083
		#define HOME 1071
		#define END 1079
		#define ESC 27
		#define INS 1082
		#define F1 1059
		#define F2 1060
		#define F3 1061
		#define F4 1062
		#define F5 1063
		#define F6 1064
		#define F7 1065
		#define F8 1066
		#define F9 1067
		#define F10 1068
   #endif
#endif /* end of file _console_h_ */