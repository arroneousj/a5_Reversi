// a5.cpp

#include "cmpt_error.h"
#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <algorithm>


using namespace std;

//*****************************************************************************************
// from class example

enum class Cell {
	empty,
	black,
	white,
	move,
	DEBUG
};

// foreground colours
const string red = "\033[0;31m";
const string green = "\033[1;32m";
const string yellow = "\033[1;33m";
const string pink = "\033[38;5;206m";
const string cyan = "\033[0;36m";
const string magenta = "\033[0;35m";
const string reset_color = "\033[0m";

// generate "pieces" with corresponding colours
const string empty_str = pink + "." + reset_color;
const string white_str = green + "0" + reset_color;
const string black_str = red + "X" + reset_color;
const string move_str = yellow + "#" + reset_color;

const string DEBUG_str = cyan + "@" + reset_color;


string to_str(Cell c) {
	switch(c) {
		case Cell::empty : return empty_str;
		case Cell::white : return white_str;
		case Cell::black : return black_str;
		case Cell::move : return move_str;
		case Cell::DEBUG : return DEBUG_str; 
	}
	cmpt::error("to_str error");
	return "";
}

typedef vector<vector<Cell>> Board;

Board make_empty_board(){
	Board result = {
		{Cell::empty, Cell::empty, Cell::empty, Cell::empty, Cell::empty, Cell::empty, Cell::empty, Cell::empty},
		{Cell::empty, Cell::empty, Cell::empty, Cell::empty, Cell::empty, Cell::empty, Cell::empty, Cell::empty},
		{Cell::empty, Cell::empty, Cell::empty, Cell::empty, Cell::empty, Cell::empty, Cell::empty, Cell::empty},
		{Cell::empty, Cell::empty, Cell::empty, Cell::white, Cell::black, Cell::empty, Cell::empty, Cell::empty},
		{Cell::empty, Cell::empty, Cell::empty, Cell::black, Cell::white, Cell::empty, Cell::empty, Cell::empty},
		{Cell::empty, Cell::empty, Cell::empty, Cell::empty, Cell::empty, Cell::empty, Cell::empty, Cell::empty},
		{Cell::empty, Cell::empty, Cell::empty, Cell::empty, Cell::empty, Cell::empty, Cell::empty, Cell::empty},
		{Cell::empty, Cell::empty, Cell::empty, Cell::empty, Cell::empty, Cell::empty, Cell::empty, Cell::empty}
	};
	return result;
}

int count(const Board& board, Cell a){
	int result = 0;
	for (const vector<Cell>& row : board)
	{
		for (Cell c : row)
		{
			if(c == a) result++;
		}
	}
	return result;
}

void display(const Board& board){
	cout << "    ";
	for (int i = 0; i < 8; ++i)
	{
		cout << (i+1) << " ";
	}
	cout << "\n";

	char r = 'A';
	for (const vector<Cell>& row : board)
	{
		cout << "  " << r << " ";
		r++;
		for (Cell c : row)
		{
			cout << to_str(c) << " ";
		}
		cout << "\n";
	}

	cout << "\n     "
		 << white_str << " " << count(board, Cell::white)
		 << "    "
		 << black_str << " " << count(board, Cell::black)
		 << "\n" << "\n";
}

// location storing x y cordinate of each point
struct Loc {
	int x;
	int y;
};

// from class example
//*****************************************************************************************

// player abstract base class
class Player {
public:
	virtual void make_move(Board& board, vector<Loc>& valid_spaces) = 0;
	virtual bool get_skip_status() const = 0;
	// destructor
	virtual ~Player() {}
};

// class for human player
// piece colour BLACK
class Human_player : public Player {
private:
	int move_row = 0; // y value
	int move_col = 0; // x value
	bool move_skipped = false; // whether player's turn was skipped

