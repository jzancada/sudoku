//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.Controls.Presentation.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.Types.hpp>

#include <info.h>
#include <FMX.Colors.hpp>
#include <FMX.Objects.hpp>
//---------------------------------------------------------------------------
class TForm2 : public TForm
{
__published:	// IDE-managed Components
	TButton *Button_0;
	TButton *Button_1;
	TButton *Button_2;
	TButton *Button_3;
	TButton *Button_4;
	TButton *Button_5;
	TButton *Button_6;
	TButton *Button_7;
	TButton *Button_8;
	TButton *Button_9;
	TButton *Button_10;
	TButton *Button_11;
	TButton *Button_12;
	TButton *Button_13;
	TButton *Button_14;
	TButton *Button_15;
	TButton *Button_16;
	TButton *Button_17;
	TButton *Button_18;
	TButton *Button_19;
	TButton *Button_20;
	TButton *Button_21;
	TButton *Button_22;
	TButton *Button_23;
	TButton *Button_24;
	TButton *Button_25;
	TButton *Button_26;
	TButton *Button_27;
	TButton *Button_28;
	TButton *Button_29;
	TButton *Button_30;
	TButton *Button_31;
	TButton *Button_32;
	TButton *Button_33;
	TButton *Button_34;
	TButton *Button_35;
	TPanel *Panel1;
	TButton *Button1;
	TButton *BZero;
	TButton *BTest;
	TButton *BHelp;
	TLabel *LStatus;
	void __fastcall Button_Click(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, System::WideChar &KeyChar,
          TShiftState Shift);
	void __fastcall BSolveClick(TObject *Sender);
	void __fastcall BZeroClick(TObject *Sender);
	void __fastcall BTestClick(TObject *Sender);
	void __fastcall BHelpClick(TObject *Sender);
private:	// User declarations
	TButton*    btn_v [NUM_CELDAS];
	TRectangle* mark_v[NUM_CELDAS];   /* background of every cell, drawn on top of the button */
	int         cursorPos = 0;        /* cell currently selected, 0..NUM_CELDAS-1 */
	info_C info;

	void layout_board();
	TRectangle* new_line(float x, float y, float w, float h);
	void draw_grid();

	void refresh_panel();
	void move_cursor(int dRow, int dCol);
	void show_help();
	void do_solve();
public:		// User declarations
	__fastcall TForm2(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm2 *Form2;
//---------------------------------------------------------------------------
#endif
