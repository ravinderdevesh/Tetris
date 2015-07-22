#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <time.h>
#include <sstream>
#include <vector>
using namespace std;

static const int tilesize = 30 ;
static const int bgcolor = 97 ;
static const int xmaxtiles = 14 ;
static const int ymaxtiles = 20 ;
static const double timeout = 0.5 ;

class Tile {
	int x; int y;	// x and y position of a given tile on board matrix
	public :
	void setx(int a) { 	//function to set the value of x to a given value
		x = a ;
	}
	void sety(int b) { 	//function to set the value of y to a given value
		y = b ;
	}
	void increase_x(int a) {
		x = x + a ;
	}
	void increase_y(int b) {
		y = y + b ;
	}
	int getx() { 	//function to get the value of x
		return x ;
	}
	int gety() {	//function to get the value of y
		return y ;
	}
} ;

class ActivePiece { 	//ActivePiece class defines the properties of the current piece
	Tile tiles[4] ;		//each piece has 4 tiles
	int color ;			//color of active piece
	char type ;			//type of active piece (there are 7 types of active piece)
	public :
	ActivePiece() {		//constructor
		srand(time(NULL)) ;		//time seeded random
		do
			color = rand() % 256 ;
		while(color == bgcolor || color == 56 || color == (bgcolor + 1) || color == (bgcolor - 2) || color == 0) ;		
		//the color should not be background color or black or other visually bad colors
		unsigned char num = rand() % 7 ;
		tiles[0].setx(0) ; tiles[0].sety(5) ;
		tiles[1].setx(0) ; tiles[1].sety(6) ;
		tiles[2].setx(0) ; tiles[2].sety(7) ;
		tiles[3].setx(0) ; tiles[3].sety(8) ;
		switch(num) {		//this switch judges the type of tile
			case 0 : type = 0 ;
					 break ;
			case 1 : tiles[0].setx(0) ; tiles[0].sety(xmaxtiles/2) ;
					 tiles[1].setx(1) ; tiles[1].sety(tiles[0].gety()) ;
					 tiles[2].setx(1) ; tiles[2].sety(tiles[1].gety() + 1) ;
					 tiles[3].setx(2) ; tiles[3].sety(tiles[2].gety()) ;
					 type = 1 ;
					 break ;
			case 2 : tiles[0].setx(0) ; tiles[0].sety(xmaxtiles/2) ;
					 tiles[1].setx(1) ; tiles[1].sety(tiles[0].gety()) ;
					 tiles[2].setx(1) ; tiles[2].sety(tiles[1].gety() - 1) ;
					 tiles[3].setx(2) ; tiles[3].sety(tiles[2].gety()) ;
					 type = 2 ;
					 break ;
			case 3 : tiles[0].setx(0) ; tiles[0].sety(xmaxtiles/2) ;
					 tiles[1].setx(0) ; tiles[1].sety(tiles[0].gety() + 1) ;
					 tiles[2].setx(0) ; tiles[2].sety(tiles[1].gety() + 1) ;
					 tiles[3].setx(1) ; tiles[3].sety(tiles[2].gety()) ;
					 type = 3 ;
					 break ;
			case 4 : tiles[0].setx(0) ; tiles[0].sety(xmaxtiles/2) ;
					 tiles[1].setx(0) ; tiles[1].sety(tiles[0].gety() - 1) ;
					 tiles[2].setx(0) ; tiles[2].sety(tiles[1].gety() - 1) ;
					 tiles[3].setx(1) ; tiles[3].sety(tiles[2].gety()) ;
					 type = 4 ;
					 break ;
			case 5 : tiles[0].setx(0) ; tiles[0].sety(xmaxtiles/2) ; 	//cannot rotate
					 tiles[1].setx(0) ; tiles[1].sety(tiles[0].gety() + 1) ;
					 tiles[2].setx(1) ; tiles[2].sety(tiles[1].gety()) ;
					 tiles[3].setx(1) ; tiles[3].sety(tiles[2].gety() - 1) ;
					 type = 5 ;
					 break ;
			case 6 : tiles[0].setx(0) ; tiles[0].sety(xmaxtiles/2 - 1) ;
					 tiles[2].setx(0) ; tiles[2].sety(tiles[0].gety() + 1) ;
					 tiles[1].setx(0) ; tiles[1].sety(tiles[2].gety() + 1) ;
					 tiles[3].setx(1) ; tiles[3].sety(tiles[2].gety()) ;
					 type = 6 ;
					 break ;
		}
	}
	int get(unsigned short i , unsigned short var) { 		//1 means x and 2 means y
		if(var == 1)
			return tiles[i].getx() ;
		else if(var == 2)
			return tiles[i].gety() ;
		else
			return -1 ;
	}
	int getColor() {  		//returns color of active piece
		return color ;
	} ;
	char getType() {		//returns type of active piece
		return type ;
	}
	void ts_down() {		//function to move the active piece down
		for(unsigned short i = 0 ; i < 4 ; i++)
			tiles[i].increase_x(1) ;
	}
	void ts_right(int occup[ymaxtiles][xmaxtiles] , bool occupied[ymaxtiles][xmaxtiles]) {		//function to move the active piece to the right
		if(ts_rightok()) {		//check if it is possible to move right
			for(unsigned short i = 0 ; i < 4 ; i++) {
				if(occupied[tiles[i].getx()][tiles[i].gety() + 1] == true)		//if the position to the right is occupied, then don't move
					return ;
			}
			for(unsigned short i = 0 ; i < 4 ; i++)		//if everything is fine then move right by one step
				tiles[i].increase_y(1) ;
		}
	}
	void ts_left(int occup[ymaxtiles][xmaxtiles] , bool occupied[ymaxtiles][xmaxtiles]) {		//function to move the active piece to the left
		if(ts_leftok()) {		//check if it is possible to move right
			for(unsigned short i = 0 ; i < 4 ; i++) {
				if(occupied[tiles[i].getx()][tiles[i].gety() - 1] == true)		//if the position to the left is occupied, then don't move
					return ;
			}
			for(unsigned short i = 0 ; i < 4 ; i++)		//if everything is fine then move left by one step
				tiles[i].increase_y(-1) ;
		}
	}
	bool twist(unsigned short rotated[][2] , unsigned short current[][2] , unsigned short px , unsigned short py , int occup[ymaxtiles][xmaxtiles] , bool occupied[ymaxtiles][xmaxtiles]) { //check if rotatable
		bool rotateit = true ;
		for(unsigned short i = 0 ; i < 3 ; i++) {
			rotated[i][0] = px + py - current[i][1] ;
			rotated[i][1] = py - px + current[i][0] ;
		}
		for(unsigned short i = 0 ; i < 3 ; i++) {
			//can't rotate if peice moves out of board or if it overlaps with some other piece
			if(rotated[i][0] >= ymaxtiles || rotated[i][0] <= 0 || rotated[i][1] >= xmaxtiles || rotated[i][1] <= 0 || occupied[rotated[i][0]][rotated[i][1]] == true)
				return false ;
		}
		return true ;
	}
	void ts_rotateCW(int occup[ymaxtiles][xmaxtiles] , bool occupied[ymaxtiles][xmaxtiles]) { //function to rotate clockwise
		unsigned short rotated[3][2] ;
		unsigned short current[3][2] ;
		unsigned short px , py ;
		if(type == 0 || type == 1 || type == 2 || type == 3 || type == 4 || type == 6) { //type 5 piece aren't rotatable
			px = tiles[2].getx() ; py = tiles[2].gety() ;
			current[0][0] = tiles[0].getx() ; current[0][1] = tiles[0].gety() ;
			current[1][0] = tiles[1].getx() ; current[1][1] = tiles[1].gety() ;
			current[2][0] = tiles[3].getx() ; current[2][1] = tiles[3].gety() ;
			if(twist(rotated , current , px , py , occup , occupied)) {
				tiles[0].setx(rotated[0][0]) ; tiles[0].sety(rotated[0][1]) ;
				tiles[1].setx(rotated[1][0]) ; tiles[1].sety(rotated[1][1]) ;
				tiles[3].setx(rotated[2][0]) ; tiles[3].sety(rotated[2][1]) ;
			}
		}
	}
	void ts_rotateCCW(int occup[ymaxtiles][xmaxtiles] , bool occupied[ymaxtiles][xmaxtiles]) { //function to rotate counter clockwise
		for(unsigned short i = 0 ; i < 3 ; i++)
			ts_rotateCW(occup , occupied) ;
	}
	bool ts_rightok() { //checks if it can move to the right
		for(unsigned short i = 0 ; i < 4 ; i++)
			if(tiles[i].gety() == (xmaxtiles - 1))
				return false ;
		return true ;
	}
	bool ts_leftok() { //checks if it can move to the right
		for(unsigned short i = 0 ; i < 4 ; i++)
			if(tiles[i].gety() == 0)
				return false ;
		return true ;
	}
} ;