	bool get_move(Board& board, vector<Loc>& valid_spaces) {
		bool move_made = false;

		// get move from user
		string move_input;
		cout << "with letter indicating Row [A-H] and number indicating Collumn [1-8] ie. D3 \n"
			 << "enter position to place piece: ";
		getline (cin, move_input);
		transform(move_input.begin(), move_input.end(), move_input.begin(), ::toupper); // make input upper case
		// input handling
		if (move_input.size() != 2)// valid string size
		{
			cmpt::error("Too manyz or too few characters entered. Please try again \n");
		}
		if (move_input[0] >= 'A' && move_input[0] <= 'H')
		{
			// valid first coordinate
		} else {
			cmpt::error("The first character out of range. Please try again \n");
		}
		if (move_input[1] >= '1' && move_input[1] <= '8')
		{
			// valid second coordinate
		} else {
			cmpt::error("The second character out of range. Please try again \n");
		}

		int ascii_char_to_int_offset = 65;
		move_row = (int)move_input[0] - ascii_char_to_int_offset; // make Row input convert to interger "y-axis"
		move_col = (move_input[1] - '0') - 1; // make Col input covert to interger "x-axis"

		// cout << "x cord " << move_col << " y cord " << move_row << "\n";

		// check to see whether move is valid
		bool valid_move_made = false;
		for (int i = 0; i < valid_spaces.size(); ++i)
		{
			if (move_row == valid_spaces[i].x && move_col == valid_spaces[i].y)
			{
				board[move_row][move_col] = Cell::black;
				move_made = true;
				valid_move_made = true;

				valid_spaces.clear(); // clear valid spaces vector
				Loc the_move;
				the_move.x = move_row;
				the_move.y = move_col;
				valid_spaces.push_back(the_move); // vector now only holds move made

				break;
			}
		}

		if (valid_move_made == false)
		{
			cmpt::error("That is not a valid move, please pick from one of the yellow spaces \n");
		}

		return move_made;
	}

public:
	
	Human_player()
	{}

	// if 
	void make_move(Board& board, vector<Loc>& valid_spaces) {
		if (valid_spaces.size() > 0) // there is a valid move present
		{
			move_skipped = false;
			bool move_made_status = false;
			while(move_made_status == false)
			{
				try {
					move_made_status = get_move(board, valid_spaces);	
				} catch (std::runtime_error e) {
					cout << magenta << e.what() << reset_color << "\n";
				} catch (...) {
					// cout << "catch ... \n";
					cout << "An unkown error occured. Please try again. \n";
				}
			}
		} else {
			// nothing skip players turn
			move_skipped = true;
			cout << cyan + "no valid move availible, player turn skipped! \n" + reset_color;
		}
	}

	bool get_skip_status() const {
		return move_skipped;
	}
	
	~Human_player()
	{}

};

// class for computer player 
// piece colour WHITE
class Computer_player : public Player {
private:
	bool move_skipped = false;

	// calculates number of pieces that would be flipped
	int number_of_flips_made(const Board& board, int x, int y, int dx, int dy){

		bool done = false;
		bool black_found = false;

		// int original_x = x;
		// int original_y = y;

		int num_of_flips = 0;

		while(done == false){
			x = x + dx;
			y = y + dy;

			if (x < 0 || x > 7 || y < 0 || y > 7) // outside board
			{
				num_of_flips = 0;
				done = true;
				
			} else if (board[x][y] == Cell::empty) // empty square
			{
				num_of_flips = 0;
				done = true;
				
			} else if (board[x][y] == Cell::move) { //  move square (would be empty)
				num_of_flips = 0;
				done = true;

			} else if (board[x][y] == Cell::black) // is black piece
			{
				num_of_flips++;
				black_found = true;
				
			} else if (board[x][y] == Cell::white && black_found == false) // found white piece first
			{
				done = true;
				
			} else if (board[x][y] == Cell::white && black_found == true) // found white piece after finding black piece
			{
				done = true;
				
			}
		}

		return num_of_flips;
	}

