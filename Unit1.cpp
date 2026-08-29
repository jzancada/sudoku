//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include <Vcl.Graphics.hpp>

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TForm2 *Form2;
#include "info.h"
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

	for (int i = 0; i < 36; i++) {
		btn_v[i]->StyledSettings  =
			btn_v[i]->StyledSettings    >>  TStyledSetting::Size;

		btn_v[i]->TextSettings->Font->Size = btn_v[0]->TextSettings->Font->Size;

		/* Tag */
		btn_v[i]->Tag = i;
		/* asigno button click */
		btn_v[i]->OnClick = Button_Click;

		btn_v[i]->TextSettings->Font->Size = 30;
	}
	/* refresh_panel */
	refresh_panel();

	/* solve */
	bool ret = info.solve(0);
	if (ret) {
		/* refresh_panel */
		refresh_panel();
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

		if (info.fixed[i]) {
			btn->TextSettings->FontColor  = claRed;
		} else
			btn->TextSettings->FontColor  = claBlack;


		/* color */
//		if (info.fixed)
			//btn->TextSetting>FontColor = clWhite; /* gris claro */
//		else
//			btn->Font->Color = clWhite;
	}
}

//---------------------------------------------------------------------------
void __fastcall TForm2::Button_Click(TObject *Sender)
{
TButton *btn = dynamic_cast<TButton*>(Sender);

esperandoNumero        = true;
tagBotonEsperandoNumero = btn->Tag;
}
//---------------------------------------------------------------------------

void __fastcall TForm2::FormKeyDown(TObject *Sender, WORD &Key, System::WideChar &KeyChar,
		  TShiftState Shift)
{
	if (esperandoNumero) {
		if (KeyChar >= '0' && KeyChar <= '6') {
			int numero = KeyChar - '0';
//			ShowMessage("Numero Pulsado "+ IntToStr(numero));
			/* se asgina al boton esperando numero*/
			info.value[tagBotonEsperandoNumero] = numero;
			if (numero > 0)
				info.fixed[tagBotonEsperandoNumero]=true;
			else
				info.fixed[tagBotonEsperandoNumero]=false;

			refresh_panel();
		}
	}
	/* se termina esperando numero */
	esperandoNumero = false;

	refresh_panel();
}
//---------------------------------------------------------------------------


void __fastcall TForm2::BSolveClick(TObject *Sender)
{
	bool ret = info.solve(0);
	if (ret) {
		/* refresh_panel */
		refresh_panel();
	}
}
//---------------------------------------------------------------------------


void __fastcall TForm2::BZeroClick(TObject *Sender)
{
	info.zero();
	refresh_panel();
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

