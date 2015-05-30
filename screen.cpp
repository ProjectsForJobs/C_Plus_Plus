
#include "screen.h"

Screen::Screen(){
	numfields=0;
}
int Screen::add(Field *fp, int autodelete){
	if (numfields<80){						//if fields array isn't full add the field to the Screen and increment numfields
		field[numfields]=fp;
		if (autodelete)						//if autodelete is true, set dflag to true  
			dflag[numfields]=true;	
		else								//if autodelete is false, set dflag to false
			dflag[numfields]=false;
		numfields++;
		return numfields;
	}
	return 0;
}
void Screen :: display(int fnum){
	if (fnum==0){
		consoleClear();						//if fnum is 0, clear the console
		for (int i=0;i<numfields;i++){		//draw all fields in the Screen
			field[i]->draw();
		}
	}
	else if (fnum>0 && fnum<=numfields){	//draw the field at fnum in the Screen
		field[fnum-1]->draw();
	}
}
int Screen:: edit(int *fpnum, int clear){
	bool exit = false, nullFpnum=false;
	int key, index=0;
	int editCon[80];
	int number;
	int y=0; 
	char test[50];
	for (int i=0; i<80;i++){
		editCon[i]=-1;
	}
	if(clear){								//if clear is true then calling screen::display function
		display();							//which clear the screen and display all the fields
	}
	else{
		for(int i=0; i<numfields; i++){		//if clear is false then just displaying all the fields without
			display(i);						//clearing the screen
		}
	}
	for(int x=0; x<numfields; x++){			
		if(field[x]->editable()){			//loop through all the fields to check wether its editable or not
			editCon[index]=x+1;				//if the field is editable, store the index number of the editable field
			index++;						//into a container called editCon which can be used to determine the editable
		}									//feilds in the screen
	}
	if(fpnum==NULL){
		nullFpnum=true;
		fpnum=new int(1);
	}
	while(!exit){
		if(editCon[0]==-1){					//if in the index #0(first) of array named editCon is empty
			key = consoleGetKey();			//the function will simply terminated without any editing
			exit = true;					//because empty editCon is equal to no editable field
		}
		else{								
			if(fpnum==NULL){						//fpnum == NULL,
				key = field[editCon[0]]->edit();	//start editing from the first editable field (field at index number of
				nullFpnum=true;
				fpnum=new int(1);
			}										//editCon[0]) without changing the value of the fpnum

			else{									//fpnum != NULL,
				if(*fpnum<editCon[index-1]){		//if fpnum is smaller than the last editable field
					for(int x=0; x<index; x++){
						/*consoleMove(0,0);
						printf("fpnum=%d editcon[%d]=%d",*fpnum,x,editCon[x]);
						getchar();*/
						if(*fpnum==editCon[x]){	//loop through the editCon array to find an index# of editable field
							*fpnum = editCon[x]; //that is nearest but greater than the fpnum provided,
												//and changes fpnum to the value of that index#
							number = x;			//copy the index number of editCon for further use in UP,DOWN,TAB,ENTER key
							break;
						}
						else if(*fpnum<editCon[x]){
							(*fpnum)++;
							x--;
						}
					}
					key = field[*fpnum-1]->edit();
				}
				else if(*fpnum >= editCon[index-1]){	//if fpnum is greater or equal to the last editable field
					*fpnum = editCon[index-1];		//change the fpnum to the last editable field
					key = field[*fpnum-1]->edit();
					number = index-1;				//copy the index number of the last editable field for 
				}									//use in UP,DOWN,TAB,ENTER key
			}
		}
		if(!exit){
		switch(key){
		case UP:
			if(fpnum==NULL){	//since fpnum==NULL edit the first editable field, pressing UP will end up at
				*fpnum = editCon[index-1];	//the last editable field. Thus change the fpnum into the last editable field#
			}
			else if(number==0){		//if number is 0, that mean right now we are at the first editable field and 
				*fpnum = editCon[index-1];	//pressing UP will makes the next field to be edited is the last editable field
			}
			else{
				number--;		//since number is a copy of an index of editCon which pointed by fpnum, pressing UP will move the
				*fpnum = editCon[number];	//pointed index to the previous one. (move to previous editable field)
			}
			break;
		case DOWN:
		case TAB:
			if(fpnum==NULL){	//since fpnum==NULL edit the first editable field, pressing DOWN/TAB will move to next editable field
				if(editCon[1]!=-1){	//or index #1 of editCon only if there are other fields that is editable,
					*fpnum = editCon[1]; // if there are no other fields that is editable, pressing DOWN/TAB wont change anything.
				}
			}
			else if(number==(index-1)){	//if number is equal to index-1, that mean right now we are at the last editable field and
				*fpnum = editCon[0];	// pressing DOWN/TAB will makes the next field to be edited is the first editable field
			}
			else{
				number++;		//because number is a copy of an index of editCon which pointed by fpnum, pressing DOWN/TAB will
				*fpnum = editCon[number]; //move the pointed index to the next one. (move to next editable field)
			}
			break;
		case ENTER:
			if(fpnum==NULL){	//since fpnum==NULL edit the first editable field, pressing ENTER will move to next editable field
				if(editCon[0]!=-1){	//or index #1 of editCon only if there are other fields that is editable. 
					*fpnum = editCon[0];	
				}
				else{					//If there are no other fields to be edited, pressing ENTER will terminated the function
					exit = true;
				}
			}
			else if(number==(index-1)){ //if number is equal to index-1, that mean right now we are at the last editable field and
				exit = true;	//pressing ENTER will terminate the function
			}
			else{			//since the fpnum is not pointing at the last editable field, pressing enter will move it to the next
				number++;	//editable field
				*fpnum = editCon[number];
			}
			break;
		case PGUP:
		case PGDN:
		case ESC:
		case F1:
		case F2:
		case F3:		//simple exit the function when one of these keys is pressed
		case F4:
		case F5:
		case F6:
		case F7:
		case F8:
		case F9:
		case F10:
			exit = true;
			break;
		}
		}
	}
	if(nullFpnum)
		delete fpnum;
	return key;
}

int Screen:: locate(Field *fp){
	for(int i=0; field[i]; i++){
		if(field[i]==fp)					//loop through the field array and find a field equal to fp
			return i+1;
	}
	return 0;
}
Field* Screen :: fieldptr(int n){
	if(field[n-1])							//return the field in the array at n
		return field[n-1];
	return NULL;
}
Screen::~Screen(){
	for (int i=0; i<numfields;i++){			//delete any fields where the corresponding dflag is true
		if(dflag[i])
			delete [] field[i];
	}
}
