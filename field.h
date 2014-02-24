#ifndef _FIELD_H_   
   #define _FIELD_H_
extern "C" {
#include "console.h"
}

class Field{
	protected:
		int rows, cols, flength, slength, ctypes;
		int offs, begin;
		char *charStr;
		static int insert;
	public:
		Field(char *str, int row, int col, int flen, int slen, int offset=0, int start=0, int ctype=1); //Field initialization
		void draw( ) const; //displays the Field
		int draw(int row, int col, int l, int h, char edge, char top_btm, char ls_rs) const; //displays the field with a box 
		virtual bool editable( ) const; //returns whether the field may be edited
		virtual int edit( ); //displays the field and allows it to be edited if possible
		virtual char* data( ); //returns a pointer to the data in the field
		virtual ~Field(); //virtual deconstructor
};
#endif