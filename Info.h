//---------------------------------------------------------------------------

#ifndef InfoH
#define InfoH
//---------------------------------------------------------------------------

/* the shape of the board, and the only place where it is stated. The blocks
   are 2 rows by 3 columns, not square like the ones of the usual 9 x 9 game.
   BOARD_SIZE is also how many different values there are, 1 to 6 */
static const int BOARD_SIZE = 6;
static const int BLOCK_ROWS = 2;
static const int BLOCK_COLS = 3;
static const int NUM_CELLS  = BOARD_SIZE * BOARD_SIZE;

/* The board of a 6 x 6 sudoku and the solver, with no interface code: this
   class knows the rules of the game, not how it is drawn.

   A cell is an index from 0 to 35, counted by rows */
class info_C{
public:
	int    	value[NUM_CELLS]; /* 1 to 6, and 0 when the cell is still empty */
	/* true when the value was given by the problem, false when it was found
	   by solve(). It is what tells a clue from an answer */
	bool    fixed[NUM_CELLS];

public:
	info_C();
	/* an example problem, the one the BTest button loads */
	void set_example();
	/* empties the board */
	void zero();

	/* writes a value and marks it as part of the problem */
	void set_value(int pos, int val){
		value[pos] = val;
		fixed[pos] = true;
	}

	/* where a cell sits on the board. These only depend on the index of the
	   cell, so they are worked out on the spot rather than kept in tables
	   that could fall out of step with the board */
	static int row(int pos) { return pos / BOARD_SIZE; }
	static int col(int pos) { return pos % BOARD_SIZE; }
	/* first row and first column of the block the cell belongs to */
	static int block_row(int pos) { return (row(pos) / BLOCK_ROWS) * BLOCK_ROWS; }
	static int block_col(int pos) { return (col(pos) / BLOCK_COLS) * BLOCK_COLS; }

	/* the rule of the game, written once: two cells of the same row, column
	   or block may not hold the same value. Everything below is this rule
	   asked from one side or the other */
	static bool same_group(int a, int b);

	/* true when val can go into pos without breaking the rule */
	bool is_valid(int pos, int val);
	/* fills in every empty cell from pos on, false when there is no solution */
	bool solve(int pos);

	/* the same rule read back, while the problem is being written: which of
	   the cells the user typed are wrong */
	bool conflict(int pos);
	int  count_conflicts();
};

#endif
