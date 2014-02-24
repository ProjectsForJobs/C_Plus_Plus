#pragma once
#include <iostream>
#include "console.h"
#include "field.h"
#include "textfield.h"
using namespace std;

int main(){
	char page[ ] =
"Once there lived a village of creatures along the bottom of a great crystal " 
"river. The current of the river swept silently over them all - young and old, "
"rich and poor, good and evil, the current going its own way, knowing only "
"its own crystal self. Each creature in its own manner clung tightly to the "
"twigs and rocks of the river bottom, for clinging was their way of life, and "
"resisting the current what each had learned from birth. "
"But one creature said at last, 'I am tired of clinging. Though I cannot see "
"it with my eyes, I trust that the current knows where it is going. I shall "
"let go, and let it take me where it will. Clinging I shall die of boredom' "
"The other creatures laughed and said, 'Fool! Let go and that current you "
"worship will throw you tumbled and smashed across the rocks, and you will "
"die quicker than boredom!' But the one heeded them not, and taking a breath "
"did let go, and at once was tumbled and smashed by the current across the rocks. "
"Yet in time, as the creature refused to cling again, the current lifted him "
"free from the bottom, and he was bruised and hurt no more. "
"And the creatures downstream, to whom he was a stranger cried, 'See a miracle! "
"A creature like ourselves, yet he flies! See the Messiah come to save us all!' "
"And the one carried in the current said, 'I am no more Messiah than you. The "
"river delights to lift us free, if only we dare let go. Our true work is this "
"voyage, this adventure.' But they cried the more, 'Saviour!' all the while "
"clinging to the rocks, and then when they looked again he was gone, and they "
"were left alone making legends of a Saviour. --Richard Bach--";


	char page2[]=
"Professor Catherine is pretty, beautiful. Professor Catherine is pretty, beautiful. "
"Professor Catherine is pretty, beautiful. Professor Catherine is pretty, beautiful. ";
	TextField x(page, 5, 5, 50, 10);
	x.edit( );
	//TextField y(page2, 5,5,10,10);
	//y.edit();

	//cout << " Edit() " << endl;
	//int x=0;
	char* test="editing";
	char* test2=new char[6];
	strcpy(test2, test);
//	consoleEdit(test2, 7, 0, 0, 5, NULL, NULL, &x, 1);
	int off=8;	//check offset>length
	//consoleEdit("Editing", 5, 0, 0, 6, NULL, &off, NULL, 1);
	return 0;
}
	