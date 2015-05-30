#include "screen.h"
extern "C" {
  #include "console.h"
}

int main(){
	int *test=new int;
	*test=1;
	consoleOn();
	Field *field1=new Field("hello",5,5,20,20,2,1,1);
	Field *field2=new Field("hi there", 4,5,10,20,0,0,1);
	Screen screen1;
	screen1.add(field1);
	screen1.add(field2);
	screen1.edit(test,0);
	consoleClear();
	consoleOff();
	getchar();
	delete field1;
	delete field2;
	delete test;

}