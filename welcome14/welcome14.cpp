#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <wchar.h>
#include <uefi.h>
#include "UefiBase.hpp"
#include "TextWindow.hpp"
#include "DPRINTF.H"
#include "base_t.h"
#include <intrin.h>

//Available functions: https://github.com/KilianKegel/toro-C-Library#implementation-status

#define ELC(x) (sizeof(x)/sizeof(x[0]))

extern "C" EFI_SYSTEM_TABLE * gSystemTable;
extern "C" EFI_HANDLE gImageHandle;

extern bool gfKbdDbg;

#define TYPE char
#define TYPESIZE sizeof(TYPE)
#define TYPEMASK ((1ULL << TYPESIZE * 8)-1)

#define FORMATW_ADDR L"%02X: %02X%s"
#define FORMATWOADDR L"%s%02X%s"

#define RTCRD(idx) (__outbyte(0xED,0x55),__outbyte(0xED,0x55),__outbyte(0x70,idx),__outbyte(0xED,0x55),__outbyte(0xED,0x55),__inbyte(0x71))

#define IODELAY __outbyte(0xED, 0x55)

int rtcrd(int idx) 
{
	int nRet = 0;
	int UIP = 0;

	do {

		//
		// read UIP- update in progress first
		//
		__outbyte(0x70, 0xA); 
		IODELAY;

		UIP = 0x80 == (0x80 & __inbyte(0x71));
		IODELAY;

		__outbyte(0x70, idx);
		IODELAY;

		nRet = __inbyte(0x71); IODELAY;

	} while (1 == UIP);

	return nRet;
}
//
// globally shared data
//
bool gExit = false;
bool gHexView = true;

//
// menu item functions
//
void fnMnuItm_File_Exit(CTextWindow *pThis, void *pContext)
{
	pThis->TextPrint({20,20},"fnMnuItm_File_Exit...");
	gExit = true;
}

void fnMnuItm_View_HexSym(CTextWindow* pThis, void* pContext) 
{ 
	CTextWindow* pRoot = pThis->TextWindowGetRoot();

	pRoot->TextBlockClear();
	gHexView ^= true;// toggle debug state
	pThis->TextClearWindow();
}

//
// About - BOX
//
void fnMnuItm_About_0(CTextWindow* pThis, void* pContext)
{
	CTextWindow* pAboutBox;
	CTextWindow* pRoot = pThis->TextWindowGetRoot();

	pThis->TextClearWindow();	// clear the pull down menu 
	pRoot->TextBlockRfrsh();	// refresh the main window

	if (1)// center coordinate calculation
	{

	}
	pAboutBox = new CTextWindow(pThis, { pRoot->WinDim.X/2 - 60/2,pRoot->WinDim.Y/2 - 20/2 }, { 60,20 }, EFI_BACKGROUND_CYAN | EFI_YELLOW);
	
	pAboutBox->TextBorder(
		{ 0,0 },
		{ 60,20 },
		BOXDRAW_DOWN_RIGHT,
		BOXDRAW_DOWN_LEFT,
		BOXDRAW_UP_RIGHT,
		BOXDRAW_UP_LEFT,
		BOXDRAW_HORIZONTAL,
		BOXDRAW_VERTICAL,
		nullptr
	);

	pAboutBox->TextPrint({ pAboutBox->WinDim.X / 2 - (int32_t)strlen("RTCSpy+")/2,2 }, "RTCSpy+");
	pAboutBox->TextPrint({ pAboutBox->WinDim.X / 2 - (int32_t)strlen("RealTimeClock Analyser")/2,5 }, "RealTimeClock Analyser");

	//RealTimeClock Analyser

	for (TEXT_KEY key = NO_KEY; KEY_ESC != key && KEY_ENTER != key; key = pThis->TextGetKey(), pThis->TextWindowUpdateProgress())
		;

	pAboutBox->BgAtt = EFI_BACKGROUND_LIGHTGRAY | EFI_BLACK;
	delete pAboutBox;
}

