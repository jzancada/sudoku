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
	// comprobar fila
	for (int c = 0; c < 6; c++) {
		int p = celda_fila[pos] * 6 + c;

		if (value[p] == numero) {
			return false;
		}
	}

	// comprobar columna
	for (int f=0; f < 6; f++) {
		int p = f * 6 + celda_columna[pos];

		if (value[p] == numero)  {
			return false;
		}
	}

	// comprobar SuperCelda
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


bool info_C::solve(int pos){
	// hemos terminado
	if (pos == 36) {
		return true;
	}

	// si esta fija, pasar a la siguiente
	if (fixed[pos]) {
		return solve(pos + 1);
	}

	// probar número del 1 al 6
	for (int numero=1; numero <= 6; numero++) {
//			OutputDebugString(L"Valor = ");
		if (EsValido (pos, numero)) {
			value[pos] = numero;

			if (solve(pos + 1))
				return true;

			value[pos] = 0;
		}
	}

	return false;
}


