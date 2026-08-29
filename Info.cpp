//---------------------------------------------------------------------------

#pragma hdrstop

#include "Info.h"
#pragma package(smart_init)

//---------------------------------------------------------------------------
void info_C::zero(){
	for (int n=0; n < NUM_CELDAS; n++) {
		value[n] = 0;
		fixed[n] = false;

		celda_fila[n]       = n / 6;
		celda_columna[n]    = n % 6;
		celda_superCeldaFilaIni[n] = (celda_fila[n] / 2) * 2;
		celda_superCeldaColIni [n] = (celda_columna[n] / 3) * 3;
	}
}

info_C::info_C(){
	zero();
	set_example();
}

void info_C::set_example(){
set_Value_and_Fixed(0, 1);
set_Value_and_Fixed(3, 4);
set_Value_and_Fixed(6, 2);
set_Value_and_Fixed(9, 5);
set_Value_and_Fixed(12, 3);
set_Value_and_Fixed(15, 6);
set_Value_and_Fixed(20, 1);
set_Value_and_Fixed(23, 2);
set_Value_and_Fixed(26, 2);
set_Value_and_Fixed(29, 4);
set_Value_and_Fixed(32, 5);
set_Value_and_Fixed(35, 3);

/*
Value[0] = 1;
Value[3] = 4;
Value[6] = 2;
Value[9] = 5;
Value[12] = 3;
Value[15] = 6;
Value[20] = 1;
Value[23] = 2;
Value[26] = 2;
Value[29] = 4;
Value[32] = 5;
Value[35] = 3;*/
}

bool info_C::EsValido(int pos, int numero){
	// check the row
	for (int c = 0; c < 6; c++) {
		int p = celda_fila[pos] * 6 + c;

		if (value[p] == numero) {
			return false;
		}
	}

	// check the column
	for (int f=0; f < 6; f++) {
		int p = f * 6 + celda_columna[pos];

		if (value[p] == numero)  {
			return false;
		}
	}

	// check the block, 2 rows x 3 columns
	for (int f = celda_superCeldaFilaIni[pos];
			f < celda_superCeldaFilaIni[pos]+2;
			f++) {
		for (int c = celda_superCeldaColIni[pos];
			c < celda_superCeldaColIni[pos]+3;
			c++) {
			int p = f * 6 + c;

			if (value[p] == numero) {
				return false;
			}
		}
	}

	return true;
}


// true when both cells share a row, a column or a block, that is, when the
// sudoku rules forbid them to hold the same value
bool info_C::same_group(int a, int b){
	if (celda_fila[a] == celda_fila[b]) {
		return true;
	}

	if (celda_columna[a] == celda_columna[b]) {
		return true;
	}

	return celda_superCeldaFilaIni[a] == celda_superCeldaFilaIni[b]
		&& celda_superCeldaColIni [a] == celda_superCeldaColIni [b];
}

// true when the value the user wrote in this cell is repeated in another cell
// of the same row, column or block.
// Only the cells of the problem are compared, the ones filled in by solve()
// are correct by construction and are ignored here
bool info_C::conflict(int pos){
	if (!fixed[pos] || value[pos] == 0) {
		return false;
	}

	for (int q = 0; q < NUM_CELDAS; q++) {
		if (q == pos || !fixed[q]) {
			continue;
		}

		if (value[q] == value[pos] && same_group(pos, q)) {
			return true;
		}
	}

	return false;
}

// how many cells of the problem are wrong, 0 means the problem is well stated
int info_C::count_conflicts(){
	int n = 0;

	for (int pos = 0; pos < NUM_CELDAS; pos++) {
		if (conflict(pos)) {
			n++;
		}
	}

	return n;
}


bool info_C::solve(int pos){
	// every cell has been filled
	if (pos == 36) {
		return true;
	}

	// a fixed cell keeps its value, go on with the next one
	if (fixed[pos]) {
		return solve(pos + 1);
	}

	// clear what a previous solve may have left here
	value[pos] = 0;

	// try the numbers 1 to 6
	for (int numero=1; numero <= 6; numero++) {
		if (EsValido (pos, numero)) {
			value[pos] = numero;

			if (solve(pos + 1))
				return true;

			value[pos] = 0;
		}
	}

	return false;
}