	// returns the move that flips the most amount of pieces
	Loc computer_move_generator(const Board& board, const vector<Loc> valid_spaces){
		Loc final_computer_move;
		int num_pieces_flipped = 0;
		int num_pieces_flipped_hold = 0;

			// add up number of pieces flipped for that piece position
			for (int i = 0; i < valid_spaces.size(); ++i)
			{
				int dx = 1;
				int dy = 0;
				num_pieces_flipped += number_of_flips_made(board, valid_spaces[i].x, valid_spaces[i].y, dx, dy);
				dx = 1;
				dy = -1;
				num_pieces_flipped += number_of_flips_made(board, valid_spaces[i].x, valid_spaces[i].y, dx, dy);
				dx = 0;
				dy = -1;
				num_pieces_flipped += number_of_flips_made(board, valid_spaces[i].x, valid_spaces[i].y, dx, dy);
				dx = -1;
				dy = -1;
				num_pieces_flipped += number_of_flips_made(board, valid_spaces[i].x, valid_spaces[i].y, dx, dy);
				dx = -1;
				dy = 0;
				num_pieces_flipped += number_of_flips_made(board, valid_spaces[i].x, valid_spaces[i].y, dx, dy);
				dx = -1;
				dy = 1;
				num_pieces_flipped += number_of_flips_made(board, valid_spaces[i].x, valid_spaces[i].y, dx, dy);
				dx = 0;
				dy = 1;
				num_pieces_flipped += number_of_flips_made(board, valid_spaces[i].x, valid_spaces[i].y, dx, dy);
				dx = 1;
				dy = 1;
				num_pieces_flipped += number_of_flips_made(board, valid_spaces[i].x, valid_spaces[i].y, dx, dy);

				// cout << "checking space " << valid_spaces[i].x << " and " << valid_spaces[i].y << "\n";
				// cout << "num of flips made " << num_pieces_flipped << "\n";
				// cout << "versus numb of flips previously "  << num_pieces_flipped_hold << "\n"
				// 	 << "\n";

				if (num_pieces_flipped > num_pieces_flipped_hold)
				{
					num_pieces_flipped_hold = num_pieces_flipped;
					num_pieces_flipped = 0; // reset counter
					final_computer_move.x = valid_spaces[i].x;
					final_computer_move.y = valid_spaces[i].y;
				} else {
					// not bigger than previous
					// reset counter
					num_pieces_flipped = 0;
				}
			}

		return final_computer_move;
	}

public:

	Computer_player()
	{}

	// computer will always grab the corner space if it is a valid move availible to it
	// otherwise, it chooses the spot that flips the most amount of pieces
	void make_move(Board& board, vector<Loc>& valid_spaces) {
		if (valid_spaces.size() > 0) // there is a valid move present
		{
			// cout << "computer looking to make move \n";
			move_skipped = false;
			bool computer_move_made = false;
			Loc move_location;

			// if corner is a valid move, take corner space
			for (int i = 0; i < valid_spaces.size(); ++i)
			{
				if (valid_spaces[i].x == 0 && valid_spaces[i].y == 0)
				{
					// top left corner is a valid move
					board[valid_spaces[i].x][valid_spaces[i].y] = Cell::white;
					computer_move_made = true;
					move_location.x = valid_spaces[i].x;
					move_location.y = valid_spaces[i].y;

				} else if (valid_spaces[i].x == 7 && valid_spaces[i].y == 0)
				{
					// bottom left corner is a valid move
					board[valid_spaces[i].x][valid_spaces[i].y] = Cell::white;
					computer_move_made = true;
					move_location.x = valid_spaces[i].x;
					move_location.y = valid_spaces[i].y;
				} else if (valid_spaces[i].x == 0 && valid_spaces[i].y == 7)
				{
					// top right corner is a valid move
					board[valid_spaces[i].x][valid_spaces[i].y] = Cell::white;
					computer_move_made = true;
					move_location.x = valid_spaces[i].x;
					move_location.y = valid_spaces[i].y;
				} else if (valid_spaces[i].x == 7 && valid_spaces[i].y == 7)
				{
					// bottom right corner is a valid move
					board[valid_spaces[i].x][valid_spaces[i].y] = Cell::white;
					computer_move_made = true;
					move_location.x = valid_spaces[i].x;
					move_location.y = valid_spaces[i].y;
				}
			}

			if (computer_move_made == false) // computer did not find corner space
			{
				// OLD METHOD
				// cout << "no corner found \n";
				// board[valid_spaces[0].x][valid_spaces[0].y] = Cell::white;
				// move_location.x = valid_spaces[0].x;
				// move_location.y = valid_spaces[0].y;

				// choose position that flips the most pieces
				// if (all) the same, then picks the first of the largest
				move_location = computer_move_generator(board, valid_spaces);

			} else {
				// computer did find a corner space and made move
			}

			valid_spaces.clear();
			valid_spaces.push_back(move_location);

		} else {
			// nothing skip computer turn
			move_skipped = true;
			cout << cyan + "No valid move availible, computer skipped! \n" + reset_color;
		}
	}

	bool get_skip_status() const {
		return move_skipped;
	}

	~Computer_player()
	{}
};

