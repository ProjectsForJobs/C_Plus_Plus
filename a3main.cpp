/*************************************************************************/
/*                                                                       */
/* file:       a3main.cpp  version 1.0                                   */
/* written by: Danny Abesdris, Catherine Leung                           */
/* date:       July 15, 2012                                             */
/*                                                                       */
/* This is the official test main for OOP344 assignment #2 Winter 2009   */
/* This file may not be modified!                                        */
/*                                                                       */
/* Compilation instructions (see below)                                  */
/* On Windows (Visual Studio 2008 C/C++ Compiler):                       */
/* cl -c console.c -o console.obj   (creates: console.obj)               */
/* cl a3main.cpp screen.cpp console.obj  (creates: a3main.exe)           */
/*                                                                       */
/* On Linux (Matrix) (GNU GCC Compiler):                                 */
/* cc -c console.o (creates: console.o)                                  */
/* c++ a3main.cpp screen.cpp console.o -lncurses  (creates: a.out)       */
/*                                                                       */
/* Note that this program cannot verify that your routines               */
/* are correct, you must run the program and observe whether             */
/* or not it is working correctly.                                       */
/* ALSO IMPORTANT: In all your source files                              */
/* (including .h files)                                                  */
/* YOU MUST INCLUDE THE FOLLOWING STATEMENT                              */
/* (as part of the header comments):                                     */
/*                                                                       */
/* "I declare that the attached assignment is wholly my own              */
/* work in accordance with Seneca Academic Policy.  No part              */
/* of this assignment has been copied manually o  r                      */
/* electronically from any other source                                  */
/* (including web sites), other than the startup framework               */
/* provided in class and this statement."                                */
/*                                                                       */
/*            __________________________________                         */
/*            (signature) (print student name and ID)                    */
/*                                                                       */
/* Submissions which do not have this statement will have                */
/* to be resubmitted, even if they have no logic flaws.                  */
/*                                                                       */
/*************************************************************************/

/* DOWNLOAD C program using this link: a3main.cpp */

extern "C" {
   #include "console.h" // this extern directive will instruct the compiler
}                       // to use the C compiler to build object code for all
                        // of the functions prototyped in "console.h"

#include "screen.h"
#include <cstring>
#include <cstdio>
#include <cstdlib>

bool screen_check(void);
void test1(void);
void test2(char *);
void test3(void);

int main( ) {
   char str[3];
   consoleOn( );

   if(screen_check( )) {
      test1( );
      for(int i=0; i<7; i++) {
         sprintf(str, "%c", 'A' + i);
         test2(str);
      }
      test3( );
   }
   consoleOff( );
   return 0;
}

bool screen_check(void) {
   char msg[41];
   bool rc = true;
   if(consoleGetCols( )<80 || consoleGetRows( )<30) {
      consoleClear( );
      consoleMove(1,1);
      sprintf(msg, "screen dimensions are: %d by %d", consoleGetCols( ), consoleGetRows( ));
      consolePutS(msg);
      consoleMove(2,1);
      consolePutS("screen dimensions must be at least");
      consoleMove(3,1);
      consolePutS("80 columns by 30 rows!");
      consoleMove(4,1);
      consolePutS("Please resize your screen, and try again.");
      consoleMove(5,1);
      consolePutS("Press any key to quit...");
      consoleGetKey( );
      consoleOff( );
      rc = false;
   }
   return rc;
}

void test1(void) {
   Screen opening;
   Field *afield, *title;
   char str[30];
   int retval, i;

   title = new Field("Test 1: Displaying some labels...", 1, 25, 35, 0);
   retval = opening.add(title, 1);
   for(i=0; i<110 && retval; i++) {
      sprintf(str, "%d", i+2);
      afield = new Field(str, 4+i%18, 2+10*(i/18), strlen(str), 0);
      retval = opening.add(afield, 1);
      if(!retval) {
         delete afield;
      }
   }
   consoleClear( );
   opening.edit( ); // calling Screen's edit( ) function to navigate
                    // through Field's added
   if(i>80) {
      consoleMove(consoleGetRows( )-4, 10);
      consolePutS("Good! Looks like you removed the 80 field limit!");
      consoleMove(consoleGetRows( )-3, 10);
      consolePutS("A linked list perhaps or just fancy DMA?");
      consoleMove(consoleGetRows( )-2, 10);
      consolePutS("Press any key to go to the next test...");
      consoleGetKey( );
   }
}

