//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include <Vcl.Graphics.hpp>
#include <FMX.Dialogs.hpp>

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TForm2 *Form2;
#include "info.h"

/* cell background colors, translucent so the digit stays readable */
static const TAlphaColor COLOR_FIXED_BG    = TAlphaColor(0x40ff9800);  /* amber: cell fixed by the problem */
static const TAlphaColor COLOR_CURSOR_BG   = TAlphaColor(0x330078d7);  /* blue : selected cell             */
static const TAlphaColor COLOR_CURSOR_LINE = TAlphaColor(0xff1565c0);  /* border of the selected cell      */
static const TAlphaColor COLOR_ERROR_BG    = TAlphaColor(0x66ff1744);  /* red  : value repeated, wrong cell */

/* lines of the board, like the ones of a spreadsheet */
static const TAlphaColor COLOR_GRID_LINE = TAlphaColor(0xff37474f);
static const float       GRID_THIN       = 1.0f;   /* between two cells            */
static const float       GRID_THICK      = 4.0f;   /* between two blocks, and frame */

/* geometry of the board, in pixels. The gaps hold the lines drawn by draw_grid */
static const float CELL_W    = 97.0f;
static const float CELL_H    = 41.0f;
static const float GAP_CELL  = 3.0f;   /* between two cells of the same block */
static const float GAP_BLOCK = 9.0f;   /* between two blocks                  */
static const float BOARD_X   = 8.0f;   /* top left corner of the board        */
static const float BOARD_Y   = 8.0f;

//---------------------------------------------------------------------------
__fastcall TForm2::TForm2(TComponent* Owner)
	: TForm(Owner)
{
	btn_v[0]=Button_0;
	btn_v[1]=Button_1;
	btn_v[2]=Button_2;
	btn_v[3]=Button_3;
	btn_v[4]=Button_4;
	btn_v[5]=Button_5;
	btn_v[6]=Button_6;
	btn_v[7]=Button_7;
	btn_v[8]=Button_8;
	btn_v[9]=Button_9;
	btn_v[10]=Button_10;
	btn_v[11]=Button_11;
	btn_v[12]=Button_12;
	btn_v[13]=Button_13;
	btn_v[14]=Button_14;
	btn_v[15]=Button_15;
	btn_v[16]=Button_16;
	btn_v[17]=Button_17;
	btn_v[18]=Button_18;
	btn_v[19]=Button_19;
	btn_v[20]=Button_20;
	btn_v[21]=Button_21;
	btn_v[22]=Button_22;
	btn_v[23]=Button_23;
	btn_v[24]=Button_24;
	btn_v[25]=Button_25;
	btn_v[26]=Button_26;
	btn_v[27]=Button_27;
	btn_v[28]=Button_28;
	btn_v[29]=Button_29;
	btn_v[30]=Button_30;
	btn_v[31]=Button_31;
	btn_v[32]=Button_32;
	btn_v[33]=Button_33;
	btn_v[34]=Button_34;
	btn_v[35]=Button_35;

	/* the board is laid out here and not in the designer, so that the gap
	   between blocks stays exact whatever the cell size is */
	layout_board();

	/* the lines go on top of the cells, but under the colors of refresh_panel,
	   which are created next, so that the selected cell is never covered */
	draw_grid();

	for (int i = 0; i < NUM_CELDAS; i++) {
		btn_v[i]->StyledSettings  =
			btn_v[i]->StyledSettings    >>  TStyledSetting::Size;

		btn_v[i]->TextSettings->Font->Size = btn_v[0]->TextSettings->Font->Size;

		/* the cell index is kept in the Tag */
		btn_v[i]->Tag = i;
		/* every cell shares the same click handler */
		btn_v[i]->OnClick = Button_Click;

		btn_v[i]->TextSettings->Font->Size = 30;

		/* the cells must not take the keyboard focus, the form handles the keys */
		btn_v[i]->CanFocus = false;

		/* a styled TButton has no background color of its own, so a rectangle is
		   laid on top of it to paint the state of the cell */
		TRectangle *mark = new TRectangle(this);

		mark->Parent  = btn_v[i]->Parent;
		mark->SetBounds(btn_v[i]->Position->X, btn_v[i]->Position->Y,
						btn_v[i]->Width,       btn_v[i]->Height);
		mark->HitTest = false;          /* clicks go through to the button below */
		mark->XRadius = 4;
		mark->YRadius = 4;
		mark->Fill->Kind   = TBrushKind::None;
		mark->Stroke->Kind = TBrushKind::None;

		mark_v[i] = mark;
	}

	/* no control may keep the keyboard focus: a focused TButton swallows the
	   space bar and clicks itself, and the space bar empties the selected cell */
	Button1->CanFocus = false;   /* BSolve */
	BZero->CanFocus   = false;
	BTest->CanFocus   = false;
	BHelp->CanFocus   = false;

	/* the warning line under the buttons, red and out of the style */
	LStatus->StyledSettings = LStatus->StyledSettings
							  >> TStyledSetting::FontColor
							  >> TStyledSetting::Size;
	LStatus->TextSettings->Font->Size  = 13;
	LStatus->TextSettings->FontColor   = claRed;
	LStatus->TextSettings->WordWrap    = true;

	refresh_panel();

	bool ret = info.solve(0);
	if (ret) {
		refresh_panel();
	}
}
/* places the 36 cells on a regular grid. The board is 6 x 6 and its blocks are
   2 rows x 3 columns, so the wider gap goes after every 3rd column and after
   every 2nd row. Those gaps are where draw_grid paints the thick lines */
