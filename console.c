/* file:       console.c                                          */
/* purpose:    source file for console I/O functions              */
/*             supporting both Windows (using Visual Studio 2008) */
/*             and UNIX using GCC 4 platforms                     */
/* written by: Danny Abesdris, Catherine Leung                    */


#include <stdlib.h>
#include "console.h"


void consoleOn(void) {
  #if PLATFORM == WINDOWS
     /* no startup requirements for windows */
  #elif PLATFORM == UNIX || PLATFORM == OSX
     initscr( );
     noecho( );
     cbreak( );
     keypad(stdscr, 1);
  #endif
}
void consoleOff(void) {
  #if PLATFORM == WINDOWS
     consoleClear( );
  #elif PLATFORM == UNIX || PLATFORM == OSX
     refresh( );
     endwin( );
  #endif
}
void consoleMove(int row, int col) {
  #if PLATFORM == WINDOWS  /* windows specific routines for cursor control */
     HANDLE hStdout;
     struct _CONSOLE_SCREEN_BUFFER_INFO x;
     hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
     GetConsoleScreenBufferInfo(hStdout, &x);
     x.dwCursorPosition.X = col;
     x.dwCursorPosition.Y = row;
     SetConsoleCursorPosition(hStdout, x.dwCursorPosition);
  #elif PLATFORM == UNIX || PLATFORM == OSX
     move(row, col);
  #endif
}
int consoleGetKey(void) {
  int key;
  #if PLATFORM == WINDOWS
    key = getch( );
    /* On windows, getch( ) returns 0 for function keys, but
       0xE0 for all other non-ascii (extended) keys */
    key = (key == 0 || key == 0xE0) ? getch( ) + 1000 : key;
  #elif PLATFORM == UNIX || PLATFORM == OSX
    key = getch( );
  #endif
  return key;
}
int consoleGetRows(void) {
  int rows;
  #if PLATFORM == WINDOWS
     HANDLE hStdout;
     struct _CONSOLE_SCREEN_BUFFER_INFO x;
     hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
     GetConsoleScreenBufferInfo(hStdout, &x);
     rows = x.dwSize.Y;
  #elif PLATFORM == UNIX || PLATFORM == OSX
     rows = LINES;
  #endif
  return rows;
}
int consoleGetCols(void) {
  int cols;
  #if PLATFORM == WINDOWS
     HANDLE hStdout;
     struct _CONSOLE_SCREEN_BUFFER_INFO x;
     hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
     GetConsoleScreenBufferInfo(hStdout, &x);
     cols = x.dwSize.X;
  #elif PLATFORM == UNIX || PLATFORM == OSX
    cols = COLS;
  #endif
  return cols;
}
void consoleClear(void) {
  #if PLATFORM == WINDOWS
     DWORD n;                         /* Number of characters written */
     DWORD size;                      /* number of visible characters */
     COORD coord = { 0 };             /* Top left screen position     */
     struct _CONSOLE_SCREEN_BUFFER_INFO csbi;

     /* Get a handle to the console */
     HANDLE h = GetStdHandle (STD_OUTPUT_HANDLE);
     GetConsoleScreenBufferInfo (h, &csbi);

     /* Find the number of characters to overwrite */
     size = csbi.dwSize.X * csbi.dwSize.Y;

     /* Overwrite the screen buffer with whitespace */
     FillConsoleOutputCharacter(h, TEXT(' '), size, coord, &n);
     GetConsoleScreenBufferInfo(h, &csbi);
     FillConsoleOutputAttribute(h, csbi.wAttributes, size, coord, &n);

     /* Reset the cursor to the top left position */
     SetConsoleCursorPosition ( h, coord );
  #elif PLATFORM == UNIX || PLATFORM == OSX
    erase( );
  #endif
}
void consolePutC(int c) {
  #if PLATFORM == WINDOWS
     putch(c);
  #elif PLATFORM == UNIX || PLATFORM == OSX
     addch(c);
  #endif
}
void consolePutS(char *s) {
  #if PLATFORM == WINDOWS
     cputs(s);
  #elif PLATFORM == UNIX || PLATFORM == OSX
     addstr(s);
  #endif
}
void consoleFlush(void) {
  #if PLATFORM == WINDOWS
     /* not required to update the screen on windows */
  #elif PLATFORM == UNIX || PLATFORM == OSX
     refresh( );
  #endif
}
void consoleDraw(char *sd, int row, int col, int len){
	int size = 0, i = 0;
	size = strlen(sd);
	consoleMove(row,col);
	if(len >= size)
	  consolePutS(sd);
	else{
	  for(i=0;i<len;i++){
		consolePutC(sd[i]);
	  }
	}
	while(size<len){
		consoleMove(row,col+size);
		consolePutC(' ');
		size++;
	}
	consoleMove(row,col);
}
int consoleBox(int row, int col, int l, int h, char edge, char top_btm, char ls_rs){
	int maxrows=consoleGetRows(); 
	int maxcols=consoleGetCols(); 
	int i,currow=row;
	if (col+l>maxcols || row+h>maxrows || row<0 || col < 0 ||l<2) /*check if the box cant fit inside the space provided by the console*/
		return 0;
	consoleMove(row,col);
	consolePutC(edge);		/*draws first corner*/
	for(i=0;i<(l-2);i++){	/*draws the top edge characters based on the length given*/
		consolePutC(top_btm);
	}
	consolePutC(edge);		/*top-right corner*/
	for(i=0;i<(h-2);i++){	/*draws the side edges*/
		consoleMove(++currow,col);
		consolePutC(ls_rs);
		consoleMove(currow,col+(l-1));
		consolePutC(ls_rs);
	}
	consoleMove(++currow,col);
	consolePutC(edge);		/*bottom-left corner*/
	for(i=0;i<(l-2);i++){	/*draws bottom edge characters*/
		consolePutC(top_btm);
	}
	consolePutC(edge);		/*bottom-right corner*/
	return 1;
}
int consoleEdit(char *se, int slen, int row, int col, int flen, int *start, int *offset, int *insert, int ctype){
	//offset start from 0, eliminate 0 chars; start from 5, eliminate 5 chars, actual length of string
	//start- cursor position- cursor at 1st char display is indexed as 0
	int key = 0;
	unsigned int length = strlen(se); //return number of chars before null character-start from 0 if no chars
	int locOff = 0;	//locOff is used to preserve original offset value
	int locPos = 0; //locPos is used to preserve original start value
	int i,nLength;				//?
	int j,validKey=0;			//?
	int done = 0;
	char test[100],temp,temp2;	//?

	/*preserve initial string*/
	char *copy= (char *) malloc(strlen(se)+1);	//strbuf
	strcpy(copy,se);			
	/*initialize offset and locOff*/
	if(offset!=NULL){
		locOff = *offset;	//preserve original offset value
	}
	else{
		offset = (int*) malloc(sizeof(int));	//set *offset to 0 if NULL
		*offset=0;
	}

	/*initialize cursor and locPos*/
	if(start!=NULL){
		locPos = *start;	//preserve original cursor start value
	}
	else{ 
		start = (int*) malloc(sizeof(int));	//set *offset to 0 if NULL
		*start=0;
	}

	/*Offset restrictions (offset relate to string length and form actual length)
	 *if offset past string length, then set offset to length 
	 *e.g. "Lord h" length 6, *offset=7 will eliminate 7 chars, set it to 6 is enough
	 */
	if (*offset > length) {
		*offset = length;
	}

	/*Cursor restrictions- should be in a known location, within margin 
	 *(cursor relate to field length and actual length-se and offset) 
	 */	
	if (*start > flen || *start > (length - *offset)) { 	
	/*The cursor past field or past displayed string (actual length) 
	 *-->cursor need go back to last char or field length depends on 2 situation
	 *start>flen e.g. Lord flen=4 *start=5 which means |Lord| _
	 *start>actual length e.g. |ord| actual length=3 *start =4 which means |ord| _
	 */
		if ((length-*offset) > flen) { 
			//1st case: Actual length > field length--> results in field length
			*start = flen - 1; // e.g. if |Lor|d _ , then |Lor| cursor under r--> *start=2=flen(3)-1
			//for scrolling purpose!

		}
		else { //2nd case: Actual length < = field length-->results in last char + 1 
			*start = length - *offset; //e.g. if |Lord| _ , then |Lord|_-->*start=4 p.s. 1 char beyond d
		}
	}

	/*start edit*/
	while(!done){
		nLength = strlen(se); //Get new string size (preserve old one)
		//sprintf(test,"Length: %d Start: %d Offset: %d",nLength,*start,*offset);
		consoleMove(0,0);
		//consolePutS(test);
		consoleFlush();
		consoleDraw(&se[*offset],row, col, flen); //display box
		consoleMove(row, col + *start); 
		//dusplay cursor at string's row and column (with the *start) e.g. col =5(L) & *start= 0 -->cursor at L
		/*get key!*/
		key= consoleGetKey();
		switch(key){
		case LEFT: //3 cases: scrolling, move, halt
			if(*start != 0){ //e.g. Lo_-->goes to o
				(*start)--;
			}
			else if(*offset != 0){ //start=0 (1st char cursor) but with offset, so that scrolling strOffset
				(*offset)--;
			}
			else{ //start=0 and offset=0-->do nothing
				//done = 0;
			}
			break;
		case RIGHT: //3 cases: scrolling, move, halt
			//Based on cursor adjusts offset to show the remaining characters of string
			if (*start == (flen-1) && (nLength - *offset >= flen)) {
				//last char cursor in field && actual length >=field length
				//e.g. |Lor|d cursor under r
				(*offset)++; 
				//scrolling at beginning, add 1 offset will eliminate more, show 1 remaining char
			}
			//Movement halts after last character in actual length
			else if (*start < (nLength) - *offset) {
				//else not last char cursor in field or actual length < field length (think each case for above if)  
				// e.g. Lord cursor under L/o/r/d
				(*start)++; //moves cursor to o/r/d/_
				//if (*start >= flen)	//o man...this is impossible as the cursor restrictions has applied.
					//(*offset)++; //scrolling
			}
			else { //*start>=actual length p.s. cursor restriction so that only case is *start=actual length
				//do nothing as there is only one case left which is last character
			}
			break;
		case INS:
			*insert = !(*insert);
			break;
		case HOME:
			*start = 0;
			*offset = 0;
			break;
		case END: //2cases: actual length>=field and < field
			//String actual length is larger then field, 
			//then need to display remaining chars, different from HOME
			if (nLength - *offset > flen) {
				//e.g.  |Lord| of rings 13 and field length is 4, 
				//then cursor goes to s and offset eleminate before n
				*offset = (nLength - flen) + 1; //show |ngs_|
				*start = nLength-*offset; //actual length+1 for cursor
			}
			else if (nLength-*offset==flen){	
				//good addition from u, can be included in above if loop by n-offset>=flen
				*start = nLength-1 - *offset; 
				(*offset)++;
			}
			else
				*start= nLength-*offset; //actual length +1 for cursor
			break;
		case ENTER:
		case UP:
		case DOWN:
		case TAB:
		case PGUP:
		case PGDN:
		case F1:
		case F2:
		case F3:
		case F4:
		case F5:
		case F6:
		case F7:
		case F8:
		case F9:
		case F10:
			done = 1;
			break;
		case ESC: //check redraw
			*start = locPos; //original cursor
			*offset = locOff; //original offset
			strcpy(se, copy); //original string
			consoleDraw(&se[*offset],row, col, flen);		
      		consoleMove(row, col + *start);		
			done = 1;
			break;
		case DEL: 
			//there is a case cannot use DEL which is the cursor at the last char 
			//e.g. Lord\0 cursor position \0
			//as a result i<nLength
			//Move all characters to left by one, after the cursor
			for (i = *offset + *start; i<nLength; i++) {
				se[i] = se[i+1];
			}
		
			break;
		case BS: //there is a case cannot use BS which is the cursor at 1st char e.g. Lord L 
			//if (*start==0 && *offset!=0) which is equivalent to (*start != 0 || *offset != 0)
			if (*start ==0 && *offset==0){
			}
			else { // (*start != 0 || *offset != 0) 
				//Move all characters to left by one, including the char above the cursor
				i = *offset + *start;
				se[i-1]=se[i];
				for (i = *offset + *start; i<nLength; i++) {
					se[i] = se[i+1];
				}
				//Cursor or Offset adjustments---> different from DEL (cursor won't change)
				if (*start != 0) {
					(*start)--;
				}
				else if (*offset != 0) { //*start==0 and *offset!=0
					(*offset)--;
				}
				else { //base case start and offset ==0, do nothing
				}
			}
			break;

		default:
			//if((*start+*offset) < slen){
				switch(ctype){
				case 0:
					break;
				case 1:
					validKey=1;
					break;
				case 2:
					if(key=='0' || key=='1'){
						validKey=1;
					}
					break;
				case 4:
					if(key>='0' && key<='9'){
						validKey=1;
					}
					break;
				case 8:
					if(key>='0' && key<='7'){
						validKey=1;
					}
					break;
				case 16:
					if((key>='0' && key<='9') || (key>='a' && key<='f') || (key>='A' && key <='F')){
						validKey=1;
					}
					break;
				}
				if(validKey==1){
					if(*insert==0){	//overstrike mode
						if((nLength<=slen)&&(*start+*offset<nLength)){
							//overstriker allows editing for string has same number of chars as max string
							//but not past last char. Reason: no char after last char so that nothing can overstrike
							se[(*start)+(*offset)]=key;
							//if(*start<nLength)
							(*start)++;
						}
					}
					else if (*insert==1){ //insert mode
						if(nLength<slen){
							//insert allows editing for string which has less number of chars as max string
							//but also position past last char. Reason: insert=add, opposite of overstrike
							for(i=nLength;i>=(*start+*offset);i--){
								se[i+1]=se[i];
							}
							se[(*start)+(*offset)]=key;
							//if(*start<flen)
							(*start)++;
						}
					}
					if(*start>(flen-1)){ //e.g. |ABC|D cursor on D after overstrike
						(*offset)++; 
						(*start)--;
						//results in |BCD| cursor on D by +*offset and -*start
					}
					validKey=0;
				}
				break;
			//}
		}
	}
	free(copy);
	return key;
}
