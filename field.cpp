
#include "field.h"

Field::Field(char *str, int row, int col, int flen,int slen, int offset, int start, int ctype){
	//int size = 0;	
	rows = row;
	cols = col;
	flength = flen;
    slength = slen;   
    ctypes = ctype;
    //size = strlen(str);
	begin = start;
	offs = offset;
    charStr = new char[slen+1];		//use slen instead of string length assuming editing happen later
	if (str!=0){
		strcpy(charStr,str);
	}
	//printf("begin:%d offs:%d string:%s", begin,offs,charStr);
	//getchar();

}

int Field::insert=1;  //original value is insert mode 

bool Field::editable( ) const{
	bool rc = false;
	if(slength > 0 && ctypes!=0)		//not offset>0. consider |Lord| which is 0 offset, it has to be allowed editing
	  rc = true;
	return rc;
}

int Field::edit( ){
	int *start= new int;
	int *offset= new int;
	int key;
	//start=(int*)begin;	
	//offset=(int*)offs;
	*start=begin;
	*offset=offs;
	//if(offs > 1)		
	if (editable()){	
	//use editable() instead coz it is virtual, it will use derived version of editable() e.g. textfield
		key=consoleEdit(charStr, slength, rows, cols, flength, start, offset, &insert, ctypes);	
		return key;
	}
	else{
	   consoleDraw(charStr,rows,cols,flength);
	   return 0;
	}
	 delete start;
	 delete offset;
}

void Field::draw( ) const{
	consoleDraw(charStr,rows,cols,flength);
}

int Field::draw(int row, int col, int l, int h,char edge, char top_btm, char ls_rs) const{
	consoleDraw(charStr,row,col,flength);
	consoleBox(row,col,l,h,edge,top_btm,ls_rs);
	return 1;
}

char* Field::data( ){
	return charStr;
}

Field::~Field(){
	if(charStr)
		delete [] charStr;
}
