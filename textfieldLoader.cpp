#include "textfieldLoader.h"

TextFieldLoader::TextFieldLoader(const char* filename, int row, int col, int width, int height):
TextField(0, row, col, width, width){
	fstream inf;
	//char* buffer;
	//open file
	inf.open(filename, ios::binary);
	//find length of the file to allocate memory
	inf.seekg (0, ios::end);
	int length=inf.tellg();
	inf.seekg(0, ios::beg);
	//allocate memory
	//buffer = new char [length];
	//read the content of the file into charStr (Field's field)
	//strcpy(charStr, buffer);
	inf.read (charStr,length);
}
TextFieldLoader::~TextFieldLoader(){
	//close file
	close();
	//delete dynamic memory
	delete[] charStr;
}