//************************************************************************************
// looks for valid moves for BLACK pieces
void potential_space_checker_black(const Board& board, vector<Loc>& potential_spaces){
	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 8; ++j) // check across the row (col by col)
		{
			if (board[i][j] == Cell::white) //"[x][y]"
			{
				for (int y = -1; y < 2; ++y) // col before current space, col it's on and col after
				{
					if (j+y < 0 || j+y > 7)
					{
						// skip as col is outside of board
					} else {
						if (board[i][j+y] == Cell::empty) // horizontal along the middle spaces
						{
							Loc space_found;
							space_found.x = i; // x
							space_found.y = j+y; // y
							potential_spaces.push_back(space_found);
						}

						if (i-1 < 0)
						{
							// skip as spaces are outside of board
						} else {
							if (board[i-1][j+y] == Cell::empty) // horizontal along the bottom spaces
							{
								Loc space_found;
								space_found.x = i-1; // x
								space_found.y = j+y; // y
								potential_spaces.push_back(space_found);
							}
						}

						if (i+1 > 7)
						{
							// skip as spaces are ouside of board
						} else {
							if (board[i+1][j+y] == Cell::empty) // horizontal along the top spaces
							{
								Loc space_found;
								space_found.x = i+1; // x
								space_found.y = j+y; // y
								potential_spaces.push_back(space_found);
							}
						}
					}
				}
			}
		}
	}
}

bool valid_space_checker_black(const Board& board, vector<Loc>& valid_spaces, int x, int y, int dx, int dy){

	bool done = false;
	bool white_found = false;
	bool valid_space_found = false;

	int original_x = x;
	int original_y = y;

	// int DEBUG_it_counter = 0;

	while(done == false){
		x = x + dx;
		y = y + dy;

		if (x < 0 || x > 7 || y < 0 || y > 7) // outside board
		{
			done = true;
			valid_space_found = false;
		} else if (board[x][y] == Cell::empty) // empty square
		{
			done = true;
			valid_space_found = false;
		} else if (board[x][y] == Cell::white) // is white piece
		{
			white_found = true;
			valid_space_found = false;
		} else if (board[x][y] == Cell::black && white_found == false) // found black piece first
		{
			done = true;
			valid_space_found = false;
		} else if (board[x][y] == Cell::black && white_found == true) // found black piece after finding white piece
		{
			done = true;
			valid_space_found = true;
			Loc yes_location;
			yes_location.x = original_x;
			yes_location.y = original_y;
			valid_spaces.push_back(yes_location);
		}

	}

	return valid_space_found;
}

void valid_space_final_black_write(Board& board, const vector<Loc>& potential_spaces, vector<Loc>& valid_spaces){
	bool valid_space_found = false;
	for (int i = 0; i < potential_spaces.size(); i++)
	{
		int dx = 1;
		int dy = 0;
		valid_space_found = valid_space_checker_black(board, valid_spaces, potential_spaces[i].x, potential_spaces[i].y, dx, dy);
		// cout << "(1, 0) checker ran checking " << potential_spaces[i].x << " " << potential_spaces[i].y << " result is " << valid_space_found << "\n";
		if (valid_space_found == false)
		{
			dx = 1;
			dy = -1;
			valid_space_found = valid_space_checker_black(board, valid_spaces, potential_spaces[i].x, potential_spaces[i].y, dx, dy);
			// cout << "(1, -1) checker ran checking " << potential_spaces[i].x << " " << potential_spaces[i].y << " result is " << valid_space_found << "\n";
		}
		if (valid_space_found == false)
		{
			dx = 0;
			dy = -1;
			valid_space_found = valid_space_checker_black(board, valid_spaces, potential_spaces[i].x, potential_spaces[i].y, dx, dy);
			// cout << "(0, -1) checker ran checking " << potential_spaces[i].x << " " << potential_spaces[i].y << " result is " << valid_space_found << "\n";
		}
		if (valid_space_found == false)
		{
			dx = -1;
			dy = -1;
			valid_space_found = valid_space_checker_black(board, valid_spaces, potential_spaces[i].x, potential_spaces[i].y, dx, dy);
			// cout << "(-1, -1) checker ran checking " << potential_spaces[i].x << " " << potential_spaces[i].y << " result is " << valid_space_found << "\n";
		}
		if (valid_space_found == false)
		{
			dx = -1;
			dy = 0;
			valid_space_found = valid_space_checker_black(board, valid_spaces, potential_spaces[i].x, potential_spaces[i].y, dx, dy);
			// cout << "(-1, 0) checker ran checking " << potential_spaces[i].x << " " << potential_spaces[i].y << " result is " << valid_space_found << "\n";
		}
		if (valid_space_found == false)
		{
			dx = -1;
			dy = 1;
			valid_space_found = valid_space_checker_black(board, valid_spaces, potential_spaces[i].x, potential_spaces[i].y, dx, dy);
			// cout << "(-1, 1) checker ran checking " << potential_spaces[i].x << " " << potential_spaces[i].y << " result is " << valid_space_found << "\n";
		}
		if (valid_space_found == false)
		{
			dx = 0;
			dy = 1;
			valid_space_found = valid_space_checker_black(board, valid_spaces, potential_spaces[i].x, potential_spaces[i].y, dx, dy);
			// cout << "(0, 1) checker ran checking " << potential_spaces[i].x << " " << potential_spaces[i].y << " result is " << valid_space_found << "\n";
		}
		if (valid_space_found == false)
		{
			dx = 1;
			dy = 1;
			valid_space_found = valid_space_checker_black(board, valid_spaces, potential_spaces[i].x, potential_spaces[i].y, dx, dy);
			// cout << "(1, 1) checker ran checking " << potential_spaces[i].x << " " << potential_spaces[i].y << " result is " << valid_space_found << "\n";
		}
	}

	// draws valid space positions into board
	// cout << "writing to board \n";
	for (int i = 0; i < valid_spaces.size(); ++i)
	{
		board[valid_spaces[i].x][valid_spaces[i].y] = Cell::move;
	}
}
//************************************************************************************************************************************

