
#include "textfield.h"

TextField::TextField(char *page, int row, int col, int width, 
	int height, char edge, char top_btm, char ls_rs):Field(page, row, col, width, width){
		box_height=height;
		box_edge=edge;
		box_top=top_btm;
		box_side=ls_rs;
}

TextField::~TextField(){
}

bool TextField::editable( ) const{
	return true;
}

char* TextField::data( ){
	if(charStr)
		return charStr;
	else return 0;
}

void TextField::display(int lineOffset){
	int startindex=0;
	int indexspace=startindex;
	int endindex=0;
	int drawcol=cols+1;
	int drawrow=rows+1;//rows+1, cols+1 is where 1st char should be put
	int endcol=cols+flength+1;	//col occupied by border char
	consoleMove(drawrow, drawcol);
	

	/*Display the textfield*/
	while ((charStr[indexspace]!='\0')&&(drawrow!=(rows+box_height+1))){
	//to test if it is end of array, no-continue, yes-stop (combine with last statement)
	//stop searching whitespace if drawrow reaches box bottom border
		
		/*search whitespace index*/
		while((charStr[indexspace]!=' ') && (charStr[indexspace]!='\t') && (charStr[indexspace]!='\0')){
			indexspace++;
		} //index of whitespace is found
		endindex=indexspace-1; 
		
		/*case1: word length is greater than flength*/
		if (endindex-startindex+1>flength){	
			//case1A:
			while ((startindex!=endindex) && (drawrow!=(rows+box_height+1))){
			//stop displaying if drawrow reaches box bottom border 
				for ( ; drawcol<endcol; drawcol++,startindex++){
					consolePutC(charStr[startindex]);	//remember everytime putC is _word, not word
				}
				drawrow++;
				drawcol=cols+1;
				consoleMove(drawrow,drawcol-1);
			}
		}
		/*case2: word length is lesser than flength*/
		else {
			//case2A: has to put the word into next line
			if ((drawcol+(endindex-startindex+1)-1) >= endcol){ //drawcol=4, str length=3, draw to 6, endcol=6
				drawrow++;	//start scrolling if lineOffset==1
				drawcol=cols+1;
				if(lineOffset==0){
					consoleMove(drawrow,drawcol);	
					startindex++;//because putC _word, so that -1 to eliminate the space
				}
				if(lineOffset>=1){
					drawrow=drawrow-1;	//for below condition, for holding drawrow as rows+1
					consoleMove(drawrow, drawcol);	//for holding drawrow as rows+1
					startindex++;//because putC _word, so that -1 to eliminate the space
					lineOffset--;
				}
				//stop displaying if drawrow reaches box bottom border and lineOffset==0 (show the lines)
				if ((drawrow!=(rows+box_height+1))){
					for ( ; startindex<=endindex; drawcol++,startindex++){
						if (lineOffset==0){
							consolePutC(charStr[startindex]);
						}
					}
				}
			}
			//case2B: can be put into the same line
			else{
				for ( ; startindex<=endindex; drawcol++,startindex++){
					if (lineOffset==0){
						consolePutC(charStr[startindex]);
					}
				}
			}
		}
		
		/*initialization for next word*/
		if ((charStr[indexspace]!='\0')){	
			indexspace++; 
		}
	}
	consoleBox(rows, cols, flength+2,box_height+2, box_edge, box_top, box_side);
	consoleMove(rows+1, cols+1);

}


int TextField::edit( ){
	int lineOffset=0;
	display(lineOffset);
	//finish 1st display and await user input
	bool done=false;
	int key;
	int cursor_row=rows+1;
	while(!done){
		key= consoleGetKey();
		switch(key){
			case DOWN:
				//case1: normal down
				if(cursor_row+1<rows+box_height+1){	
					consoleMove(cursor_row+1,cols+1);
					cursor_row++;
				}
				else{
				//case2: scrolling= display with different startindex by using above display method
					lineOffset++;
					consoleClear();
					display(lineOffset);
					consoleMove(rows+box_height,cols+1);
					cursor_row=rows+box_height;
				}
				break;
			case UP:
				//case1: normal up
				if(cursor_row-1>rows){	
					consoleMove(cursor_row-1,cols+1);
					cursor_row--;
				}
				else{
				//case2: scrolling= display with different startindex by using above display method
					lineOffset--;
					consoleClear();
					display(lineOffset);
					consoleMove(rows+1,cols+1);
					cursor_row=rows+1;
				}
				break;
			case PGDN:
				//case2: scrolling= display with different startindex by using above display method
					lineOffset=lineOffset+box_height;
					consoleClear();
					display(lineOffset);
					consoleMove(rows+box_height,cols+1);
					cursor_row=rows+box_height;
				break;
			case PGUP:
				//case2: scrolling= display with different startindex by using above display method
					lineOffset=lineOffset-box_height;
					consoleClear();
					display(lineOffset);
					consoleMove(rows+1,cols+1);
					cursor_row=rows+1;
				break;
			case HOME:
				//case2: scrolling= display with different startindex by using above display method
					lineOffset=0;
					consoleClear();
					display(lineOffset);
					consoleMove(rows+1,cols+1);
					cursor_row=rows+1;
				break;
			case END:
				//case2: scrolling= display with different startindex by using above display method
					lineOffset=lineOffset-box_height;
					consoleClear();
					display(lineOffset);
					consoleMove(rows+box_height,cols+1);
					cursor_row=rows+box_height;
				break;
		case ENTER:
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
		}	//end switch
	}	//end while
	return key;
}	//end edit()


