//---------------------------------------------------------------------------

#pragma hdrstop

#include "Info.h"
#pragma package(smart_init)

//---------------------------------------------------------------------------
/* empties the board */
void info_C::zero(){
	for (int pos = 0; pos < NUM_CELLS; pos++) {
		value[pos] = 0;
		fixed[pos] = false;
	}
}

/* a new board already holds the example problem, so that the application has
   something to show as soon as it opens */
info_C::info_C(){
	zero();
	set_example();
}

/* the example problem, one clue every three cells */
void info_C::set_example(){
	set_value( 0, 1);
	set_value( 3, 4);
	set_value( 6, 2);
	set_value( 9, 5);
	set_value(12, 3);
	set_value(15, 6);
	set_value(20, 1);
	set_value(23, 2);
	set_value(26, 2);
	set_value(29, 4);
	set_value(32, 5);
	set_value(35, 3);
}

/* true when both cells share a row, a column or a block, that is, when the
   rules of the game forbid them to hold the same value.
   This is the only place where the rule is stated: is_valid and conflict are
   both written on top of it, so they can never disagree about what the rules
   of the board are */
bool info_C::same_group(int a, int b){
	if (row(a) == row(b)) {
		return true;
	}

	if (col(a) == col(b)) {
		return true;
	}

	return block_row(a) == block_row(b)
		&& block_col(a) == block_col(b);
}

/* true when val can be written into pos: no other cell that shares a group
   with it holds that value already. This is the question the solver asks
   before it writes anything.
   The cell itself is skipped, so the answer does not depend on what pos
   happens to hold at the time */
bool info_C::is_valid(int pos, int val){
	for (int q = 0; q < NUM_CELLS; q++) {
		if (q == pos) {
			continue;
		}

		if (value[q] == val && same_group(pos, q)) {
			return false;
		}
	}

	return true;
}

/* true when the value the user wrote in this cell is repeated in another cell
   of the same row, column or block.
   Only the cells of the problem are compared. The ones filled in by solve()
   passed is_valid on the way in, so they cannot be wrong, and comparing them
   would paint the whole board red as soon as a clue is */
bool info_C::conflict(int pos){
	if (!fixed[pos] || value[pos] == 0) {
		return false;
	}

	for (int q = 0; q < NUM_CELLS; q++) {
		if (q == pos || !fixed[q]) {
			continue;
		}

		if (value[q] == value[pos] && same_group(pos, q)) {
			return true;
		}
	}

	return false;
}

/* how many cells of the problem are wrong. 0 means the problem follows the
   rules and can be handed to solve() */
int info_C::count_conflicts(){
	int n = 0;

	for (int pos = 0; pos < NUM_CELLS; pos++) {
		if (conflict(pos)) {
			n++;
		}
	}

	return n;
}

/* fills in the board from pos on, by backtracking: try a value, go on with
   the next cell, and undo the choice when the rest of the board turns out to
   have no solution.
   The cells are walked in order, so pos is at once the cell being filled and
   how far the search has got */
bool info_C::solve(int pos){
	// every cell has been filled
	if (pos == NUM_CELLS) {
		return true;
	}

	// a fixed cell keeps its value, go on with the next one
	if (fixed[pos]) {
		return solve(pos + 1);
	}

	// clear what a previous solve may have left here, so that a board with no
	// solution is left empty rather than holding a stale answer
	value[pos] = 0;

	// try every value the board allows, 1 to 6
	for (int val = 1; val <= BOARD_SIZE; val++) {
		if (is_valid(pos, val)) {
			value[pos] = val;

			if (solve(pos + 1))
				return true;

			// that value led nowhere, take it back and try the next one
			value[pos] = 0;
		}
	}

	// no value fits here, so the choice made before this call was wrong
	return false;
}