//************************************************************************************************************************************
// looks for valid moves for WHITE pieces
void potential_space_checker_white(const Board& board, vector<Loc>& potential_spaces){
	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 8; ++j) // check across the row (col by col)
		{
			if (board[i][j] == Cell::black) //"[y][x]"
			{
				for (int y = -1; y < 2; ++y) // col before current space, col it's on and col after
				{
					if (j+y < 0 || j+y > 7)
					{
						// skip as col is outside of board
					} else {
						if (board[i][j+y] == Cell::empty) // horizontal along the middle spaces
						{
							Loc space_found;
							space_found.x = i; // x
							space_found.y = j+y; // y
							potential_spaces.push_back(space_found);
						}

						if (i-1 < 0)
						{
							// skip as spaces are outside of board
						} else {
							if (board[i-1][j+y] == Cell::empty) // horizontal along the bottom spaces
							{
								Loc space_found;
								space_found.x = i-1; // x
								space_found.y = j+y; // y
								potential_spaces.push_back(space_found);
							}
						}

						if (i+1 > 7)
						{
							// skip as spaces are ouside of board
						} else {
							if (board[i+1][j+y] == Cell::empty) // horizontal along the top spaces
							{
								Loc space_found;
								space_found.x = i+1; // x
								space_found.y = j+y; // y
								potential_spaces.push_back(space_found);
							}
						}
					}
				}
			}
		}
	}
}

bool valid_space_checker_white(const Board& board, vector<Loc>& valid_spaces, int x, int y, int dx, int dy){

	bool done = false;
	bool black_found = false;
	bool valid_space_found = false;

	int original_x = x;
	int original_y = y;

	while(done == false){
		x = x + dx;
		y = y + dy;

		if (x < 0 || x > 7 || y < 0 || y > 7) // outside board
		{
			done = true;
			valid_space_found = false;
		} else if (board[x][y] == Cell::empty) // empty square
		{
			done = true;
			valid_space_found = false;
		} else if (board[x][y] == Cell::black) // is black piece
		{
			black_found = true;
			valid_space_found = false;
		} else if (board[x][y] == Cell::white && black_found == false) // found white piece first
		{
			done = true;
			valid_space_found = false;
		} else if (board[x][y] == Cell::white && black_found == true) // found white piece after finding black piece
		{
			done = true;
			valid_space_found = true;
			Loc yes_location;
			yes_location.x = original_x;
			yes_location.y = original_y;
			valid_spaces.push_back(yes_location);
		}
	}

	return valid_space_found;
}