void timeractions(void *p) ;
class Board : public Fl_Widget { //the board class
	bool occupied[ymaxtiles][xmaxtiles] ; //array that stores 1 for positions that are occupied and 0 if unoccupied
	int occup[ymaxtiles][xmaxtiles] ; //array that stores color of each position on board
	int nextoccup[5][5] ;	//the grid to display the next piece
	ActivePiece currentpiece ;	//the current piece object
	ActivePiece nextPiece ;	//the next piece object
	char* scoreLabel ;	//score label
	Fl_Box* scoreBox ;	//score box
	int score ;		//stores score
	public :
	Board() : Fl_Widget (0 , 0 , xmaxtiles * tilesize + 300 , ymaxtiles * tilesize , "Tetris") { //constructor
		for(unsigned short i = 0 ; i < ymaxtiles ; i++) {
			for(unsigned short j = 0 ; j < xmaxtiles ; j++) {
				occup[i][j] = bgcolor ;	//board is empty
				occupied[i][j] = false ;
			}
		}
		for(unsigned short i = 0 ; i < 5 ; i++) {
			for(unsigned short j = 0 ; j < 5 ; j++)
				nextoccup[i][j] = bgcolor + 1 ;	//the display part is empty initially
		}
		score = 0 ;
	}
	void setScoreBox(Fl_Box** sb) { //function to set the scorebox
		scoreBox = *sb ;
		score = 0;
		scoreLabel = (char*) malloc(sizeof(char)*10);
	}
	private :
	bool isFree() { 	//returns true if the piece has permission to move down
		for(unsigned short i = 0 ; i < 4 ; i++) {
			//the position below the current piece should be unoccupied
			if((currentpiece.get(i , 1) >= ymaxtiles) || (occupied[currentpiece.get(i , 1) + 1][currentpiece.get(i , 2)] == true) || (occup[currentpiece.get(i , 1) + 1][currentpiece.get(i , 2)] != bgcolor && occup[currentpiece.get(i , 1) + 1][currentpiece.get(i , 2)] != currentpiece.getColor()))
				return false ;
		}
		return true ;
	}
	void transferTiles() {		//transfers the tiles to the board
		for(unsigned int i = 0 ; i < 4 ; i++) {
			occup[currentpiece.get(i , 1)][currentpiece.get(i , 2)] = currentpiece.getColor() ;
			occupied[currentpiece.get(i , 1)][currentpiece.get(i , 2)] = true ;
		}
		deleteRow() ; //delete all the rows that are complete
		draw() ; //redraw the board
	}
	void deleteRow() {		//deletes the rows that have been completed
		bool b = true ;
		for(unsigned int i = 0 ; i < ymaxtiles ; i++) {
			b = true ;
			//checks if the particular row is full or not
			for(unsigned int j = 0 ; j < xmaxtiles ; j++) {
				if(occupied[i][j] == false)
					b = false ;
			}
			if(b == true) { //if the row is full then proceed below
				for(unsigned short j = i ; j > 0 ; j--) {
					for(unsigned short k = 0 ; k < xmaxtiles ; k++) {
						occup[j][k] = occup[j - 1][k] ; //shif the rows down
						occupied[j][k] = occupied[j - 1][k] ;
					}
				}
				for(unsigned short k = 0 ; k < xmaxtiles ; k++) {
					occup[0][k] = bgcolor ; //generate a new row at the top
					occupied[0][k] = false ;
				}
				score = score + 10 ;
			}
		}
	}
	void draw() { //the draw function that draws everything on the board
		for(unsigned short i = 0 ; i < ymaxtiles ; i++) {
			for(unsigned short j = 0 ; j < xmaxtiles ; j++) {
				if(occup[i][j] != bgcolor)
					fl_draw_box(FL_BORDER_BOX , j * tilesize , i * tilesize , tilesize , tilesize , occup[i][j]); //border boxes for active pieces
				else
					fl_draw_box(FL_FLAT_BOX , j * tilesize , i * tilesize , tilesize , tilesize , occup[i][j]); //plain boxes for background
			}
		}
		//display the next piece
		for(unsigned short i = 0 ; i < 5 ; i++) {
			for(unsigned short j = 0 ; j < 5 ; j++) {
				if(nextoccup[i][j] != (bgcolor + 1))
					fl_draw_box(FL_BORDER_BOX , (j + xmaxtiles + 4 )* tilesize , (i + ymaxtiles / 2 + 2)* tilesize , tilesize , tilesize , nextoccup[i][j]);
				else
					fl_draw_box(FL_FLAT_BOX , (j + xmaxtiles + 4 )* tilesize , (i + ymaxtiles / 2 + 2)* tilesize , tilesize , tilesize , nextoccup[i][j]);
			}
		}
		fl_draw_box(FL_FLAT_BOX , xmaxtiles * tilesize + 5 , 0 , 10 , ymaxtiles * tilesize , bgcolor - 2) ; //the partitioning line
	}
	void putTile() { //transfer the colors to board
		for(unsigned short j = 0 ; j < 4 ; j++) {
			occup[currentpiece.get(j , 1)][currentpiece.get(j , 2)] = currentpiece.getColor() ;
		}
	}
	int handle(int e) { //event handling function
		if(e == FL_KEYDOWN) { //if event is key down
			switch(Fl::event_key()) {
				case FL_Escape: exit (1);  // Esc key
				case FL_Left: deleteTile() ;currentpiece.ts_left(occup , occupied); putTile() ;break;  // left arrow
				case FL_Right: deleteTile() ;currentpiece.ts_right(occup , occupied); putTile() ;break;  // left arrow
				case FL_Up : deleteTile() ; currentpiece.ts_rotateCW(occup , occupied); putTile() ;break; 	//rotate ClockWise
				case FL_Down : deleteTile() ; currentpiece.ts_rotateCCW(occup , occupied); putTile() ;break; 	//rotate Counter ClockWise
				case 32 : while(isFree() == true) {
								deleteTile() ; //delete the tile from it's original position
								currentpiece.ts_down() ; //move it down
								putTile() ; //update the board
						  }
			}
		}
		redraw() ; //draw the updated board
	}
	bool generatePiece() { //function that generates a new piece
		currentpiece = nextPiece ; //the next piece now plays the role of current piece
		for(unsigned short i = 0 ; i < 4 ; i++) {
			if(occup[currentpiece.get(i , 1)][currentpiece.get(i , 2)] != bgcolor)
				return false ; //piece could not be generated
		}
		return true ; //piece successfully generated
	}
	void deleteTile() { //deletes the acive piece from it's original position
		for(unsigned short j = 0 ; j < 4 ; j++) {
			occup[currentpiece.get(j , 1)][currentpiece.get(j , 2)] = bgcolor ;
		}
	}
	int periodic() { //the periodic function repeats itself after a fixed time interval
		if(isFree() == true) { //if the piece can move down then let it move in the 3 steps mentioned below
			deleteTile() ; //delete it from it's original position
			currentpiece.ts_down() ; //move it down
			putTile() ; //tell the board that it has moved
		}
		else { //if the piece can't move then it means it has reached it's bottom limit
			transferTiles() ; //shift the piece to the board
			if(generatePiece() == false) { //generate new piece and check if it is geneated
				//game ends.
				stringstream strs;
				strs << (int)score;
				string temp_str = strs.str();
				strcpy(scoreLabel,"Gameover. Score : ");
				strcat(scoreLabel,(char*) temp_str.c_str() );
				
				scoreBox->label(scoreLabel);
				redraw() ;
				Fl :: wait() ;
				exit(1) ;
				Fl::repeat_timeout (0.5,timeractions,this);
			}
			//if new piece is generated successfully then proceed with it
			ActivePiece newPiece ; //create a new piece
			nextPiece = newPiece ; //the next piece will act as new piece
			for(unsigned short i = 0 ; i < 5 ; i++) {
				for(unsigned short j = 0 ; j < 5 ; j++)
					nextoccup[i][j] = bgcolor + 1 ; //show the next piece on the board
			}
			switch(nextPiece.getType()) { //checks the type of new piece and acordingly tells the board to store it
				case 0 : nextoccup[2][0] = nextoccup[2][1] = nextoccup[2][2] = nextoccup[2][3] = nextPiece.getColor() ;
						 break ; 
				case 1 : nextoccup[1][1] = nextoccup[2][1] = nextoccup[2][2] = nextoccup[3][2] = nextPiece.getColor() ;
						 break ;
				case 2 : nextoccup[1][2] = nextoccup[2][2] = nextoccup[2][1] = nextoccup[3][1] = nextPiece.getColor() ;
						 break ;
				case 3 : nextoccup[1][1] = nextoccup[1][2] = nextoccup[1][3] = nextoccup[2][3] = nextPiece.getColor() ;
						 break ;
				case 4 : nextoccup[1][1] = nextoccup[1][2] = nextoccup[1][3] = nextoccup[2][1] = nextPiece.getColor() ;
						 break ;
				case 5 : nextoccup[2][1] = nextoccup[2][2] = nextoccup[3][1] = nextoccup[3][2] = nextPiece.getColor() ;
						 break ;
				case 6 : nextoccup[1][1] = nextoccup[1][2] = nextoccup[1][3] = nextoccup[2][2] = nextPiece.getColor() ;
						 break ;
			}
		}
		//show score
		stringstream strs;
		strs << (int)score;
		string temp_str = strs.str();
		strcpy(scoreLabel,"Score : ");
		strcat(scoreLabel,(char*) temp_str.c_str() );

		scoreBox->label(scoreLabel);
		redraw() ;
		Fl::repeat_timeout (timeout,timeractions,this);
	}
	friend void timeractions(void* p) ;
} ;
void timeractions(void *p) { //this is the function that provides periodicity to the program
	((Board *)p)->periodic ();
}
int main(int argc, char *argv[]) {
    Fl_Window *window = new Fl_Window (800 , ymaxtiles * tilesize , "TETRIS") ; //new window for the game
	window -> color(bgcolor) ;
	Board* b = new Board() ;
	Fl_Box* scorebox = new Fl_Box(tilesize*xmaxtiles+25,50,350,200,"Score : 0\0") ;
	scorebox -> box(FL_UP_BOX) ;
    scorebox -> labelfont(FL_BOLD+FL_ITALIC) ;
    scorebox -> labelsize(26) ;
    scorebox -> labeltype(FL_ENGRAVED_LABEL) ;
	b -> setScoreBox(&scorebox) ; // setting scorebox created in main to class. Board
	window -> end() ; 
   	window -> show() ;
	Fl :: add_timeout(0.1, timeractions,b) ;
    	return(Fl::run()) ;
}