void fnMnuItm_About_1(CTextWindow* pThis, void* pContext)
{
	gfKbdDbg ^= true;// toggle debug state

	if (false == gfKbdDbg)
	{
		char* pLineKiller = new char[pThis->ScrDim.X - 4];

		memset(pLineKiller, '\x20', pThis->ScrDim.X - 4);
		pLineKiller[pThis->ScrDim.X - 4] = '\0';

		pThis->GotoXY({ 2,pThis->ScrDim.Y - 3 });
		printf("%s", pLineKiller);
	}

	pThis->TextClearWindow();
}

void resetconsole(void)
{
	gSystemTable->ConOut->EnableCursor(gSystemTable->ConOut, 1);
	gSystemTable->ConOut->SetAttribute(gSystemTable->ConOut, EFI_BACKGROUND_BLACK + EFI_WHITE);
}

int main(int argc, char** argv)
{
	int nRet = 1;
	gSystemTable = (EFI_SYSTEM_TABLE*)(argv[-1]);		//SystemTable is passed in argv[-1]
	gImageHandle = (void*)(argv[-2]);					//ImageHandle is passed in argv[-2]
	TEXT_KEY key = NO_KEY;

	DPRINTF(("hello\n"));
	atexit(resetconsole);

	do
	{
		char* pc = new char[256];
		int* pi = new int(12345678);

		struct {
			RELPOS RelPos;
			const wchar_t* wcsMenuName;
			CTextWindow* pTextWindow;
			ABSDIM MnuDim;
			const wchar_t* rgwcsMenuItem[8];	// max. number of menu items is currently 8
			void (* rgfnMnuItm[8])(CTextWindow *, void *pContext);	// max. number of menu items is currently 8
		}menu[] =
		{
			{{04,0},	L" File ", nullptr,{20,3/* # menuitems + 2 */},	{L"Exit            "},{&fnMnuItm_File_Exit}},
			{{12,0},	L" View ", nullptr,{20,3/* # menuitems + 2 */},	{L"Hex/Sym view    "},{&fnMnuItm_View_HexSym}},
			{{20,0},	L" Help ", nullptr,{20,4/* # menuitems + 2 */},	{L"About           ",L"KEYBOARD DEBUG  "},{&fnMnuItm_About_0, &fnMnuItm_About_1 }}
		};

		CTextWindow FullScreen(nullptr/* no parent pointer - this is the main window !!! */, DFLT_SCREEN_ATTRIBS);

		//
		// draw the border
		//
		if (1)
		{
			wchar_t wcsTitle[] = { BOXDRAW_VERTICAL_LEFT_DOUBLE,BLOCKELEMENT_LIGHT_SHADE ,BLOCKELEMENT_LIGHT_SHADE ,BLOCKELEMENT_LIGHT_SHADE ,BLOCKELEMENT_LIGHT_SHADE ,BLOCKELEMENT_LIGHT_SHADE ,BLOCKELEMENT_LIGHT_SHADE ,
				0x20,L'R',L'T',L'C',L'S',L'p',L'y',L'+',0x20,
				BLOCKELEMENT_LIGHT_SHADE ,BLOCKELEMENT_LIGHT_SHADE ,BLOCKELEMENT_LIGHT_SHADE ,BLOCKELEMENT_LIGHT_SHADE ,BLOCKELEMENT_LIGHT_SHADE ,BLOCKELEMENT_LIGHT_SHADE,BOXDRAW_VERTICAL_RIGHT_DOUBLE,'\0'};


			FullScreen.TextBorder(
				{ 0,1 },
				{ FullScreen.WinDim.X, FullScreen.WinDim.Y - 2/*FullScreen.WinDim.Y*/ },
				//{ FullScreen.WinPos.X-10,FullScreen.WinPos.Y - 11 },
				BOXDRAW_DOUBLE_DOWN_RIGHT,
				BOXDRAW_DOUBLE_DOWN_LEFT,
				BOXDRAW_DOUBLE_UP_RIGHT,
				BOXDRAW_DOUBLE_UP_LEFT,
				BOXDRAW_DOUBLE_HORIZONTAL,
				BOXDRAW_DOUBLE_VERTICAL,
				&wcsTitle[0]
			);
		}
		//
		// draw the menu strings
		//
		for (int i = 0; i < ELC(menu); i++)		// initialize the menu strings
			FullScreen.TextPrint(menu[i].RelPos, menu[i].wcsMenuName);

		//
		// draw status/help line
		//
		if (1)
		{
			wchar_t wcsARROW_LEFT[2] = { ARROW_LEFT ,'\0' }, 
					wcsARROW_UP[2] = { ARROW_UP ,'\0' }, 
					wcsARROW_RIGHT[2] = { ARROW_RIGHT ,'\0' }, 
					wcsARROW_DOWN[2] = { ARROW_DOWN ,'\0'};
			FullScreen.TextPrint({  1, FullScreen.ScrDim.Y - 1 },EFI_BACKGROUND_BLUE|EFI_WHITE, L"F10:Menu    ENTER:Select   ESC:Return   %s%s%s%s:Navigate", wcsARROW_LEFT, wcsARROW_RIGHT, wcsARROW_UP, wcsARROW_DOWN);
		}

		//
		// the master loop
		//
		if (1)
		{
			enum STATE {
				MENU_DFLT,
				MENU_ENTER_ACTIVATION,
				MENU_IS_ACTIVE,
				MENU_IS_OPEN,
				/*MENU_123_IS_ACTIVE,*/
			}state = MENU_DFLT;
			int idxMenu = 0, idxMnuItm = 0;
			int pgress = 0;

			for (;false == gExit;)
			{
				//clock_t endclk = CLOCKS_PER_SEC / 8 + clock();
				//EFI_KEY_DATA KeyData = FullScreen.ReadKeyStrokeEx();
				if (0)
				{
					//
					// progress
					//
					static char* rgstrProgress[] = { "|", "/", "-", "\\" };
					pgress = (pgress + 1) % (sizeof(rgstrProgress) / sizeof(rgstrProgress[0]));
					FullScreen.TextPrint({ FullScreen.WinDim.X - 3,0 }, EFI_BACKGROUND_LIGHTGRAY + EFI_BLACK ,"[%s]", rgstrProgress[pgress]);
				}
				FullScreen.TextWindowUpdateProgress();

				key = FullScreen.TextGetKey();

				switch (state) {
				case MENU_ENTER_ACTIVATION:
					FullScreen.TextPrint(menu[idxMenu].RelPos, EFI_BACKGROUND_CYAN | EFI_YELLOW, menu[idxMenu].wcsMenuName);
					state = MENU_IS_ACTIVE;
					break;
				case MENU_IS_ACTIVE:
					if (KEY_ESC == key) {
						key = NO_KEY;
						state = MENU_DFLT;
						for (int i = 0; i < ELC(menu); i++)		// normalize the menu strings
							FullScreen.TextPrint(menu[i].RelPos, EFI_BACKGROUND_LIGHTGRAY | EFI_BLACK, menu[i].wcsMenuName);
					}
					else if (KEY_LEFT == key) {
						FullScreen.TextPrint(menu[idxMenu].RelPos, EFI_BACKGROUND_LIGHTGRAY | EFI_BLACK, menu[idxMenu].wcsMenuName);	
						idxMenu = ((idxMenu - 1) < 0) ? ELC(menu) - 1 : idxMenu - 1;
						FullScreen.TextPrint(menu[idxMenu].RelPos, EFI_BACKGROUND_CYAN | EFI_YELLOW, menu[idxMenu].wcsMenuName);
						key = NO_KEY;
					}
					else if (KEY_RIGHT == key) {
						FullScreen.TextPrint(menu[idxMenu].RelPos, EFI_BACKGROUND_LIGHTGRAY | EFI_BLACK, menu[idxMenu].wcsMenuName);
						idxMenu = ((idxMenu + 1) < ELC(menu)) ? idxMenu + 1 : 0;
						FullScreen.TextPrint(menu[idxMenu].RelPos, EFI_BACKGROUND_CYAN | EFI_YELLOW, menu[idxMenu].wcsMenuName);
						key = NO_KEY;
					}
					else if (KEY_ENTER == key) {
						menu[idxMenu].pTextWindow = new CTextWindow(&FullScreen,{ menu[idxMenu].RelPos.X, 2 }, menu[idxMenu].MnuDim, EFI_BACKGROUND_CYAN | EFI_YELLOW);
						menu[idxMenu].pTextWindow->TextBorder({ 0, 0 }, menu[idxMenu].MnuDim,
							BOXDRAW_DOWN_RIGHT,
							BOXDRAW_DOWN_LEFT,
							BOXDRAW_UP_RIGHT,
							BOXDRAW_UP_LEFT,
							BOXDRAW_HORIZONTAL,
							BOXDRAW_VERTICAL,
							nullptr);
						//
						// fill menu with menuitem strings
						//
						menu[idxMenu].pTextWindow->pwcsBlockDrawBuf[0] = '\0';	// initially terminate the string list
					
						for (int i = 0; /* NOTE: check for NULL string to terminate the list */menu[idxMenu].rgwcsMenuItem[i]; i++)
						{
							wchar_t* wcsList = menu[idxMenu].pTextWindow->pwcsBlockDrawBuf;
							size_t x = wcslen(wcsList);								// always get end of strings

							swprintf(&wcsList[x], UINT_MAX,L"%s\n", menu[idxMenu].rgwcsMenuItem[i]);
						}
						menu[idxMenu].pTextWindow->TextBlockDraw({ 2, 1 });
						idxMnuItm = 0;
						menu[idxMenu].pTextWindow->TextPrint({ 2,1 }, EFI_BACKGROUND_MAGENTA | EFI_YELLOW, menu[idxMenu].rgwcsMenuItem[0]);

						state = MENU_IS_OPEN;
						key = NO_KEY;
					}
					break;
				case MENU_IS_OPEN:
					if (KEY_ESC == key) {
						menu[idxMenu].pTextWindow->BgAtt = EFI_BACKGROUND_LIGHTGRAY | EFI_BLACK; // set background attribute
						delete menu[idxMenu].pTextWindow;
						FullScreen.TextBlockRfrsh();
						state = MENU_IS_ACTIVE;
					}
					else if (KEY_ENTER == key)
					{
						menu[idxMenu].pTextWindow->BgAtt = EFI_BACKGROUND_LIGHTGRAY | EFI_BLACK; // set background attribute
						(*menu[idxMenu].rgfnMnuItm[idxMnuItm])(menu[idxMenu].pTextWindow, nullptr);
						for (int i = 0; i < ELC(menu); i++)		// normalize the menu strings
							FullScreen.TextPrint(menu[i].RelPos, EFI_BACKGROUND_LIGHTGRAY | EFI_BLACK, menu[i].wcsMenuName);
						state = MENU_DFLT;
					}
					else if (KEY_DOWN == key) {
						int idxMnuItmNUM = menu[idxMenu].MnuDim.Y - 2/* number of lines within the pulldown menu */;

						menu[idxMenu].pTextWindow->TextPrint({ 2,idxMnuItm + 1 }, EFI_BACKGROUND_CYAN | EFI_YELLOW, menu[idxMenu].rgwcsMenuItem[idxMnuItm]);	// de-highlight previous menu item
						idxMnuItm = (++idxMnuItm == idxMnuItmNUM ? 0 : idxMnuItm);
						menu[idxMenu].pTextWindow->TextPrint({ 2,idxMnuItm + 1 }, EFI_BACKGROUND_MAGENTA | EFI_YELLOW, menu[idxMenu].rgwcsMenuItem[idxMnuItm]);	//    highlight current  menu item
					}
					else if (KEY_UP == key) {
						int idxMnuItmNUM = menu[idxMenu].MnuDim.Y - 2/* number of lines within the pulldown menu */;

						menu[idxMenu].pTextWindow->TextPrint({ 2,idxMnuItm + 1 }, EFI_BACKGROUND_CYAN | EFI_YELLOW, menu[idxMenu].rgwcsMenuItem[idxMnuItm]);	// de-highlight previous menu item
						idxMnuItm = (--idxMnuItm < 0 ? idxMnuItmNUM - 1 : idxMnuItm);
						menu[idxMenu].pTextWindow->TextPrint({ 2,idxMnuItm + 1 }, EFI_BACKGROUND_MAGENTA | EFI_YELLOW, menu[idxMenu].rgwcsMenuItem[idxMnuItm]);	//    highlight current  menu item
					}

					key = NO_KEY;
					break;
				case MENU_DFLT:
					if (KEY_F10 == key)
						state = MENU_ENTER_ACTIVATION,
						key = NO_KEY;
					if (true == gHexView)
					{
						//
						//	refresh the FullScreen Window
						//
						FullScreen.pwcsBlockDrawBuf[0] = '\0';
						for (int i = 0; i < 128 / TYPESIZE; i++)
							swprintf(&FullScreen.pwcsBlockDrawBuf[wcslen(FullScreen.pwcsBlockDrawBuf)],
								(const size_t)8192,
								(i % (16 / TYPESIZE) == 0 ? FORMATW_ADDR : FORMATWOADDR),
								(i % (16 / TYPESIZE) == 0 ? (void*)(i * TYPESIZE) : L""),
								TYPEMASK & RTCRD(i * TYPESIZE),
								((i + 1) % (16 / TYPESIZE)) ? (((i + 1) % (8 / TYPESIZE)) ? L" " : L" - ") : L"\n");

						FullScreen.TextBlockDraw({ FullScreen.WinPos.X + 3, FullScreen.WinPos.Y + 3 });
					}
					else {
						//
						// NOTE:The RTC handling below is TOO SIMPLE, because it IGNORES the UIP Update In Progress phase. 
						//      During that phase registers (calendar registers) have indetermined values.
						//      For that reason "%" division and string-length-equality is done for indexed strings.
						//
						static char* wday_name_long[7] = {
							"Sunday",
							"Monday",
							"Tuesday",
							"Wednesday",
							"Thursday",
							"Friday",
							"Saturday" };
						static char* mon_name_long[12] = {
							"January",
							"February",
							"March",
							"April",
							"May",
							"June",
							"July",
							"August",
							"September",
							"October",
							"November",
							"December" };

						FullScreen.pwcsBlockDrawBuf[0] = '\0';
						#define ENDOFDRAWBUF &FullScreen.pwcsBlockDrawBuf[wcslen(FullScreen.pwcsBlockDrawBuf)]

						swprintf(ENDOFDRAWBUF, (const size_t)INT_MAX,
							L"Date/Time: %hs, %X. %hs %02X%02X, %02X:%02X,%02X \n",
							wday_name_long[(rtcrd(6)) % 7],
							rtcrd(7),
							mon_name_long[-1 + (rtcrd(8)) % 12],
							rtcrd(0x32), rtcrd(9),
							0xFF & rtcrd(4),
							0xFF & rtcrd(2),
							0xFF & rtcrd(0)
						);
						swprintf(ENDOFDRAWBUF, (const size_t)INT_MAX,
							L" \nAlarm    : %02X:%02X,%02X \n", 0xFF & rtcrd(5), 0xFF & rtcrd(3), 0xFF & rtcrd(1));

						//
						// register A
						//
						typedef union _regA {
							uint8_t reg;
							struct {
								uint8_t RegA_RS0 : 1;
								uint8_t RegA_RS1 : 1;
								uint8_t RegA_RS2 : 1;
								uint8_t RegA_RS3 : 1;
								uint8_t RegA_DV0 : 1;
								uint8_t RegA_DV1 : 1;
								uint8_t RegA_DV2 : 1;
								uint8_t RegA_UIP : 1;
							}field;
						}REGA;
						REGA RegA;
						RegA.reg = (uint8_t)RTCRD(0x0A);
						swprintf(ENDOFDRAWBUF, INT_MAX, L" \nRegister A:\n    UIP %d, DV2 %d, DV1 %d, DV0 %d, RS3 %d, RS2 %d, RS1 %d, RS0 %d\n",
							RegA.field.RegA_RS0,
							RegA.field.RegA_RS1,
							RegA.field.RegA_RS2,
							RegA.field.RegA_RS3,
							RegA.field.RegA_DV0,
							RegA.field.RegA_DV1,
							RegA.field.RegA_DV2,
							RegA.field.RegA_UIP);

						//
						// register B
						//
						typedef union _regB {
							uint8_t reg;
							struct {
								uint8_t RegB_DSE : 1;
								uint8_t RegB_C2412 : 1;
								uint8_t RegB_DM : 1;
								uint8_t RegB_SQWE : 1;
								uint8_t RegB_UIE : 1;
								uint8_t RegB_AIE : 1;
								uint8_t RegB_PIE : 1;
								uint8_t RegB_SET : 1;
							}field;
						}REGB;
						REGB RegB;
						RegB.reg = (uint8_t)RTCRD(0x0B);
						swprintf(ENDOFDRAWBUF, INT_MAX, L" \nRegister B:\n    SET %d, PIE %d, AIE %d, UIE %d, SQWE %d, DM %d, 24/12 %d, DSE %d\n",
							RegB.field.RegB_SET,
							RegB.field.RegB_PIE,
							RegB.field.RegB_AIE,
							RegB.field.RegB_UIE,
							RegB.field.RegB_SQWE,
							RegB.field.RegB_DM,
							RegB.field.RegB_C2412,
							RegB.field.RegB_DSE);
						//
						// register C
						//
						typedef union _regC {
							uint8_t reg;
							struct {
								uint8_t RegC_B0 : 1;
								uint8_t RegC_B1 : 1;
								uint8_t RegC_B2 : 1;
								uint8_t RegC_B3 : 1;
								uint8_t RegC_UF : 1;
								uint8_t RegC_AF : 1;
								uint8_t RegC_PF : 1;
								uint8_t RegC_IRQF : 1;
							}field;
						}REGC;
						REGC RegC;
						RegC.reg = (uint8_t)RTCRD(0x0C);
						swprintf(ENDOFDRAWBUF, INT_MAX, L" \nRegister C:\n    IRQF %d, PF %d, AF %d, UF %d, B3 %d, B2 %d, B1 %d, B0 %d\n",
							RegC.field.RegC_IRQF,
							RegC.field.RegC_PF,
							RegC.field.RegC_AF,
							RegC.field.RegC_UF,
							RegC.field.RegC_B3,
							RegC.field.RegC_B2,
							RegC.field.RegC_B1,
							RegC.field.RegC_B0);
						//
						// register D
						//
						typedef union _regD {
							uint8_t reg;
							struct {
								uint8_t RegD_B0 : 1;
								uint8_t RegD_B1 : 1;
								uint8_t RegD_B2 : 1;
								uint8_t RegD_B3 : 1;
								uint8_t RegD_B4 : 1;
								uint8_t RegD_B5 : 1;
								uint8_t RegD_B6 : 1;
								uint8_t RegD_VRT : 1;
							}field;
						}REGD;
						REGD RegD;
						RegD.reg = (uint8_t)RTCRD(0x0D);
						swprintf(ENDOFDRAWBUF, INT_MAX, L" \nRegister D:\n    VRT %d, B6 %d, B5 %d, B4 %d, B3 %d, B2 %d, B1 %d, B0 %d\n",
							RegD.field.RegD_VRT,
							RegD.field.RegD_B6,
							RegD.field.RegD_B5,
							RegD.field.RegD_B4,
							RegD.field.RegD_B3,
							RegD.field.RegD_B2,
							RegD.field.RegD_B1,
							RegD.field.RegD_B0);
						FullScreen.TextBlockDraw({ FullScreen.WinPos.X + 3, FullScreen.WinPos.Y + 3 });

					}
					break;
				default:break;
				}

				key = NO_KEY;
			}
		}

		delete pc;
		delete pi;
		DPRINTF(("...exit0"));

	} while (false == gExit);

	DPRINTF(("...exit\n"));
	return nRet;
}