void valid_space_final_white_write(Board& board, const vector<Loc>& potential_spaces, vector<Loc>& valid_spaces){
	bool valid_space_found = false;
	for (int i = 0; i < potential_spaces.size(); i++)
	{
		int dx = 1;
		int dy = 0;
		valid_space_found = valid_space_checker_white(board, valid_spaces, potential_spaces[i].x, potential_spaces[i].y, dx, dy);
		if (valid_space_found == false)
		{
			dx = 1;
			dy = -1;
			valid_space_found = valid_space_checker_white(board, valid_spaces, potential_spaces[i].x, potential_spaces[i].y, dx, dy);
		}
		if (valid_space_found == false)
		{
			dx = 0;
			dy = -1;
			valid_space_found = valid_space_checker_white(board, valid_spaces, potential_spaces[i].x, potential_spaces[i].y, dx, dy);
		}
		if (valid_space_found == false)
		{
			dx = -1;
			dy = -1;
			valid_space_found = valid_space_checker_white(board, valid_spaces, potential_spaces[i].x, potential_spaces[i].y, dx, dy);
		}
		if (valid_space_found == false)
		{
			dx = -1;
			dy = 0;
			valid_space_found = valid_space_checker_white(board, valid_spaces, potential_spaces[i].x, potential_spaces[i].y, dx, dy);
		}
		if (valid_space_found == false)
		{
			dx = -1;
			dy = 1;
			valid_space_found = valid_space_checker_white(board, valid_spaces, potential_spaces[i].x, potential_spaces[i].y, dx, dy);
		}
		if (valid_space_found == false)
		{
			dx = 0;
			dy = 1;
			valid_space_found = valid_space_checker_white(board, valid_spaces, potential_spaces[i].x, potential_spaces[i].y, dx, dy);
		}
		if (valid_space_found == false)
		{
			dx = 1;
			dy = 1;
			valid_space_found = valid_space_checker_white(board, valid_spaces, potential_spaces[i].x, potential_spaces[i].y, dx, dy);
		}
	}

	// draws valid space positions into board [not necessary for white as computer plays]
	for (int i = 0; i < valid_spaces.size(); ++i)
	{
		board[valid_spaces[i].x][valid_spaces[i].y] = Cell::move;
	}
}
//***********************************************************************************************************************************

void intermediate_board_cleanup(Board& board, vector<Loc>& potential_spaces, vector<Loc>& valid_spaces){
	//*******************************************************
	// reset board by clearing all the cells with potential move and make empty again
	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			if (board[i][j] == Cell::move)
			{
				board[i][j] = Cell::empty;
			}
		}
	}
	// clear vectors
	potential_spaces.clear();
	valid_spaces.clear();
	//********************************************************
}

// when black piece is placed. this updates the board by flipping the appropriate affected pieces
void piece_flipper_black(Board& board, const Loc& move_made_coordinate, const int& dx, const int& dy){

	bool done = false;
	bool white_found = false;
	bool valid_direction_found = false;
	Loc final_location;

	int x = move_made_coordinate.x;
	int y = move_made_coordinate.y;

	while(done == false){
		x = x + dx;
		y = y + dy;

		if (x < 0 || x > 7 || y < 0 || y > 7) // outside board
		{
			done = true;
			valid_direction_found = false;
		} else if (board[x][y] == Cell::empty) // empty square
		{
			done = true;
			valid_direction_found = false;
		} else if (board[x][y] == Cell::white) // is white piece
		{
			white_found = true;
			valid_direction_found = false;
		} else if (board[x][y] == Cell::black && white_found == false) // found black piece first
		{
			done = true;
			valid_direction_found = false;
		} else if (board[x][y] == Cell::black && white_found == true) // found black piece after finding white piece
		{
			done = true;
			valid_direction_found = true;
			final_location.x = x; // store location of final black piece
			final_location.y = y; // flip up to this location
		}
	}

	if (valid_direction_found == true)
	{
		// cout << "valid direction found " << dx << " " << dy << "\n";
		x = move_made_coordinate.x;
		y = move_made_coordinate.y;
		// cout << "x: " << x << " fl.x: " << final_location.x << "\n";
		// cout << "y: " << y << " fl.y: " << final_location.y << "\n";
		// flip pieces between move made coordinate to final location using dx and dy to get there
		while(x != final_location.x || y != final_location.y) // replace all cells inbetween the beginning and end
		{
			// cout << "replacing cells \n";
			board[x][y] = Cell::black;
			x = x + dx;
			y = y + dy;
		}
	}
	
}