void test2(char *letter) {
   Screen s2;
   Field *title, *afield, *ifield, *bottomscroll;
   char titlestr[40]="Test 2";
   char bottomstr[40]="TEST ", fstr[11];
   char num[3], str[30], msg[81];
   int i, snum, pos[7] = { 2, 2, 4, 4, 6, 6, 6 };
   char addon[40];
   char instr[7][70] = { "The following fields are numbered 1 to 7.",
                         "An * beside the field number indicates an editable",
                         "field.  The fields are not all editable!",
                         "The initial Field number...",
                         "(i.e. *fpnum of Screen::edit( )) is--> ",
                         "Check to see if you are starting at the right",
                         "place, then terminate editing to continue."
                       };

   snum = letter[0] - 'A' + 1;
   strcat(titlestr, letter);
   strcat(titlestr, ": Checking Starting Fields");
   sprintf(num, "%d", snum);
   strcat(instr[4], num);

   strcpy(addon, "Test 2");
   strcat(addon, letter);
   strcat(addon, ": Checking Starting Fields");
   title = new Field(titlestr, 1, consoleGetCols( )/2 - 20, 40, 0);

   sprintf(bottomstr, "TEST 2%s *fpnum is: %d", letter, snum);
   bottomscroll = new Field(bottomstr, 23, 10, strlen(bottomstr), 0);

   for(i=0; i<7; i++) {
      sprintf(fstr, "Field<%d>", i+1);
      strcpy(str, fstr);
      if(i%2) {
         strcat(str," *");
         afield=new Field(str, 13+i, 30, 15, 12);
      }
      else {
         afield=new Field(str, 13+i, 30, 15, 0);
      }
      s2.add(afield, 1);
   }
   s2.add(title, 1);
   s2.add(bottomscroll, 1);
   for(i=0; i<7; i++) {
      ifield = new Field(instr[i], 3+i, 10, 70, 0);
      s2.add(ifield, 1);
   }
   consoleClear( );
   sprintf(msg, "The cursor should be on the 'F' in Field<%d>", pos[snum-1]);
   consoleDraw(msg, 24, 10, 60);
   s2.edit(&snum, 0);
}

void test3(void) {
   Screen tf;
   Field *label1;
   TextField *story, *lyrics;
   char page1[ ] =
      "Once there lived a village of creatures along the bottom of a great crystal "
      "river. The current of the river swept silently over them all -- young and old, "
      "rich and poor, good and evil, the current going its own way, knowing only "
      "its own crystal self. Each creature in its own manner clung tightly to the "
      "twigs and rocks of the river bottom, for clinging was their way of life, and "
      "resisting the current what each had learned from birth. "
      "But one creature said at last, 'I am tired of clinging. Though I cannot see "
      "it with my eyes, I trust that the current knows where it is going. I shall "
      "let go, and let it take me where it will. Clinging I shall die of boredom.' "
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
      "were left alone making legends of a Saviour. --Richard Bach--",

   page2[ ] = "The Grand Illusion --STYX-- 1977 "
              "Welcome to the grand illusion "
              "Come on in and see what's happening "
              "Pay the price, get your tickets for the show "
              "The stage is set, the band starts playing "
              "Suddenly your heart is pounding "
              "Wishing secretly you were a star. "
              "But don't be fooled by the radio "
              "The TV or the magazines "
              "They show you photographs of how your life should be "
              "But they're just someone else's fantasy "
              "So if you think your life is complete confusion "
              "Because you never win the game "
              "Just remember that it's a grand illusion "
              "And deep inside we're all the same. "
              "We're all the same... "
              "So if you think your life is complete confusion "
              "Because your neighbours got it made "
              "Just remember that it's a grand illusion "
              "And deep inside we're all the same. "
              "We're all the same... "
              "America spells competition, join us in our blind ambition "
              "Get yourself a brand new motor car "
              "Someday soon we'll stop to ponder what on earth's this spell we're under "
              "We made the grade and still we wonder who the hell we are ";

   // slen=0 for this Field (thus not editable)
   label1 = new Field("See if scrolling through the TextFields below works correctly!", 5, 5, 65, 0);
   tf.add(label1, 1);

   story  = new TextField(page1, 7, 5, 30, 10); // TextField's include the bounding box
   tf.add(story, 1);

   lyrics = new TextField(page2, 7, 40, 32, 5);
   tf.add(lyrics, 1);

   consoleClear( );
   tf.edit(NULL, 1); // calling Screen's edit( ) function to navigate
                     // through Field's added

   consoleMove(19, 5);
   consolePutS("If scrolling through the TextField's above worked, and");
   consoleMove(20, 5);
   consolePutS("if your program has reached this point without crashing...");
   consoleMove(21, 5);
   consolePutS("It may just be ready to submit!!!");
   consoleMove(22, 5);
   consolePutS("Press any key to continue, and watchout for segmentation faults!");
   consoleGetKey( );
}