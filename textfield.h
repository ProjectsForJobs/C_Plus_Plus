#ifndef _TEXTFIELD_H_   
   #define _TEXTFIELD_H_
#include "field.h"
extern "C" {
#include "console.h"
}

class TextField: public Field{
	//char* page;
	//int row;			
	//int col;		
	//int width;	//inherited from field's charStr, rows, cols and flength
	int box_height;
	char box_edge;
	char box_top;
	char box_side;
  public:
	TextField(char *page, int row, int col, int width,
          int height, char edge='+', char top_btm='-', char ls_rs='|');
	int edit( );
	virtual ~TextField();
    bool editable( ) const;
	char* data( );
	void display(int lineOffset);
};
#endif