// checks in all 8 directions from placed piece calling the function that will filp pieces accordingly
void piece_flipper_black_caller8(Board& board, const Loc& move_made_coordinate){
	int dx = 1;
	int dy = 0;
	piece_flipper_black(board, move_made_coordinate, dx, dy);
		
	dx = 1;
	dy = -1;
	piece_flipper_black(board, move_made_coordinate, dx, dy);

	dx = 0;
	dy = -1;
	piece_flipper_black(board, move_made_coordinate, dx, dy);
	
	dx = -1;
	dy = -1;
	piece_flipper_black(board, move_made_coordinate, dx, dy);
	
	dx = -1;
	dy = 0;
	piece_flipper_black(board, move_made_coordinate, dx, dy);

	dx = -1;
	dy = 1;
	piece_flipper_black(board, move_made_coordinate, dx, dy);
		
	dx = 0;
	dy = 1;
	piece_flipper_black(board, move_made_coordinate, dx, dy);
	
	dx = 1;
	dy = 1;
	piece_flipper_black(board, move_made_coordinate, dx, dy);
}

// when white piece is placed. this updates the board by flipping the appropriate affected pieces
void piece_flipper_white(Board& board, const Loc& move_made_coordinate, const int& dx, const int& dy){

	bool done = false;
	bool black_found = false;
	bool valid_direction_found = false;
	Loc final_location;

	int x = move_made_coordinate.x;
	int y = move_made_coordinate.y;

	while(done == false){
		x = x + dx;
		y = y + dy;

		if (x < 0 || x > 7 || y < 0 || y > 7) // outside board
		{
			done = true;
			valid_direction_found = false;
		} else if (board[x][y] == Cell::empty) // empty square
		{
			done = true;
			valid_direction_found = false;
		} else if (board[x][y] == Cell::black) // is black piece
		{
			black_found = true;
			valid_direction_found = false;
		} else if (board[x][y] == Cell::white && black_found == false) // found white piece first
		{
			done = true;
			valid_direction_found = false;
		} else if (board[x][y] == Cell::white && black_found == true) // found white piece after finding black piece
		{
			done = true;
			valid_direction_found = true;
			final_location.x = x; // store location of final black piece
			final_location.y = y; // flip up to this location
		}
	}

	if (valid_direction_found == true)
	{
		// cout << "valid direction found " << dx << " " << dy << "\n";
		x = move_made_coordinate.x;
		y = move_made_coordinate.y;
		// cout << "x: " << x << " fl.x: " << final_location.x << "\n";
		// cout << "y: " << y << " fl.y: " << final_location.y << "\n";
		// flip pieces between move made coordinate to final location using dx and dy to get there
		while(x != final_location.x || y != final_location.y) // replace all cells inbetween the beginning and end
		{
			// cout << "replacing cells \n";
			board[x][y] = Cell::white;
			x = x + dx;
			y = y + dy;
		}
	}
	
}

// checks in all 8 directions from placed piece calling the function that will filp pieces accordingly
void piece_flipper_white_caller8(Board& board, const Loc& move_made_coordinate){
	int dx = 1;
	int dy = 0;
	piece_flipper_white(board, move_made_coordinate, dx, dy);
		
	dx = 1;
	dy = -1;
	piece_flipper_white(board, move_made_coordinate, dx, dy);

	dx = 0;
	dy = -1;
	piece_flipper_white(board, move_made_coordinate, dx, dy);
	
	dx = -1;
	dy = -1;
	piece_flipper_white(board, move_made_coordinate, dx, dy);
	
	dx = -1;
	dy = 0;
	piece_flipper_white(board, move_made_coordinate, dx, dy);

	dx = -1;
	dy = 1;
	piece_flipper_white(board, move_made_coordinate, dx, dy);
		
	dx = 0;
	dy = 1;
	piece_flipper_white(board, move_made_coordinate, dx, dy);
	
	dx = 1;
	dy = 1;
	piece_flipper_white(board, move_made_coordinate, dx, dy);
}

// ask yser whether they want to reset game or quit
// error handled
bool game_on_status(){
	string response;
	bool game_on = false;
		cout << "Do you want to start a new game? (Y/N) ";
		getline (cin, response);
		transform(response.begin(), response.end(), response.begin(), ::toupper); // make input upper case

		// input handling
		if (response.size() != 1)// valid string size
		{	
			cmpt::error("Too many characters entered. Please try again.");
		}

		if (response[0] == 'Y')
		{	
			game_on = true;
		} else if (response[0] == 'N')
		{
			game_on = false;
		} else {
			cmpt::error("Not a valid input. Please try again. \"Y\" for yes and \"N\" for no");
		}
	return game_on;
}


