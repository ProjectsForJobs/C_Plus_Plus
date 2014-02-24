
#include "textfield.h"
#include <fstream>
using namespace std;

class TextFieldLoader: public TextField, public fstream{
	public:	
		TextFieldLoader(const char* filename, int row, int col, int width, int height);	
		~TextFieldLoader();
};

