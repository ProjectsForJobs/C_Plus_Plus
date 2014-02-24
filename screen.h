#ifndef _SCREEN_H_   
   #define _SCREEN_H_
extern "C" {
#include "console.h"
}
#include "field.h"
#include "textfield.h"
class Screen{
	private:
        Field  *field[80];
		bool dflag[80];
        int numfields;
    public:
        Screen(); //initiliation
        int add(Field *fp, int autodelete=0); //adds the Field *fp to the Screen. Returns the number of Fields in the Screen or 0 if adding was unsuccessful
        void display(int fnum=0); //displays the Field at fnum in the Screen. If 0, clears current displayed Fields and displays all Fields
        int edit(int *fpnum=NULL, int clear=0);
        int locate(Field *fp); //locate the Field *fp in the Screen. Returns the Field number in Screen or 0 if unsuccessful
        Field* fieldptr(int n); //returns the Field with number n in the Screen or null if n is invalid
		~Screen(); //deconstructor to delete fields
};
#endif