int main() {

	bool game_on = true;

	while(game_on == true)
	{

		cout << "\n" << "                 GAME START ";
		cout << "\n" << "                 \"Legend\" \n"
			 << white_str << " denotes white piece " << black_str << " denotes black piece \n"
			 << empty_str << " denotes empty spot  " << move_str << " denotes legal move position \n" << "\n"; 
		Board board = make_empty_board(); // board is 8x8

		display(board);

		vector<Loc> potential_spaces; // stores cordinates for potential move spaces
		vector<Loc> valid_spaces; // stores coordinates for valid spaces
		Human_player subject; // human player
		Computer_player computer; // computer player

		while(subject.get_skip_status() == false || computer.get_skip_status() == false)
		{
			// iterate through every space looking for Cells containing white piece
			potential_space_checker_black(board, potential_spaces);

			// find valid spaces from list of potentials and draw onto board
			valid_space_final_black_write(board, potential_spaces, valid_spaces);

			cout << cyan + "BOARD WITH VALID SPACES FOR PLAYER \n" + reset_color;
			display(board); // display board so player can see
	
			subject.make_move(board, valid_spaces); // if move is availble, prompt player to make move
			// vector "valid_spaces" now contains coordinate of move
	
			Loc move_made_coordinate = valid_spaces[0]; // store the move made location

			//*******************************************************
			intermediate_board_cleanup(board, potential_spaces, valid_spaces); // clean the board and vectors for next player (computer)
			//********************************************************
			// check from that point all 8 directions and see which pieces need to be flipped
			piece_flipper_black_caller8(board, move_made_coordinate);

			cout << cyan + "BOARD AFTER PLAYER'S MOVE \n" + reset_color;
			display(board); // show new board
			//*******************************************************
			intermediate_board_cleanup(board, potential_spaces, valid_spaces); // might not be necessary to call again
			//********************************************************

			// iterate through every space looking for Cells containing black piece
			potential_space_checker_white(board, potential_spaces);
			// find valid spaces from list of potentials and draw onto board
			valid_space_final_white_write(board, potential_spaces, valid_spaces);

			cout << cyan + "VALID COMPUTER MOVE SPACES \n" + reset_color;
			display(board);

			computer.make_move(board, valid_spaces); // if move is availble, computer will make move
			// vector "valid_spaces" now contains coordinate of move
	
			move_made_coordinate = valid_spaces[0]; // store the move made location

			//*******************************************************
			intermediate_board_cleanup(board, potential_spaces, valid_spaces); // clean the board and vectors for next player (computer)
			//********************************************************
			// check from that point all 8 directions and see which pieces need to be flipped
			piece_flipper_white_caller8(board, move_made_coordinate);

			//*******************************************************
			intermediate_board_cleanup(board, potential_spaces, valid_spaces); // clean the board and vectors for next player (computer)
			//********************************************************

			cout << cyan + "BOARD AFTER COMPUTER MAKES MOVE \n" + reset_color;
			display(board);
		} // end of match
		
		int num_black_pieces = count(board, Cell::black);
		int num_white_pieces = count(board, Cell::white);
		// determines who won, white or black, computer of player
		if (num_black_pieces > num_white_pieces)
		{
			cout << red + "Player Wins! \n" + reset_color;
			cout << "With " << num_black_pieces << " to the computer's " << num_white_pieces << "\n";
		} else if (num_black_pieces == num_white_pieces)
		{
			cout << "It's a draw \n";
			cout << "with player's " << num_black_pieces << " to the compouter's " << num_white_pieces << "\n";
		} else {
			cout << green + "Computer Wins! \n" + reset_color;
			cout << "with " << num_white_pieces << " to the player's " << num_black_pieces << "\n";
		}

		// ask whether game wants to be continued
		bool response_status = false;
		while(response_status == false)
		{
			try {
					game_on = game_on_status();	// game resets
					response_status = true; // proper response recorded
				} catch (std::runtime_error e) {
					cout << magenta << e.what() << reset_color << "\n";
					response_status = false;
				} catch (...) {
					cout << "Unknown Error Occured. Please try again (Y or N) \n";
					response_status = false;
				}
		}

		if (game_on == true)
		{
			cout << "OK! restarting game \n";
		}

	} // end of game

	cout << "\n" << "GOOD BYE \n" << "\n";
	return 0;
}