void TForm2::layout_board(){
	float x[6];
	float y[6];

	x[0] = BOARD_X;
	y[0] = BOARD_Y;

	for (int c = 1; c < 6; c++) {
		x[c] = x[c - 1] + CELL_W + ((c % 3 == 0) ? GAP_BLOCK : GAP_CELL);
	}

	for (int r = 1; r < 6; r++) {
		y[r] = y[r - 1] + CELL_H + ((r % 2 == 0) ? GAP_BLOCK : GAP_CELL);
	}

	for (int i = 0; i < NUM_CELDAS; i++) {
		btn_v[i]->SetBounds(x[i % 6], y[i / 6], CELL_W, CELL_H);
	}

	/* the panel behind the board is fitted to it, with the same margin all around */
	Panel1->SetBounds(0, 0, x[5] + CELL_W + BOARD_X, y[5] + CELL_H + BOARD_Y);
}

/* one line of the board, laid on top of the form and transparent to the mouse */
TRectangle* TForm2::new_line(float x, float y, float w, float h){
	TRectangle *line = new TRectangle(this);

	line->Parent  = btn_v[0]->Parent;
	line->SetBounds(x, y, w, h);
	line->HitTest = false;
	line->Fill->Kind   = TBrushKind::Solid;
	line->Fill->Color  = COLOR_GRID_LINE;
	line->Stroke->Kind = TBrushKind::None;

	return line;
}

/* draws the 7 + 7 lines of the board. The cells are already laid out with a
   small gap between two cells and a wider one between two blocks, so every
   line is simply painted in the middle of the gap it belongs to.
   The geometry is read from the cells themselves, so moving them in the
   designer moves the lines as well */
