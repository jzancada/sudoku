//---------------------------------------------------------------------------

#ifndef InfoH
#define InfoH
//---------------------------------------------------------------------------
#define NUM_CELDAS 36
#include <Windows.h>

class info_C{
public:
	int    	value[NUM_CELDAS]; /* 1..6 , 0 - no asignada */
	bool    fixed[NUM_CELDAS];
	int     celda_fila[NUM_CELDAS];
	int     celda_columna[NUM_CELDAS];
	int     celda_superCeldaFilaIni[NUM_CELDAS];
	int     celda_superCeldaColIni[NUM_CELDAS];

public:
	info_C();
	void set_example();

	void set_Value_and_Fixed(int id, int val){
		value[id] = val;
		fixed[id] = true;
	}

	bool EsValido(int pos, int numero);
	bool solve(int pos);
	void zero();
};

#endif