void TForm2::draw_grid(){
	float left   = btn_v[0]->Position->X;
	float top    = btn_v[0]->Position->Y;
	float right  = btn_v[5]->Position->X  + btn_v[5]->Width;
	float bottom = btn_v[30]->Position->Y + btn_v[30]->Height;

	/* the lines stick out half a thick line at both ends, so that the corners
	   of the frame are closed */
	float over = GRID_THICK / 2;

	/* 7 vertical lines, thick every 3 columns: the two sides of the frame and
	   the single line that splits the board into left and right blocks */
	for (int c = 0; c <= 6; c++) {
		float thick = (c % 3 == 0) ? GRID_THICK : GRID_THIN;
		float x;

		if (c == 0) {
			x = left;
		} else if (c == 6) {
			x = right;
		} else {
			/* middle of the gap between the previous column and this one */
			x = (btn_v[c - 1]->Position->X + btn_v[c - 1]->Width
				 + btn_v[c]->Position->X) / 2;
		}

		new_line(x - thick / 2, top - over, thick, (bottom - top) + 2 * over);
	}

	/* 7 horizontal lines, thick every 2 rows: the top and bottom of the frame
	   and the two lines that split the board into three bands of blocks */
	for (int r = 0; r <= 6; r++) {
		float thick = (r % 2 == 0) ? GRID_THICK : GRID_THIN;
		float y;

		if (r == 0) {
			y = top;
		} else if (r == 6) {
			y = bottom;
		} else {
			y = (btn_v[(r - 1) * 6]->Position->Y + btn_v[(r - 1) * 6]->Height
				 + btn_v[r * 6]->Position->Y) / 2;
		}

		new_line(left - over, y - thick / 2, (right - left) + 2 * over, thick);
	}
}

void TForm2::refresh_panel(){
	TButton *btn;

	for (int i = 0; i < NUM_CELDAS; i++) {
		btn = btn_v[i];

		btn->Text = info.value[i];
		if (info.value[i] == 0) {
			btn->Text = " ";
		}

		/* the values given by the problem are written in red */
		if (info.fixed[i]) {
			btn->TextSettings->FontColor  = claRed;
		} else
			btn->TextSettings->FontColor  = claBlack;

		/* background: red for a wrong cell, amber for a fixed one, blue for the
		   selected one. A wrong cell wins, it is what has to be fixed first */
		TRectangle *mark = mark_v[i];

		if (info.conflict(i)) {
			mark->Fill->Kind  = TBrushKind::Solid;
			mark->Fill->Color = COLOR_ERROR_BG;
		} else if (info.fixed[i]) {
			mark->Fill->Kind  = TBrushKind::Solid;
			mark->Fill->Color = COLOR_FIXED_BG;
		} else if (i == cursorPos) {
			mark->Fill->Kind  = TBrushKind::Solid;
			mark->Fill->Color = COLOR_CURSOR_BG;
		} else {
			mark->Fill->Kind  = TBrushKind::None;
		}

		/* the selected cell also gets a border, so that it can still be told
		   apart when it is a fixed cell and already painted amber */
		if (i == cursorPos) {
			mark->Stroke->Kind      = TBrushKind::Solid;
			mark->Stroke->Color     = COLOR_CURSOR_LINE;
			mark->Stroke->Thickness = 3;
		} else {
			mark->Stroke->Kind = TBrushKind::None;
		}
	}

	/* warn about a problem that breaks the rules before it is solved */
	int wrong = info.count_conflicts();

	if (wrong > 0) {
		LStatus->Text = "Wrong problem: a value is repeated in a row, a column or a block. "
						"Check the cells in red.";
	} else {
		LStatus->Text = "";
	}
}

/* moves the selected cell, dRow / dCol are -1, 0 or +1 */
void TForm2::move_cursor(int dRow, int dCol){
	int row = cursorPos / 6 + dRow;
	int col = cursorPos % 6 + dCol;

	/* the cursor stops at the edges of the board */
	if (row < 0 || row > 5 || col < 0 || col > 5) {
		return;
	}

	cursorPos = row * 6 + col;

	refresh_panel();
}

//---------------------------------------------------------------------------
void __fastcall TForm2::Button_Click(TObject *Sender)
{
	TButton *btn = dynamic_cast<TButton*>(Sender);

	if (btn == NULL) {
		return;
	}

	/* clicking a cell only moves the selection there, the value comes from the keyboard */
	cursorPos = (int) btn->Tag;

	refresh_panel();
}
//---------------------------------------------------------------------------

void __fastcall TForm2::FormKeyDown(TObject *Sender, WORD &Key, System::WideChar &KeyChar,
		  TShiftState Shift)
{
	/* the arrow keys move the selected cell.
	   Key is cleared so that FMX does not use the arrow to move the focus */
	switch (Key) {
	case vkLeft:
		move_cursor(0, -1);
		Key = 0;
		return;
	case vkRight:
		move_cursor(0, +1);
		Key = 0;
		return;
	case vkUp:
		move_cursor(-1, 0);
		Key = 0;
		return;
	case vkDown:
		move_cursor(+1, 0);
		Key = 0;
		return;
	}

	/* 1 to 6 writes a value into the selected cell and fixes it as part of the
	   problem. 0 and the space bar empty the cell again */
	if ((KeyChar >= '0' && KeyChar <= '6') || KeyChar == ' ') {
		int numero = (KeyChar == ' ') ? 0 : KeyChar - '0';

		info.value[cursorPos] = numero;
		info.fixed[cursorPos] = (numero > 0);

		refresh_panel();

		KeyChar = 0;
		return;
	}

	/* s solves the board, same as the Solve button */
	if (KeyChar == 's' || KeyChar == 'S') {
		do_solve();

		KeyChar = 0;
		return;
	}

	/* h shows the instructions, same as the Help button */
	if (KeyChar == 'h' || KeyChar == 'H') {
		show_help();

		KeyChar = 0;
	}
}
//---------------------------------------------------------------------------


void __fastcall TForm2::BSolveClick(TObject *Sender)
{
	do_solve();
}
//---------------------------------------------------------------------------


void __fastcall TForm2::BZeroClick(TObject *Sender)
{
	info.zero();

	/* an empty board starts again with the first cell selected */
	cursorPos = 0;

	refresh_panel();
}
//---------------------------------------------------------------------------



/* solves the board, used by the Solve button and by the s key.
   The problem is checked first, so that solve() is only called on a problem
   that follows the rules */
void TForm2::do_solve(){
	if (info.count_conflicts() > 0) {
		ShowMessage(L"The problem is wrong: a value is repeated in a row, a column "
					L"or a block.\n\nFix the cells drawn in red and try again.");
		return;
	}

	bool ret = info.solve(0);

	refresh_panel();

	if (!ret) {
		ShowMessage(L"This problem follows the rules, but it has no solution.");
	}
}
//---------------------------------------------------------------------------


/* instructions, shown by the Help button and by the h key */
void TForm2::show_help(){
	ShowMessage(
		L"SUDOKU 6 x 6\n"
		/* __DATE__ is replaced by the compiler with the day the program was
		   built, so the version shown here can never be out of date */
		L"Version built on " __DATE__ L"\n"
		L"\n"
		L"One cell is always selected. It is drawn with a blue border.\n"
		L"\n"
		L"KEYBOARD\n"
		L"  Arrow keys  -  move the selected cell\n"
		L"  1 to 6      -  give that value to the selected cell. The value becomes\n"
		L"                 part of the problem, and is drawn in red on an amber cell\n"
		L"  0 or space  -  empty the selected cell again\n"
		L"  s           -  solve the board\n"
		L"  h           -  show this help\n"
		L"\n"
		L"MOUSE\n"
		L"  Click on a cell  -  select it, then type its value\n"
		L"\n"
		L"BUTTONS\n"
		L"  BSolve  -  fill in the empty cells, keeping the values of the problem\n"
		L"  BZero   -  empty the whole board\n"
		L"  BTest   -  load an example problem\n"
		L"  Help    -  show this help\n"
		L"\n"
		L"COLORS\n"
		L"  Amber cell, red digit  -  value given by the problem\n"
		L"  Blue border            -  cell currently selected\n"
		L"  Black digit            -  value found by the solver\n"
		L"  Red cell               -  wrong value: the same number is already in\n"
		L"                            that row, column or block\n"
		L"\n"
		L"The problem is checked while it is being written. As long as a cell is\n"
		L"red the board cannot be solved, so the problem is always solved from a\n"
		L"position that follows the rules.");
}
//---------------------------------------------------------------------------


void __fastcall TForm2::BHelpClick(TObject *Sender)
{
	show_help();
}
//---------------------------------------------------------------------------


void __fastcall TForm2::BTestClick(TObject *Sender)
{
	info.zero();
	info.set_example();
//	info.solve(0);
	refresh_panel();
}
//---------------------------------------------------------------------------

